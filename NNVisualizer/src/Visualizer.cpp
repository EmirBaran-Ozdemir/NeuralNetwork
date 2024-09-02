#pragma once
#include "Visualizer.h"

namespace NNVisualizer {
	Visualizer::Visualizer() :
		m_hwnd(NULL),
		m_Direct2dFactory(NULL),
		m_DWriteFactory(NULL),
		m_TextFormat(NULL),
		m_RenderTarget(NULL),
		m_LooseWeightBrush(NULL),
		m_MediumWeightBrush(NULL),
		m_TightWeightBrush(NULL)
	{}

	Visualizer::~Visualizer()
	{
		SafeRelease(&m_Direct2dFactory);
		SafeRelease(&m_DWriteFactory);
		SafeRelease(&m_RenderTarget);
		SafeRelease(&m_TextFormat);
		SafeRelease(&m_LooseWeightBrush);
		SafeRelease(&m_MediumWeightBrush);
		SafeRelease(&m_TightWeightBrush);
	}
	HRESULT Visualizer::Initialize()
	{
		HRESULT hr;

		// Initialize device-independent resources, such
		// as the Direct2D factory.
		hr = CreateDeviceIndependentResources();

		if(SUCCEEDED(hr))
		{
			// Register the window class.
			WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = Visualizer::WndProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = sizeof(LONG_PTR);
			wcex.hInstance = HINST_THISCOMPONENT;
			wcex.hbrBackground = NULL;
			wcex.lpszMenuName = NULL;
			wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
			wcex.lpszClassName = "D2DVisualizer";

			RegisterClassEx(&wcex);

			// In terms of using the correct DPI, to create a window at a specific size
			// like this, the procedure is to first create the window hidden. Then we get
			// the actual DPI from the HWND (which will be assigned by whichever monitor
			// the window is created on). Then we use SetWindowPos to resize it to the
			// correct DPI-scaled size, then we use ShowWindow to show it.

			m_hwnd = CreateWindow(
				"D2DVisualizer",
				"NNVisualizer",
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				0,
				0,
				NULL,
				NULL,
				HINST_THISCOMPONENT,
				this);

			if(m_hwnd)
			{
				// Because the SetWindowPos function takes its size in pixels, we
				// obtain the window's DPI, and use it to scale the window size.
				float dpi = GetDpiForWindow(m_hwnd);
				m_ViewportWidth = static_cast<int>(ceil(1440.f * dpi / 96.f));
				m_ViewportHeight = static_cast<int>(ceil(820.f * dpi / 96.f));
				SetWindowPos(
					m_hwnd,
					NULL,
					NULL,
					NULL,
					m_ViewportHeight,
					m_ViewportHeight,
					SWP_NOMOVE);
				ShowWindow(m_hwnd, SW_SHOWNORMAL);
				UpdateWindow(m_hwnd);
			}
		}

		return hr;
	}

	HRESULT Visualizer::OnRender()
	{
		HRESULT hr = S_OK;

		hr = CreateDeviceResources();
		if(SUCCEEDED(hr))
		{
			m_RenderTarget->BeginDraw();
			m_RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			m_RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
			D2D1_SIZE_F rtSize = m_RenderTarget->GetSize();
			// Draw a grid background.
			int width = static_cast<int>(rtSize.width);
			int height = static_cast<int>(rtSize.height);

			//this->DrawNode();
			if(m_NeuralNetwork != nullptr)
			{
				LoopNN();

			}
			hr = m_RenderTarget->EndDraw();
		}
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_Direct2dFactory);

