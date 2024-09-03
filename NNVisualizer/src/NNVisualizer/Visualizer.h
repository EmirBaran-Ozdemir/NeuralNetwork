#pragma once
#include "nnvpch.h"
#include "Core/NeuralNetwork.h"
#include "Renderer/Camera.h"
namespace NNVisualizer {

	class Visualizer
	{
	public:
		Visualizer();
		~Visualizer();
		HRESULT Initialize();
		void RunMessageLoop();
		void SetNN(std::unique_ptr<NNCore::NeuralNetwork> neuralNetwork);

	private:
		HRESULT CreateDeviceIndependentResources();
		HRESULT CreateDeviceResources();
		void DiscardDeviceResources();
		HRESULT OnRender();
		void OnResize(
			UINT width,
			UINT height
		);
		void SetZoomFactor(float zoomFactor);
		static LRESULT CALLBACK WndProc(
			HWND hWnd,
			UINT message,
			WPARAM wParam,
			LPARAM lParam
		);
		void DrawNode(D2D1_POINT_2F position, float radius, double value);
		void MoveCameraLeft(float distance);
		void MoveCameraRight(float distance);
		void MoveCameraUp(float distance);
		void MoveCameraDown(float distance);
		void DrawWeight(D2D1_POINT_2F start, D2D1_POINT_2F end, float weight);
		void LoopNN();
		void ZoomOut();
		void ZoomIn();

		static D2D1_MATRIX_3X2_F Matrix4x4ToMatrix3x2(const D2D1_MATRIX_4X4_F& m4x4) {
			return D2D1::Matrix3x2F(
				m4x4._11, m4x4._12,
				m4x4._21, m4x4._22,
				m4x4._41, m4x4._42
			);
		}
	private:
		HWND m_hwnd;
		ID2D1Factory* m_Direct2dFactory;
		IDWriteFactory* m_DWriteFactory;
		ID2D1HwndRenderTarget* m_RenderTarget;
		IDWriteTextFormat* m_TextFormat;
		ID2D1SolidColorBrush* m_LooseWeightBrush;
		ID2D1SolidColorBrush* m_MediumWeightBrush;
		ID2D1SolidColorBrush* m_TightWeightBrush;
		Renderer::Camera* m_Camera;
		int m_ViewportHeight;
		int m_ViewportWidth;
		float m_zoomFactor = 1.0f;
		//! Render Data
		std::unique_ptr<NNCore::NeuralNetwork> m_NeuralNetwork;
	};
}
