#pragma once
#include "nnvpch.h"

namespace NNVisualizer {

	class Visualizer
	{
	public:
		Visualizer();
		~Visualizer();

		// Register the window class and call methods for instantiating drawing resources
		HRESULT Initialize();

		// Process and dispatch messages
		void RunMessageLoop();

	private:
		// Initialize device-independent resources.
		HRESULT CreateDeviceIndependentResources();

		// Initialize device-dependent resources.
		HRESULT CreateDeviceResources();

		// Release device-dependent resource.
		void DiscardDeviceResources();

		// Draw content.
		HRESULT OnRender();

		// Resize the render target.
		void OnResize(
			UINT width,
			UINT height
		);

		// The windows procedure.
		static LRESULT CALLBACK WndProc(
			HWND hWnd,
			UINT message,
			WPARAM wParam,
			LPARAM lParam
		);
	private:
		HWND m_hwnd;
		ID2D1Factory* m_pDirect2dFactory;
		ID2D1HwndRenderTarget* m_pRenderTarget;
		ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
		ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
	};
}
