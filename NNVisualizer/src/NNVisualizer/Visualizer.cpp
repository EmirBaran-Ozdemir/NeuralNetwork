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
		m_TightWeightBrush(NULL),
		m_Camera(nullptr),
		m_ViewportWidth(0),
		m_ViewportHeight(0)
	{
	}

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
		HRESULT hr = S_OK;

		// Initialize device-independent resources, such as the Direct2D factory.
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
			wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
			wcex.lpszClassName = "D2DVisualizer";

			RegisterClassEx(&wcex);

			// Create the window hidden initially.
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
				this
			);

			if(m_hwnd)
			{
				// Adjust window size based on DPI.
				float dpi = GetDpiForWindow(m_hwnd);
				m_ViewportWidth = static_cast<int>(ceil(1440.f * dpi / 96.f));
				m_ViewportHeight = static_cast<int>(ceil(820.f * dpi / 96.f));
				SetWindowPos(
					m_hwnd,
					NULL,
					NULL,
					NULL,
					m_ViewportWidth,
					m_ViewportHeight,
					SWP_NOMOVE
				);
				m_Camera = new Renderer::Camera();

				// Show and update the window.
				ShowWindow(m_hwnd, SW_SHOWNORMAL);
				UpdateWindow(m_hwnd);

				// Invalidate the client area to trigger a WM_PAINT message.
				InvalidateRect(m_hwnd, NULL, FALSE);
			}
			else
			{
				hr = E_FAIL;
			}
		}

		return hr;
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
				hr = m_RenderTarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::LightGreen),
					&m_LooseWeightBrush
				);
				if(FAILED(hr)) { OutputDebugString("Failed to create LooseWeightBrush\n"); }
			}
			if(SUCCEEDED(hr))
			{
				hr = m_RenderTarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::Yellow),
					&m_MediumWeightBrush
				);
				if(FAILED(hr)) { OutputDebugString("Failed to create MediumWeightBrush\n"); }
			}
			if(SUCCEEDED(hr))
			{
				hr = m_RenderTarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::Red),
					&m_TightWeightBrush
				);
				if(FAILED(hr)) { OutputDebugString("Failed to create TightWeightBrush\n"); }
			}
			if(SUCCEEDED(hr))
			{
				hr = m_RenderTarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF(D2D1::ColorF::LightGray, 0.6)),
					&m_GrayBrush
				);
				if(FAILED(hr)) { OutputDebugString("Failed to create DisabledBrush\n"); }
			}
			if(SUCCEEDED(hr))
			{
				hr = m_RenderTarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::Black),
					&m_BlackBrush
				);
				if(FAILED(hr)) { OutputDebugString("Failed to create DisabledBrush\n"); }
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



	void Visualizer::RunMessageLoop()
	{
		MSG msg;

		while(GetMessage(&msg, NULL, 0, 0))
		{
			auto res = TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	HRESULT Visualizer::OnRender()
	{
		HRESULT hr = CreateDeviceResources();
		if(SUCCEEDED(hr)) {
			m_RenderTarget->BeginDraw();
			D2D1_MATRIX_4X4_F viewMatrix = m_Camera->GetViewMatrix();
			m_RenderTarget->SetTransform(Matrix4x4ToMatrix3x2(viewMatrix));
			m_RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

			// Render logic here
			if(m_NeuralNetwork != nullptr) {
				LoopNN();
			}

			hr = m_RenderTarget->EndDraw();
			if(hr == D2DERR_RECREATE_TARGET) {
				hr = S_OK;
				DiscardDeviceResources();
			}
		}
		return hr;
	}


	void Visualizer::LoopNN()
	{
		const auto& layers = m_NeuralNetwork->GetLayers();
		const auto& weights = m_NeuralNetwork->GetWeights();

		// Loop through each layer to draw nodes
		for(size_t layerIndex = 0; layerIndex < layers.size(); ++layerIndex)
		{
			const auto& layer = layers[layerIndex];
			int layerSize = layer->GetSize();
			float xOffset = 20.0f + layerIndex * m_HorizontalSpacing;
			// Top gap for centering
			float yStartingGap = ((float(m_LargestLayerSize - layerSize) / 2) * (m_VerticalSpacing));

			for(size_t neuronIndex = 0; neuronIndex < layerSize; ++neuronIndex)
			{
				bool isChoosen = false;
				float yOffset = yStartingGap + 20.0f + neuronIndex * m_VerticalSpacing;
				D2D1_POINT_2F neuronPosition = D2D1::Point2F(xOffset, yOffset);

				double neuronValue = layer->GetNeurons()[neuronIndex]->GetActivatedValue();
				if(m_ChoosenNeuron != nullptr && (layer->GetNeurons()[neuronIndex].get() == m_ChoosenNeuron))
				{
					isChoosen = true;
				}
				DrawNode(neuronPosition, m_NodeRadius, neuronValue, isChoosen);
			}

			if(layerIndex < layers.size() - 1)
			{
				const auto& nextLayer = layers[layerIndex + 1];
				const auto& weightMatrix = weights[layerIndex];

				for(size_t neuronIndex = 0; neuronIndex < layerSize; ++neuronIndex)
				{
					float yOffsetStart = yStartingGap + 20.0f + neuronIndex * m_VerticalSpacing;
					D2D1_POINT_2F start = D2D1::Point2F(xOffset + m_NodeRadius, yOffsetStart);

					for(size_t nextNeuronIndex = 0; nextNeuronIndex < nextLayer->GetSize(); ++nextNeuronIndex)
					{
						bool isChoosen = false;

						float yNextStartingGap = ((float(m_LargestLayerSize - nextLayer->GetSize()) / 2) * (m_VerticalSpacing));

						float yOffsetEnd = yNextStartingGap + 20.0f + nextNeuronIndex * m_VerticalSpacing;
						D2D1_POINT_2F end = D2D1::Point2F(xOffset + m_HorizontalSpacing - m_NodeRadius, yOffsetEnd);

						double weight = weightMatrix->GetValue(neuronIndex, nextNeuronIndex);
						if(m_ChoosenNeuron != nullptr && (layer->GetNeurons()[neuronIndex].get() == m_ChoosenNeuron || nextLayer->GetNeurons()[nextNeuronIndex].get() == m_ChoosenNeuron))
						{
							isChoosen = true;
						}
						DrawWeight(start, end, static_cast<float>(weight), isChoosen);
					}
				}
			}
		}
	}


	void Visualizer::DrawNode(D2D1_POINT_2F position, float radius, double value, bool isChoosen)
	{
		ID2D1SolidColorBrush* brush;
		if(!m_AnyNeuronChoosed || isChoosen)
		{
			brush = m_BlackBrush;
		}
		else
		{
			brush = m_GrayBrush;
		}

		D2D1_ELLIPSE ellipse = D2D1::Ellipse(position, radius, radius);
		m_RenderTarget->DrawEllipse(&ellipse, brush);

		// Draw the text
		std::wstringstream wss;
		wss.precision(2);
		wss << std::fixed << value;
		std::wstring valueStr = wss.str();

		D2D1_RECT_F layoutRect = D2D1::RectF(
			position.x + radius,
			position.y + radius,
			position.x - radius,
			position.y - radius
		);

		m_RenderTarget->DrawText(
			valueStr.c_str(),
			static_cast<UINT32>(valueStr.size()),
			m_TextFormat,
			layoutRect,
			brush
		);
	}

	void Visualizer::DrawWeight(D2D1_POINT_2F start, D2D1_POINT_2F end, float weight, bool isConnectedToChoosen)
	{
		ID2D1SolidColorBrush* brush;
		if(!m_AnyNeuronChoosed || isConnectedToChoosen)
		{
			if(weight < 0.33)
			{
				brush = m_LooseWeightBrush;
			}
			else if(weight < 0.66)
			{
				brush = m_MediumWeightBrush;
			}
			else
			{
				brush = m_TightWeightBrush;
			}
		}
		else
		{
			brush = m_GrayBrush;
		}

		m_RenderTarget->DrawLine(start, end, brush, 0.5f);
	}


	LRESULT CALLBACK Visualizer::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT result = 0;
		if(message == WM_CREATE)
		{
			LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
			Visualizer* pVisualizer = (Visualizer*)pcs->lpCreateParams;
			::SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pVisualizer));
			result = 1;
		}
		else
		{
			Visualizer* pVisualizer = reinterpret_cast<Visualizer*>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA));
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
					result = 0;
					wasHandled = true;
					break;
				}
				case WM_MOUSEWHEEL:
				{
					int delta = GET_WHEEL_DELTA_WPARAM(wParam);
					POINT cursorPos;
					GetCursorPos(&cursorPos);
					ScreenToClient(hwnd, &cursorPos);

					if(delta > 0)
					{
						pVisualizer->ZoomIn(static_cast<float>(cursorPos.x), static_cast<float>(cursorPos.y));
					}
					else
					{
						pVisualizer->ZoomOut(static_cast<float>(cursorPos.x), static_cast<float>(cursorPos.y));
					}

					pVisualizer->UpdateTextFormat();
					result = 0;
					wasHandled = true;
					break;
				}
				case WM_KEYDOWN:
				{
					switch(wParam)
					{
					case VK_LEFT:
						pVisualizer->MoveCameraLeft(10.0f);
						break;
					case VK_RIGHT:
						pVisualizer->MoveCameraRight(10.0f);
						break;
					case VK_UP:
						pVisualizer->MoveCameraUp(10.0f);
						break;
					case VK_DOWN:
						pVisualizer->MoveCameraDown(10.0f);
						break;
					}

					result = 0;
					wasHandled = true;
					break;
				}
				case WM_LBUTTONDOWN:
				{
					POINT cursorPos;
					GetCursorPos(&cursorPos);
					ScreenToClient(hwnd, &cursorPos);
					std::cout << cursorPos.x << " " << cursorPos.y << std::endl;
					pVisualizer->CheckNodeClick(cursorPos.x, cursorPos.y);

					result = 0;
					wasHandled = true;
					break;
				}
				case WM_RBUTTONDOWN:
				{
					result = 0;
					wasHandled = true;
					break;
				}
				case WM_MOUSEMOVE:
				{
					InvalidateRect(hwnd, NULL, FALSE);
					result = 0;
					wasHandled = true;
					break;
				}
				case WM_DISPLAYCHANGE:
				{
					InvalidateRect(hwnd, NULL, FALSE);
					result = 0;
					wasHandled = true;
					break;
				}
				case WM_PAINT:
				{
					pVisualizer->OnRender();
					ValidateRect(hwnd, NULL);
					result = 0;
					wasHandled = true;
					break;
				}
				case WM_DESTROY:
				{
					PostQuitMessage(0);
					result = 1;
					wasHandled = true;
					break;
				}
				}
			}

			if(!wasHandled)
			{
				result = DefWindowProc(hwnd, message, wParam, lParam);
			}
		}

		return result;
	}


	void Visualizer::MoveCameraLeft(float distance)
	{
		m_Camera->MoveLeft(distance);
		InvalidateRect(m_hwnd, NULL, FALSE);
	}

	void Visualizer::MoveCameraRight(float distance)
	{
		m_Camera->MoveRight(distance);
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
	void Visualizer::MoveCameraUp(float distance)
	{
		m_Camera->MoveUp(distance);
		InvalidateRect(m_hwnd, NULL, FALSE);
	}

	void Visualizer::MoveCameraDown(float distance)
	{
		m_Camera->MoveDown(distance);
		InvalidateRect(m_hwnd, NULL, FALSE);
	}

	void Visualizer::ZoomIn(float cursorX, float cursorY)
	{
		float newZoomFactor = m_Camera->GetZoomFactor() * 1.1f; // Increase zoom factor by 10%
		m_Camera->Zoom(newZoomFactor, D2D1::Point2(cursorX, cursorY));
		InvalidateRect(m_hwnd, NULL, FALSE);
	}

	void Visualizer::ZoomOut(float cursorX, float cursorY)
	{
		float newZoomFactor = m_Camera->GetZoomFactor() / 1.1f; // Decrease zoom factor by 10%
		m_Camera->Zoom(newZoomFactor, D2D1::Point2(cursorX, cursorY));
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
	void Visualizer::SetNN(std::unique_ptr<NNCore::NeuralNetwork> neuralNetwork)
	{
		m_NeuralNetwork = std::move(neuralNetwork);
		const auto& layers = m_NeuralNetwork->GetLayers();

		for(const auto& layer : layers)
		{
			if(layer->GetSize() > m_LargestLayerSize)
			{
				m_LargestLayerSize = layer->GetSize();
			}
		}

		m_NodeRadius = 10.0f;
		m_VerticalSpacing = std::max(20.0f, static_cast<float>(m_ViewportHeight / (m_LargestLayerSize + 1)));
		m_HorizontalSpacing = std::max(20.0f, static_cast<float>(m_ViewportWidth / (layers.size() + 1)));
		m_Camera->SetPosition(0, 200.0f);
		UpdateWindow(m_hwnd);
		InvalidateRect(m_hwnd, NULL, FALSE);
	}

	void Visualizer::CheckNodeClick(int mouseX, int mouseY)
	{
		const auto& layers = m_NeuralNetwork->GetLayers();
		D2D1_POINT_2F worldCursorPos = m_Camera->GetCursorWorldPosition(D2D1::Point2F(mouseX, mouseY));
		for(const auto& layer : layers)
		{
			if(layer->GetSize() > m_LargestLayerSize)
			{
				m_LargestLayerSize = layer->GetSize();
			}
		}

		float nodeRadius = 10.0f;
		float verticalSpacing = std::max(20.0f, static_cast<float>(m_ViewportHeight / (m_LargestLayerSize + 1)));
		float horizontalSpacing = std::max(20.0f, static_cast<float>(m_ViewportWidth / (layers.size() + 1)));
		bool clickedOnAny = false;
		for(size_t layerIndex = 0; layerIndex < layers.size(); ++layerIndex)
		{
			const auto& layer = layers[layerIndex];
			int layerSize = layer->GetSize();
			float xOffset = 20.0f + layerIndex * horizontalSpacing;
			float yStartingGap = ((float(m_LargestLayerSize - layerSize) / 2) * (verticalSpacing));

			for(size_t neuronIndex = 0; neuronIndex < layerSize; ++neuronIndex)
			{
				float yOffset = yStartingGap + 20.0f + neuronIndex * verticalSpacing;
				D2D1_POINT_2F neuronPosition = D2D1::Point2F(xOffset, yOffset);

				// Check if the click is within the radius of the node
				float distance = sqrt(pow(neuronPosition.x - worldCursorPos.x, 2) + pow(neuronPosition.y - worldCursorPos.y, 2));

				if(distance <= nodeRadius)
				{
					clickedOnAny = true;
					m_AnyNeuronChoosed = true;
					m_ChoosenNeuron = layer->GetNeurons()[neuronIndex].get();
					std::cout << "Node clicked: Layer " << layerIndex << " Neuron " << neuronIndex << std::endl;
					InvalidateRect(m_hwnd, NULL, FALSE);
					return;
				}
			}
		}
		if(!clickedOnAny)
		{
			m_ChoosenNeuron = nullptr;
			m_AnyNeuronChoosed = false;
			InvalidateRect(m_hwnd, NULL, FALSE);
		}
	}



	void Visualizer::UpdateTextFormat()
	{
		SafeRelease(&m_TextFormat);

		HRESULT hr = m_DWriteFactory->CreateTextFormat(
			L"Sans Serif",
			NULL,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			8.0f,
			L"en-us",
			&m_TextFormat
		);

		if(SUCCEEDED(hr))
		{
			// Center the text horizontally and vertically.
			m_TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			m_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		}
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
	void Visualizer::DiscardDeviceResources()
	{
		SafeRelease(&m_RenderTarget);
		SafeRelease(&m_DWriteFactory);
		SafeRelease(&m_TextFormat);
		SafeRelease(&m_LooseWeightBrush);
		SafeRelease(&m_MediumWeightBrush);
		SafeRelease(&m_TightWeightBrush);
		SafeRelease(&m_GrayBrush);
	}

}