		if(SUCCEEDED(hr))
		{
			// Create a DirectWrite factory.
			hr = DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown**>(&m_DWriteFactory)
			);
		}
		if(hr == D2DERR_RECREATE_TARGET)
		{
			hr = S_OK;
			DiscardDeviceResources();
		}
		return hr;
	}
	void Visualizer::DrawNode(D2D1_POINT_2F position, float radius, double value)
	{
		// Draw circular node
		D2D1_ELLIPSE ellipse = D2D1::Ellipse(position, radius, radius);
		m_RenderTarget->DrawEllipse(&ellipse, m_LooseWeightBrush);

		// Convert the value to a wstring
		std::wstringstream wss;
		wss.precision(2);
		wss << std::fixed << value;
		std::wstring valueStr = wss.str();

		// Define a rectangle where the text will be drawn
		D2D1_RECT_F layoutRect = D2D1::RectF(
			position.x - radius,
			position.y - radius,
			position.x + radius,
			position.y + radius
		);

		// Draw the text inside the ellipse
		m_RenderTarget->DrawText(
			valueStr.c_str(),
			static_cast<UINT32>(valueStr.size()),
			m_TextFormat,
			layoutRect,
			m_LooseWeightBrush
		);
	}

	void Visualizer::DrawWeight(D2D1_POINT_2F start, D2D1_POINT_2F end, float weight)
	{
		// Adjust brush color or thickness based on weight if needed
		// Example: m_RenderTarget->CreateSolidColorBrush(...);
		// Draw line representing the weight
		if(weight < 0.33)
		{
			m_RenderTarget->DrawLine(start, end, m_LooseWeightBrush, weight);
		}
		else if(weight < 0.66)
		{
			m_RenderTarget->DrawLine(start, end, m_MediumWeightBrush, weight);

		}
		else
		{
			m_RenderTarget->DrawLine(start, end, m_TightWeightBrush, weight);

		}
	}

	void Visualizer::LoopNN()
	{
		const auto& layers = m_NeuralNetwork->GetLayers();
		const auto& weights = m_NeuralNetwork->GetWeights();
		size_t largestLayerSize = 0;

		for(size_t i = 0; i < layers.size(); ++i) {
			size_t layerSize = layers[i]->GetSize();

			if(layerSize > largestLayerSize) {
				largestLayerSize = layerSize;
			}
		}
		// Define layout parameters
		float nodeRadius = 10.0f;
		//float verticalSpacing = static_cast<float> (m_ViewportWidth / largestLayerSize);
		//float horizontalSpacing = static_cast<float> (m_ViewportWidth / layers.size());
		float verticalSpacing = 50.0f;
		float horizontalSpacing = 100.0f;

		// Loop through each layer to draw nodes
		for(size_t layerIndex = 0; layerIndex < layers.size(); ++layerIndex)
		{
			const auto& layer = layers[layerIndex];
			float xOffset = 20.0f + layerIndex * horizontalSpacing;

			// Draw each neuron in the layer
			for(size_t neuronIndex = 0; neuronIndex < layer->GetSize(); ++neuronIndex)
			{
				float yOffset = 20.0f + neuronIndex * verticalSpacing;
				D2D1_POINT_2F neuronPosition = D2D1::Point2F(xOffset, yOffset);

				double neuronValue = layer->GetNeurons()[neuronIndex]->GetActivatedValue();
				DrawNode(neuronPosition, nodeRadius, neuronValue);
			}

			// Draw weights to the next layer if it exists
			if(layerIndex < layers.size() - 1)
			{
				const auto& nextLayer = layers[layerIndex + 1];
				const auto& weightMatrix = weights[layerIndex]; // Matrix that connects current layer to next

				// Draw weights between current layer and next layer
				for(size_t neuronIndex = 0; neuronIndex < layer->GetSize(); ++neuronIndex)
				{
					float yOffsetStart = 20.0f + neuronIndex * verticalSpacing;
					D2D1_POINT_2F start = D2D1::Point2F(xOffset, yOffsetStart);

					for(size_t nextNeuronIndex = 0; nextNeuronIndex < nextLayer->GetSize(); ++nextNeuronIndex)
					{
						float yOffsetEnd = 20.0f + nextNeuronIndex * verticalSpacing;
						D2D1_POINT_2F end = D2D1::Point2F(xOffset + horizontalSpacing, yOffsetEnd );

						// Fetch the weight from the matrix
						double weight = weightMatrix->GetValue(neuronIndex, nextNeuronIndex); // Assumed method to get weight
						DrawWeight(start, end, static_cast<float>(weight));
					}
				}
			}
		}
	}


	void Visualizer::SetNN(std::unique_ptr<NNCore::NeuralNetwork> neuralNetwork)
	{
		m_NeuralNetwork = std::move(neuralNetwork);
	}

	void Visualizer::RunMessageLoop()
	{
		MSG msg;

		while(GetMessage(&msg, NULL, 0, 0))
		{
			auto res = TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	HRESULT Visualizer::CreateDeviceIndependentResources()
	{
		HRESULT hr = S_OK;

		// Create a Direct2D factory.
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_Direct2dFactory);
		if(SUCCEEDED(hr))
		{
			// Create a DirectWrite factory.
			hr = DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown**>(&m_DWriteFactory)
			);
		}
		return hr;
	}
	HRESULT Visualizer::CreateDeviceResources()
	{
		HRESULT hr = S_OK;

		if(!m_RenderTarget)
		{
			RECT rc;
			GetClientRect(m_hwnd, &rc);

			D2D1_SIZE_U size = D2D1::SizeU(
				rc.right - rc.left,
				rc.bottom - rc.top
			);

			// Create a Direct2D render target.
			hr = m_Direct2dFactory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(m_hwnd, size),
				&m_RenderTarget
			);

			if(SUCCEEDED(hr))
			{
				// Create a gray brush.
				hr = m_RenderTarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::LimeGreen),
					&m_LooseWeightBrush
				);
			}
			if(SUCCEEDED(hr))
			{
				// Create a gray brush.
				hr = m_RenderTarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::Yellow),
					&m_MediumWeightBrush
				);
			}
			if(SUCCEEDED(hr))
			{
				// Create a blue brush.
				hr = m_RenderTarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::Red),
					&m_TightWeightBrush
				);
			}
			if(SUCCEEDED(hr))
			{
				hr = m_DWriteFactory->CreateTextFormat(
					L"Sans Serif",    // Font family name.
					NULL,           // Font collection (NULL sets it to use the system font collection).
					DWRITE_FONT_WEIGHT_REGULAR,
					DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					8.0f,          // Font size.
					L"en-us",       // Locale.
					&m_TextFormat
				);
			}

			if(SUCCEEDED(hr))
			{
				// Center the text horizontally and vertically.
				m_TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				m_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			}
		}

		return hr;
	}

	void Visualizer::DiscardDeviceResources()
	{
		SafeRelease(&m_RenderTarget);
		SafeRelease(&m_DWriteFactory);
		SafeRelease(&m_TextFormat);
		SafeRelease(&m_LooseWeightBrush);
		SafeRelease(&m_MediumWeightBrush);
		SafeRelease(&m_TightWeightBrush);
	}


	LRESULT CALLBACK Visualizer::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT result = 0;

		if(message == WM_CREATE)
		{
			LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
			Visualizer* pVisualizer = (Visualizer*)pcs->lpCreateParams;

			::SetWindowLongPtrW(
				hwnd,
				GWLP_USERDATA,
				reinterpret_cast<LONG_PTR>(pVisualizer)
			);

			result = 1;
		}
		else
		{
			Visualizer* pVisualizer = reinterpret_cast<Visualizer*>(static_cast<LONG_PTR>(
				::GetWindowLongPtrW(
					hwnd,
					GWLP_USERDATA
				)));

			bool wasHandled = false;

			if(pVisualizer)
			{
				switch(message)
				{
				case WM_SIZE:
				{
					UINT width = LOWORD(lParam);
					UINT height = HIWORD(lParam);
					pVisualizer->OnResize(width, height);
				}
				result = 0;
				wasHandled = true;
				break;

				case WM_DISPLAYCHANGE:
				{
					InvalidateRect(hwnd, NULL, FALSE);
				}
				result = 0;
				wasHandled = true;
				break;

				case WM_PAINT:
				{
					pVisualizer->OnRender();
					ValidateRect(hwnd, NULL);
				}
				result = 0;
				wasHandled = true;
				break;

				case WM_DESTROY:
				{
					PostQuitMessage(0);
				}
				result = 1;
				wasHandled = true;
				break;
				}
			}

			if(!wasHandled)
			{
				result = DefWindowProc(hwnd, message, wParam, lParam);
			}
		}

		return result;
	}

	void Visualizer::OnResize(UINT width, UINT height)
	{
		if(m_RenderTarget)
		{
			// Note: This method can fail, but it's okay to ignore the
			// error here, because the error will be returned again
			// the next time EndDraw is called.
			m_RenderTarget->Resize(D2D1::SizeU(width, height));
		}
	}
}
