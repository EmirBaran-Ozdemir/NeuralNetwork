#pragma once
#include "Visualizer.h"

namespace NNVisualizer {

	Visualizer::Visualizer() :
		m_hwnd(NULL),
		m_Direct2dFactory(NULL),
		m_DWriteFactory(NULL),
		m_TextFormat(NULL),
		m_MenuTextFormat(NULL),
		m_ErrorTextFormat(NULL),
		m_RenderTarget(NULL),
		m_LooseWeightBrush(NULL),
		m_MediumWeightBrush(NULL),
		m_TightWeightBrush(NULL),
		m_BlackBrush(NULL),
		m_LimeGreenBrush(NULL),
		m_GrayBrush(NULL),
		m_WhiteBrush(NULL),
		m_HorizontalSpacing(0),
		m_VerticalSpacing(0),
		m_Camera(nullptr),
		m_ViewportWidth(0),
		m_ViewportHeight(0)
	{
	}

	Visualizer::~Visualizer()
	{
		SafeRelease(&m_RenderTarget);
		SafeRelease(&m_DWriteFactory);
		SafeRelease(&m_TextFormat);
		SafeRelease(&m_MenuTextFormat);
		SafeRelease(&m_ErrorTextFormat);
		SafeRelease(&m_LooseWeightBrush);
		SafeRelease(&m_MediumWeightBrush);
		SafeRelease(&m_TightWeightBrush);
		SafeRelease(&m_GrayBrush);
		SafeRelease(&m_BlackBrush);
		SafeRelease(&m_WhiteBrush);
		SafeRelease(&m_LimeGreenBrush);
		if (m_TrainingThread.joinable())
			m_TrainingThread.join();
	}

	HRESULT Visualizer::Initialize()
	{
		HRESULT hr = S_OK;

		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_Direct2dFactory);

