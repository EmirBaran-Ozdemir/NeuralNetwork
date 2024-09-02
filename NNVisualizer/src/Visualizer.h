#pragma once
#include "nnvpch.h"
#include "Core/NeuralNetwork.h"

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
		static LRESULT CALLBACK WndProc(
			HWND hWnd,
			UINT message,
			WPARAM wParam,
			LPARAM lParam
		);
		void DrawNode(D2D1_POINT_2F position, float radius, double value);
		void DrawWeight(D2D1_POINT_2F start, D2D1_POINT_2F end, float weight);
		void LoopNN();
	private:
		HWND m_hwnd;
		ID2D1Factory* m_Direct2dFactory;
		IDWriteFactory* m_DWriteFactory;
		ID2D1HwndRenderTarget* m_RenderTarget;
		IDWriteTextFormat* m_TextFormat;
		ID2D1SolidColorBrush* m_LooseWeightBrush;
		ID2D1SolidColorBrush* m_MediumWeightBrush;
		ID2D1SolidColorBrush* m_TightWeightBrush;
		//! Render Data
		std::unique_ptr<NNCore::NeuralNetwork> m_NeuralNetwork;
	};
}
