#pragma once
#include "nnvpch.h"
#include "Core/NeuralNetwork.h"
#include "Renderer/Camera.h"
#include "Components/Components.h"

namespace NNVisualizer {

	class Visualizer
	{
	public:
		Visualizer();
		~Visualizer();
		HRESULT Initialize();
		void SetNN(std::unique_ptr<NNCore::NeuralNetwork> neuralNetwork);
		void RunMessageLoop();

	private:
		HRESULT CreateDeviceIndependentResources();
		HRESULT CreateDeviceResources();
		void DiscardDeviceResources();
		HRESULT OnRender();
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		void LoopNN();
		void DrawNode(D2D1_POINT_2F position, float radius, double value, bool isChoosen);
		void DrawWeight(D2D1_POINT_2F start, D2D1_POINT_2F end, float weight, bool isConnectedToChoosen);
		bool HandleCameraMovementKeyStroke(WPARAM wParam, float distance);
		void ZoomIn(float cursorX, float cursorY);
		void ZoomOut(float cursorX, float cursorY);
		void HandleKeyStroke(WPARAM wParam, LPARAM lParam);
		void HandleMouseClick(const POINT& worldCursorPos);
		bool HandleNodeClick(int mouseX, int mouseY);
		bool HandleButtonClick(float cursorX, float cursorY);
		void OnResize(UINT width, UINT height);
		void UpdateTextFormat();


		static D2D1_MATRIX_3X2_F Matrix4x4ToMatrix3x2(const D2D1_MATRIX_4X4_F& m4x4) {
			return D2D1::Matrix3x2F(
				m4x4._11, m4x4._12,
				m4x4._21, m4x4._22,
				m4x4._41, m4x4._42
			);
		}

		static bool HandleTextFieldClick(std::shared_ptr<Components::TextField>& textField, std::shared_ptr<Components::TextField>& selectedTextField, float cursorX, float cursorY)
		{
			if(textField->IsClicked(cursorX, cursorY))
			{
				selectedTextField = textField;
				return true;
			}
			return false;
		}

		static void DrawTextField(std::shared_ptr<Components::TextField>& textField,
			std::shared_ptr<Components::TextField>& selectedTextField,
			ID2D1HwndRenderTarget* renderTarget,
			ID2D1SolidColorBrush* normalBrush,
			ID2D1SolidColorBrush* selectedBrush,
			IDWriteTextFormat* textFormat,
			float x, float y, float width, float height)
		{
			bool isSelected = textField.get() == selectedTextField.get();
			textField->Draw(renderTarget, isSelected ? selectedBrush : normalBrush, textFormat, x, y, width, height, isSelected);
		}

		static bool HandleTextFieldKeyStroke(std::shared_ptr<Components::TextField>& selectedTextField, WPARAM wParam, LPARAM lParam)
		{
			if(selectedTextField)
				return selectedTextField->KeyStroke(wParam, lParam);
			return false;
		}

	private:
		HWND m_hwnd;
		ID2D1Factory* m_Direct2dFactory;
		IDWriteFactory* m_DWriteFactory;
		ID2D1HwndRenderTarget* m_RenderTarget;
		IDWriteTextFormat* m_TextFormat;
		IDWriteTextFormat* m_MenuTextFormat;
		IDWriteTextFormat* m_ErrorTextFormat;
		ID2D1SolidColorBrush* m_LooseWeightBrush;
		ID2D1SolidColorBrush* m_MediumWeightBrush;
		ID2D1SolidColorBrush* m_TightWeightBrush;
		ID2D1SolidColorBrush* m_GrayBrush;
		ID2D1SolidColorBrush* m_BlackBrush;
		ID2D1SolidColorBrush* m_LimeGreenBrush;

		Renderer::Camera* m_Camera;

		//! Runtime data
		bool m_Initialized = false;
		std::unique_ptr<NNCore::NeuralNetwork> m_NeuralNetwork;
		std::wstring m_ExceptionMessage = L"";
		int m_ViewportHeight;
		int m_ViewportWidth;
		float m_MinTextSize = 2.0f;
		bool m_AnyNeuronChoosed = false;
		NNCore::Neuron* m_ChoosenNeuron = nullptr;

		float m_VerticalSpacing;
		float m_HorizontalSpacing;
		float m_NodeRadius = 10.0f;
		int m_LargestLayerSize = 0;

		Utils::LoopState m_LoopState = Utils::LoopState::Stopped;

		//! Components
		std::unique_ptr<Components::Button> m_StartButton;
		std::unique_ptr<Components::Button> m_StepButton;
		std::unique_ptr<Components::Button> m_StopButton;
		std::unique_ptr<Components::Button> m_InitializeButton;

		std::shared_ptr<Components::TextField> m_TopologyTextField;
		std::shared_ptr<Components::TextField> m_StartingInputsTextField;
		std::shared_ptr<Components::TextField> m_TargetOutputsTextField;
		std::shared_ptr<Components::TextField> m_SelectedTextField;


	};
}
