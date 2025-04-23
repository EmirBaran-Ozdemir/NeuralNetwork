#pragma once
#include "nnvpch.h"
#include "Core/NeuralNetwork.h"
#include "Core/LoopState.h"
#include "Renderer/Camera.h"
#include "Components/ComponentFactory.h"
#include "Events/EventHandler.h"
#include "Components/DropdownBuilder.h"
#include "Components/ButtonBuilder.h"
#include "Components/TextFieldBuilder.h"

namespace NNVisualizer {

	class Visualizer
	{
	public:
		Visualizer();
		~Visualizer();
		HRESULT Initialize();
		void SetNN(std::unique_ptr<NNCore::NeuralNetwork> neuralNetwork);
		void RunMessageLoop();
		std::shared_ptr<Components::Component> GetFocusedComponent() const { return m_FocusedComponent; }

	private:
		HRESULT CreateDeviceIndependentResources();
		HRESULT CreateDeviceResources();
		void DiscardDeviceResources();
		HRESULT OnRender();
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		void LoopNN();
		void DrawProperties();
		void DrawProperty(const std::wstring& text, D2D1_RECT_F& layoutRect, bool slideRect = false, bool drawBorder = false);
		void DrawNode(D2D1_POINT_2F position, float radius, double value, bool isChoosen, bool isProcessing);
		void DrawWeight(D2D1_POINT_2F start, D2D1_POINT_2F end, float weight, bool isConnectedToChoosen);
		bool HandleCameraMovementKeyStroke(WPARAM wParam, float distance);
		void ZoomIn(float cursorX, float cursorY);
		void ZoomOut(float cursorX, float cursorY);
		void OnResize(UINT width, UINT height);

		bool HandleMouseClick(const POINT& worldCursorPos);
		void SetFocusedComponent(const std::shared_ptr<Components::Component>& component) { m_FocusedComponent = component; }

		void UpdateTextFormat();
		Events::EventHandler* GetEventHandler() { return &m_EventHandler; }
		static D2D1_MATRIX_3X2_F Matrix4x4ToMatrix3x2(const D2D1_MATRIX_4X4_F& m4x4) {
			return D2D1::Matrix3x2F(
				m4x4._11, m4x4._12,
				m4x4._21, m4x4._22,
				m4x4._41, m4x4._42
			);
		}


		void DrawTextField(std::shared_ptr<Components::TextField>& textField, float x, float y, float width, float height) const;
		void DrawTextField(std::shared_ptr<Components::TextField>& textField, D2D1_RECT_F* rectangle) const;

		//! Component Functions
		void InitializeButtonFunc();

		void ActivationFunctionFunc();

	private:
		HWND m_hwnd;
		ID2D1Factory* m_Direct2dFactory;
		IDWriteFactory* m_DWriteFactory;
		ID2D1HwndRenderTarget* m_RenderTarget;

		////! Text Formats
		//IDWriteTextFormat* m_TextFormat;
		//IDWriteTextFormat* m_MenuTextFormat;
		//IDWriteTextFormat* m_ErrorTextFormat;

		////! Brushes
		//ID2D1SolidColorBrush* m_LooseWeightBrush;
		//ID2D1SolidColorBrush* m_MediumWeightBrush;
		//ID2D1SolidColorBrush* m_TightWeightBrush;
		//ID2D1SolidColorBrush* m_GrayBrush;
		//ID2D1SolidColorBrush* m_BlackBrush;
		//ID2D1SolidColorBrush* m_LimeGreenBrush;
		//ID2D1SolidColorBrush* m_WhiteBrush;

		//! Builders
		Components::DropdownBuilder m_DropdownBuilder;
		Components::ButtonBuilder m_ButtonBuilder;
		Components::TextFieldBuilder m_TextFieldBuilder;

		Renderer::Camera* m_Camera;

		//! Runtime data
		bool m_NNInitialized = false;
		std::wstring m_ExceptionMessage = L"";

		int m_ViewportHeight;
		int m_ViewportWidth;
		float m_MinTextSize = 2.0f;
		float m_VerticalSpacing;
		float m_HorizontalSpacing;
		float m_NodeRadius = 10.0f;
		int m_LargestLayerSize = 0;

		//! NN Properties
		std::unique_ptr<NNCore::NeuralNetwork> m_NeuralNetwork;
		std::pair<int, int> m_ChoosenNeuronColRow{ -1,-1 };
		NNCore::NeuronActivation::ActivationFunction m_ActivationFunction;
		//! Components
		std::vector<std::shared_ptr<Components::Component>> m_ComponentList;
		std::shared_ptr<Components::Component> m_FocusedComponent;
		Events::EventHandler m_EventHandler;

		std::shared_ptr<Components::Button> m_StartButton;
		std::shared_ptr<Components::Button> m_StepButton;
		std::shared_ptr<Components::Button> m_StopButton;
		std::shared_ptr<Components::Button> m_InitializeButton;

		std::shared_ptr<Components::Dropdown> m_ActivationFunctionDropdown;

		std::shared_ptr<Components::TextField> m_TopologyTextField;
		std::shared_ptr<Components::TextField> m_StartingInputsTextField;
		std::shared_ptr<Components::TextField> m_TargetOutputsTextField;
		std::shared_ptr<Components::TextField> m_MaxEpochTextField;
		std::shared_ptr<Components::TextField> m_LayerExecutionTimeTextField;

		//! Multi-Threading
		std::future<void> m_TrainingFuture;
		std::thread m_TrainingThread;
	};
}