		if (SUCCEEDED(hr))
		{
			hr = DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown**>(&m_DWriteFactory)
			);
		}

		if (SUCCEEDED(hr))
		{
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

			if (m_hwnd)
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

				ShowWindow(m_hwnd, SW_SHOWNORMAL);
				UpdateWindow(m_hwnd);

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

		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_Direct2dFactory);
		if (SUCCEEDED(hr))
		{
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

		if (!m_RenderTarget)
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

			if (SUCCEEDED(hr))
			{
				hr = m_RenderTarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::LightGreen),
					&m_LooseWeightBrush
				);
				if (FAILED(hr)) { OutputDebugString("Failed to create LooseWeightBrush\n"); }
			}
			if (SUCCEEDED(hr))
			{
				hr = m_RenderTarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::Yellow),
					&m_MediumWeightBrush
				);
				if (FAILED(hr)) { OutputDebugString("Failed to create MediumWeightBrush\n"); }
			}
			if (SUCCEEDED(hr))
			{
				hr = m_RenderTarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::Red),
					&m_TightWeightBrush
				);
				if (FAILED(hr)) { OutputDebugString("Failed to create TightWeightBrush\n"); }
			}
			if (SUCCEEDED(hr))
			{
				hr = m_RenderTarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF(D2D1::ColorF::LightGray, 0.6)),
					&m_GrayBrush
				);
				if (FAILED(hr)) { OutputDebugString("Failed to create DisabledBrush\n"); }
			}
			if (SUCCEEDED(hr))
			{
				hr = m_RenderTarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::Black),
					&m_BlackBrush
				);
				if (FAILED(hr)) { OutputDebugString("Failed to create DisabledBrush\n"); }
			}
			if (SUCCEEDED(hr))
			{
				hr = m_RenderTarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF(D2D1::ColorF::LimeGreen, 0.4)),
					&m_LimeGreenBrush
				);
				if (FAILED(hr)) { OutputDebugString("Failed to create DisabledBrush\n"); }
			}
			if (SUCCEEDED(hr))
			{
				hr = m_RenderTarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF(D2D1::ColorF::White)),
					&m_WhiteBrush
				);
				if (FAILED(hr)) { OutputDebugString("Failed to create DisabledBrush\n"); }
			}

			if (SUCCEEDED(hr))
			{
				hr = m_DWriteFactory->CreateTextFormat(
					L"Sans Serif",
					NULL,
					DWRITE_FONT_WEIGHT_REGULAR,
					DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					8.0f,
					L"en-us",
					&m_TextFormat
				);
				hr = m_DWriteFactory->CreateTextFormat(
					L"Sans Serif",
					NULL,
					DWRITE_FONT_WEIGHT_REGULAR,
					DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					24.0f,
					L"en-us",
					&m_ErrorTextFormat
				);
				hr = m_DWriteFactory->CreateTextFormat(
					L"Sans Serif",
					NULL,
					DWRITE_FONT_WEIGHT_REGULAR,
					DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					16.0f,
					L"en-us",
					&m_MenuTextFormat
				);
			}

			m_InitializeButton = std::make_unique<Components::Button>(L"Initialize");
			m_StartButton = std::make_unique<Components::Button>(L"Start");
			m_StepButton = std::make_unique<Components::Button>(L"Step");
			m_StopButton = std::make_unique<Components::Button>(L"Stop");
			m_ActivationFunctionDropdown = std::make_unique<Components::Dropdown>(L"Activation Function", NNCore::NeuronActivation::GetAllActivationFunctions());
			m_TopologyTextField = std::make_shared<Components::TextField>(L"Topology");
			m_StartingInputsTextField = std::make_shared<Components::TextField>(L"Starting Inputs");
			m_TargetOutputsTextField = std::make_shared<Components::TextField>(L"Target Outpus");
			m_MaxEpochTextField = std::make_shared<Components::TextField>(L"Max Epoch");

			if (SUCCEEDED(hr))
			{
				m_TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				m_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				m_MenuTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				m_MenuTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
				m_ErrorTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				m_ErrorTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			}
		}
		return hr;
	}

	void Visualizer::SetNN(std::unique_ptr<NNCore::NeuralNetwork> neuralNetwork)
	{
		m_NeuralNetwork = std::move(neuralNetwork);
		const auto& layers = m_NeuralNetwork->GetLayers();
		m_LayerExecutionTimeTextField = std::make_shared<Components::TextField>(std::to_wstring(m_NeuralNetwork->GetDisplayProperties().currentEpoch));

		for (const auto& layer : layers)
		{
			if (layer->GetSize() > m_LargestLayerSize)
			{
				m_LargestLayerSize = layer->GetSize();
			}
		}

		m_NodeRadius = 10.0f;
		m_VerticalSpacing = std::max(25.0f, static_cast<float>(m_ViewportHeight / (m_LargestLayerSize + 1)));

		//todo: Calculate max using verticalspacing
		m_HorizontalSpacing = std::max(120.0f, static_cast<float>(m_ViewportWidth / (layers.size() + 1)));
		m_Camera->SetPosition(0, 200.0f);
		UpdateWindow(m_hwnd);
		InvalidateRect(m_hwnd, NULL, FALSE);

	}

	void Visualizer::RunMessageLoop()
	{
		MSG msg;

		while (GetMessage(&msg, NULL, 0, 0))
		{
			auto res = TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	HRESULT Visualizer::OnRender()
	{
		HRESULT hr = CreateDeviceResources();
		if (SUCCEEDED(hr))
		{
			m_RenderTarget->BeginDraw();

			D2D1_MATRIX_4X4_F viewMatrix = m_Camera->GetViewMatrix();
			m_RenderTarget->SetTransform(Matrix4x4ToMatrix3x2(viewMatrix));
			m_RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

			if (m_NeuralNetwork != nullptr)
			{
				this->LoopNN();

			}

			D2D1_MATRIX_3X2_F previousTransform;
			m_RenderTarget->GetTransform(&previousTransform);

			m_RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

			if (!m_Initialized)
			{
				if (!m_ExceptionMessage.empty())
				{
					m_RenderTarget->DrawText(
						m_ExceptionMessage.c_str(),
						static_cast<UINT32>(m_ExceptionMessage.size()),
						m_ErrorTextFormat,
						D2D1::RectF(
							000.0f,
							000.0f,
							m_ViewportWidth,
							m_ViewportHeight
						),
						m_BlackBrush
					);
				}
				float componentWidth = 200.0f;
				float componentHeight = 50.0f;
				float componentXSpacing = 10.0f;
				// has 4 buttons so move center 2 buttons left
				float xCenter = static_cast<float>(m_ViewportWidth) / 2 - ((componentWidth * 2.5f) + (componentXSpacing * 2.0f));
				float yCenter = static_cast<float>(m_ViewportHeight) / 2 - (200.0f);
				m_InitializeButton->Draw(m_RenderTarget, m_BlackBrush, m_MenuTextFormat, xCenter + (componentWidth * 2.0f + componentXSpacing * 2.0f), yCenter - 80.0f, componentWidth, componentHeight);
				float componentXOffset = 0.0f;
				m_ActivationFunctionDropdown->Draw(m_RenderTarget, m_BlackBrush, m_MenuTextFormat, xCenter + componentXOffset, yCenter, componentWidth, componentHeight);
				componentXOffset += componentWidth + componentXSpacing;
				this->DrawTextField(m_TopologyTextField, m_SelectedTextField, m_RenderTarget, m_BlackBrush, m_LimeGreenBrush, m_BlackBrush, m_WhiteBrush, m_MenuTextFormat, xCenter + componentXOffset, yCenter, componentWidth, componentHeight);
				componentXOffset += componentWidth + componentXSpacing;
				this->DrawTextField(m_StartingInputsTextField, m_SelectedTextField, m_RenderTarget, m_BlackBrush, m_LimeGreenBrush, m_BlackBrush, m_WhiteBrush, m_MenuTextFormat, xCenter + componentXOffset, yCenter, componentWidth, componentHeight);
				componentXOffset += componentWidth + componentXSpacing;
				this->DrawTextField(m_TargetOutputsTextField, m_SelectedTextField, m_RenderTarget, m_BlackBrush, m_LimeGreenBrush, m_BlackBrush, m_WhiteBrush, m_MenuTextFormat, xCenter + componentXOffset, yCenter, componentWidth, componentHeight);
				componentXOffset += componentWidth + componentXSpacing;
				this->DrawTextField(m_MaxEpochTextField, m_SelectedTextField, m_RenderTarget, m_BlackBrush, m_LimeGreenBrush, m_BlackBrush, m_WhiteBrush, m_MenuTextFormat, xCenter + componentXOffset, yCenter, componentWidth, componentHeight);
			}
			else
			{
				this->DrawProperties();
				m_StartButton->Draw(m_RenderTarget, m_BlackBrush, m_MenuTextFormat, 10.0f, 10.0f, 100.0f, 50.0f);
				m_StepButton->Draw(m_RenderTarget, m_BlackBrush, m_MenuTextFormat, 110.0f, 10.0f, 100.0f, 50.0f);
				m_StopButton->Draw(m_RenderTarget, m_BlackBrush, m_MenuTextFormat, 210.0f, 10.0f, 100.0f, 50.0f);
			}

			m_RenderTarget->SetTransform(previousTransform);

			hr = m_RenderTarget->EndDraw();
			if (hr == D2DERR_RECREATE_TARGET)
			{
				hr = S_OK;
				this->DiscardDeviceResources();
			}
		}

		return hr;
	}

	void Visualizer::LoopNN()
	{
		const auto& layers = m_NeuralNetwork->GetLayers();
		const auto& weights = m_NeuralNetwork->GetWeights();

		for (size_t layerIndex = 0; layerIndex < layers.size(); ++layerIndex)
		{
			const auto& layer = layers[layerIndex];
			int layerSize = layer->GetSize();
			float xOffset = 20.0f + layerIndex * m_HorizontalSpacing;
			float yStartingGap = ((float(m_LargestLayerSize - layerSize) / 2) * (m_VerticalSpacing));

			for (size_t neuronIndex = 0; neuronIndex < layerSize; ++neuronIndex)
			{
				bool isChoosen = false;
				bool isProcessing = false;
				float yOffset = yStartingGap + 20.0f + neuronIndex * m_VerticalSpacing;
				D2D1_POINT_2F neuronPosition = D2D1::Point2F(xOffset, yOffset);

				double neuronValue = layer->GetNeurons()[neuronIndex]->GetBaseValue();
				if ((m_ChoosenNeuronColRow.first == layerIndex) && (m_ChoosenNeuronColRow.second == neuronIndex)) isChoosen = true;
				if (m_NeuralNetwork->GetProcessingNeuronColRow().first == layerIndex && m_NeuralNetwork->GetProcessingNeuronColRow().second == neuronIndex) isProcessing = true;
				this->DrawNode(neuronPosition, m_NodeRadius, neuronValue, isChoosen, isProcessing);
			}

			if (layerIndex < layers.size() - 1)
			{
				const auto& nextLayer = layers[layerIndex + 1];
				const auto& weightMatrix = weights[layerIndex];

				for (size_t neuronIndex = 0; neuronIndex < layerSize; ++neuronIndex)
				{
					float yOffsetStart = yStartingGap + 20.0f + neuronIndex * m_VerticalSpacing;
					D2D1_POINT_2F start = D2D1::Point2F(xOffset + m_NodeRadius, yOffsetStart);

					for (size_t nextNeuronIndex = 0; nextNeuronIndex < nextLayer->GetSize(); ++nextNeuronIndex)
					{
						bool isChoosen = false;

						float yNextStartingGap = ((float(m_LargestLayerSize - nextLayer->GetSize()) / 2) * (m_VerticalSpacing));

						float yOffsetEnd = yNextStartingGap + 20.0f + nextNeuronIndex * m_VerticalSpacing;
						D2D1_POINT_2F end = D2D1::Point2F(xOffset + m_HorizontalSpacing - m_NodeRadius, yOffsetEnd);

						double weight = weightMatrix->GetValue(neuronIndex, nextNeuronIndex);
						if (((m_ChoosenNeuronColRow.first == layerIndex) && (m_ChoosenNeuronColRow.second == neuronIndex)) || ((m_ChoosenNeuronColRow.first == layerIndex + 1) && (m_ChoosenNeuronColRow.second == nextNeuronIndex)))
						{
							isChoosen = true;
						}
						this->DrawWeight(start, end, static_cast<float>(weight), isChoosen);
					}
				}
			}
		}
	}

	void Visualizer::DrawProperties()
	{
		auto& properties = m_NeuralNetwork->GetDisplayProperties();
		D2D1_RECT_F layoutRect = D2D1::RectF(
			m_ViewportWidth - 200,
			20,
			m_ViewportWidth - 50,
			70
		);

		// Activation Function
		std::wstring activationFunctionString = m_WStringConverter.from_bytes(
			"ActivationFunction: " + NNCore::NeuronActivation::GetActivationFunctionString(properties.activationFunction)
		);
		this->DrawProperty(activationFunctionString, layoutRect);

		// Epoch Status
		std::wstring epochStatusString = m_WStringConverter.from_bytes(
			"Current/Total epoch: " + std::to_string(properties.currentEpoch) + " / " + std::to_string(properties.maxEpoch)
		);
		this->DrawProperty(epochStatusString, layoutRect);

		// Cost
		std::wstring costString = m_WStringConverter.from_bytes("Cost: " + std::to_string(properties.cost));
		this->DrawProperty(costString, layoutRect);

		// Layer Time
		if (m_NeuralNetwork->GetLoopState() == NNCore::LoopState::Stopped)
		{
			this->DrawTextField(m_LayerExecutionTimeTextField, m_SelectedTextField, m_RenderTarget, m_BlackBrush, m_LimeGreenBrush, m_BlackBrush, m_WhiteBrush, m_MenuTextFormat, &layoutRect);
		}
	}

	void Visualizer::DrawProperty(const std::wstring& text, D2D1_RECT_F& layoutRect)
	{
		m_RenderTarget->DrawTextA(text.c_str(), static_cast<UINT32>(text.size()), m_MenuTextFormat, layoutRect, m_BlackBrush);
		D2D1_RECT_F borderRect = D2D1::RectF(
			layoutRect.left - 5,
			layoutRect.top - 5,
			layoutRect.right + 5,
			layoutRect.bottom + 5
		);
		m_RenderTarget->DrawRectangle(borderRect, m_BlackBrush);
		layoutRect.top += 60;
		layoutRect.bottom += 60;
	}


	void Visualizer::DrawNode(D2D1_POINT_2F position, float radius, double value, bool isChoosen, bool isProcessing)
	{
		ID2D1SolidColorBrush* brush = m_BlackBrush;
		if (isChoosen)
		{
			brush = m_BlackBrush;
		}
		else if (isProcessing)
		{
			brush = m_LimeGreenBrush;
		}
		else if ((m_ChoosenNeuronColRow.first != -1) && (m_ChoosenNeuronColRow.second != -1))
		{
			brush = m_GrayBrush;
		}


		D2D1_ELLIPSE ellipse = D2D1::Ellipse(position, radius, radius);
		m_RenderTarget->DrawEllipse(&ellipse, brush);

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
		if (((m_ChoosenNeuronColRow.first == -1) && (m_ChoosenNeuronColRow.second == -1)) || isConnectedToChoosen)
		{
			if (weight < 0.33)
			{
				brush = m_LooseWeightBrush;
			}
			else if (weight < 0.66)
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
		if (message == WM_CREATE)
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
			POINT screenCursorPos;
			GetCursorPos(&screenCursorPos);
			POINT worldCursorPos = screenCursorPos;
			ScreenToClient(hwnd, &worldCursorPos);

			if (pVisualizer)
			{
				switch (message)
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
					if (delta > 0)
					{
						pVisualizer->ZoomIn(static_cast<float>(worldCursorPos.x), static_cast<float>(worldCursorPos.y));
					}
					else
					{
						pVisualizer->ZoomOut(static_cast<float>(worldCursorPos.x), static_cast<float>(worldCursorPos.y));
					}

					pVisualizer->UpdateTextFormat();
					result = 0;
					wasHandled = true;
					break;
				}
				case WM_KEYDOWN:
				{
					pVisualizer->HandleKeyStroke(wParam, lParam);
					result = 0;
					wasHandled = true;
					break;
				}
				case WM_LBUTTONDOWN:
				{

					std::cout << screenCursorPos.x << " " << screenCursorPos.y << std::endl;

					pVisualizer->HandleMouseClick(worldCursorPos);

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
					InvalidateRect(hwnd, NULL, FALSE);
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
			if (!wasHandled)
			{
				result = DefWindowProc(hwnd, message, wParam, lParam);
			}
		}
		return result;
	}

	void Visualizer::HandleKeyStroke(WPARAM wParam, LPARAM lParam)
	{
		if (HandleTextFieldKeyStroke(m_SelectedTextField, wParam, lParam)) return;
		if (HandleCameraMovementKeyStroke(wParam, 10.0f)) return;
	}

	bool Visualizer::HandleCameraMovementKeyStroke(WPARAM wParam, float distance)
	{
		switch (wParam)
		{
		case VK_LEFT:
			m_Camera->MoveLeft(10.0f);
			InvalidateRect(m_hwnd, NULL, FALSE);
			return true;
		case VK_RIGHT:
			m_Camera->MoveRight(10.0f);
			InvalidateRect(m_hwnd, NULL, FALSE);
			return true;
		case VK_UP:
			m_Camera->MoveUp(10.0f);
			InvalidateRect(m_hwnd, NULL, FALSE);
			return true;
		case VK_DOWN:
			m_Camera->MoveDown(10.0f);
			InvalidateRect(m_hwnd, NULL, FALSE);
			return true;
		}
		return false;
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


	void Visualizer::HandleMouseClick(const POINT& worldCursorPos)
	{

		//! Handle button clicks
		if (HandleButtonClick(worldCursorPos.x, worldCursorPos.y)) return;

		//! Handle text field selections
		if (!m_Initialized)
		{
			if (HandleTextFieldClick(m_TopologyTextField, m_SelectedTextField, worldCursorPos.x, worldCursorPos.y)) return;
			if (HandleTextFieldClick(m_StartingInputsTextField, m_SelectedTextField, worldCursorPos.x, worldCursorPos.y)) return;
			if (HandleTextFieldClick(m_TargetOutputsTextField, m_SelectedTextField, worldCursorPos.x, worldCursorPos.y)) return;
			if (HandleTextFieldClick(m_MaxEpochTextField, m_SelectedTextField, worldCursorPos.x, worldCursorPos.y)) return;
			if (HandleTextFieldClick(m_LayerExecutionTimeTextField, m_SelectedTextField, worldCursorPos.x, worldCursorPos.y)) return;
		}

		//! Handle node click
		if (HandleNodeClick(worldCursorPos.x, worldCursorPos.y)) return;

		//! If not handled by any component than reset text field
		m_SelectedTextField = nullptr;
	}

	bool Visualizer::HandleNodeClick(int mouseX, int mouseY)
	{
		if (!m_Initialized)
			return false;
		const auto& layers = m_NeuralNetwork->GetLayers();
		D2D1_POINT_2F worldCursorPos = m_Camera->GetCursorWorldPosition(D2D1::Point2F(mouseX, mouseY));
		for (const auto& layer : layers)
		{
			if (layer->GetSize() > m_LargestLayerSize)
			{
				m_LargestLayerSize = layer->GetSize();
			}
		}

		bool clickedOnAny = false;
		for (size_t layerIndex = 0; layerIndex < layers.size(); ++layerIndex)
		{
			const auto& layer = layers[layerIndex];
			int layerSize = layer->GetSize();
			float xOffset = 20.0f + layerIndex * m_HorizontalSpacing;
			float yStartingGap = ((float(m_LargestLayerSize - layerSize) / 2) * (m_VerticalSpacing));

			for (size_t neuronIndex = 0; neuronIndex < layerSize; ++neuronIndex)
			{
				float yOffset = yStartingGap + 20.0f + neuronIndex * m_VerticalSpacing;
				D2D1_POINT_2F neuronPosition = D2D1::Point2F(xOffset, yOffset);

				// Check if the click is within the radius of the node
				float distance = sqrt(pow(neuronPosition.x - worldCursorPos.x, 2) + pow(neuronPosition.y - worldCursorPos.y, 2));

				if (distance <= m_NodeRadius)
				{
					clickedOnAny = true;
					m_ChoosenNeuronColRow.first = layerIndex;
					m_ChoosenNeuronColRow.second = neuronIndex;
					InvalidateRect(m_hwnd, NULL, FALSE);
					return true;
				}
			}
		}
		if (!clickedOnAny)
		{
			m_ChoosenNeuronColRow.first = -1;
			m_ChoosenNeuronColRow.second = -1;
			InvalidateRect(m_hwnd, NULL, FALSE);
		}
	}

	bool Visualizer::HandleButtonClick(float cursorX, float cursorY)
	{
		if (m_Initialized)
		{
			if (m_StartButton->IsClicked(cursorX, cursorY))
			{
				if (m_LoopState == NNCore::LoopState::Stopped)
				{
					m_LoopState = NNCore::LoopState::Running;
					m_NeuralNetwork->ChangeLoopState(m_LoopState);
					m_TrainingThread = std::thread([&]() {
						m_NeuralNetwork->ChangeLoopState(NNCore::LoopState::Running);
						m_NeuralNetwork->Train();
						});
				}
				return true;
			}
			if (m_StepButton->IsClicked(cursorX, cursorY))
			{
				if (m_LoopState == NNCore::LoopState::Stopped)
				{
					m_LoopState = NNCore::LoopState::Stepping;
					m_TrainingThread = std::thread([&]() {
						m_NeuralNetwork->ChangeLoopState(NNCore::LoopState::Stepping);
						m_NeuralNetwork->Step();
						});
					m_LoopState = NNCore::LoopState::Stopped;
					m_NeuralNetwork->ChangeLoopState(m_LoopState);
				}
				return true;
			}
			if (m_StopButton->IsClicked(cursorX, cursorY))
			{
				m_LoopState = NNCore::LoopState::Stopped;
				m_NeuralNetwork->ChangeLoopState(m_LoopState);
				if (m_TrainingThread.joinable())
					m_TrainingThread.join();
				return true;
			}
		}
		else
		{
			if (m_ActivationFunctionDropdown->IsClicked(cursorX, cursorY))
			{
				int activationFunctionIndex = m_ActivationFunctionDropdown->GetChoosenIndex() + 1;
				NNCore::NeuronActivation::ActivationFunction activationFunction;
				m_ActivationFunction = static_cast<NNCore::NeuronActivation::ActivationFunction>(activationFunctionIndex);
				return true;
			}
			if (m_InitializeButton->IsClicked(cursorX, cursorY))
			{
				std::unique_ptr<NNCore::NeuralNetwork> myNN;
				try
				{
					NNCore::NeuralNetworkProperties properties{
						Utils::ValueParser::ParseWStringToIntVector(m_TopologyTextField->GetText()),
						Utils::ValueParser::ParseWStringToDoubleVector(m_StartingInputsTextField->GetText()),
						Utils::ValueParser::ParseWStringToDoubleVector(m_TargetOutputsTextField->GetText()),
						Utils::ValueParser::ParseWStringToInt(m_MaxEpochTextField->GetText()),
						m_ActivationFunction
					};
					myNN = std::make_unique<NNCore::NeuralNetwork>(properties);
					this->SetNN(std::move(myNN));
				}
				catch (std::invalid_argument exception)
				{
					m_ExceptionMessage = m_WStringConverter.from_bytes(exception.what());
					return true;
				}
				m_Initialized = true;
			}
		}
		return false;
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

		if (SUCCEEDED(hr))
		{
			m_TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			m_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		}
	}
	void Visualizer::OnResize(UINT width, UINT height)
	{
		if (m_RenderTarget)
		{
			// Note: This method can fail, but it's okay to ignore the
			// error here, because the error will be returned again
			// the next time EndDraw is called.
			m_RenderTarget->Resize(D2D1::SizeU(width, height));
			m_ViewportWidth = width;
			m_ViewportHeight = height;
		}
	}
	void Visualizer::DiscardDeviceResources()
	{
		SafeRelease(&m_RenderTarget);
		SafeRelease(&m_DWriteFactory);
		SafeRelease(&m_TextFormat);
		SafeRelease(&m_MenuTextFormat);
		SafeRelease(&m_ErrorTextFormat);
		SafeRelease(&m_LooseWeightBrush);
		SafeRelease(&m_MediumWeightBrush);
		SafeRelease(&m_TightWeightBrush);
		SafeRelease(&m_GrayBrush);
		SafeRelease(&m_BlackBrush);
		SafeRelease(&m_WhiteBrush);
		SafeRelease(&m_LimeGreenBrush);

	}
}
