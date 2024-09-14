#pragma once
#include "nnvpch.h"

namespace Components {
	class Dropdown {
	public:
		Dropdown(const std::wstring& menuName, const std::vector<std::wstring>& elements);
		void Draw(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, float x, float y, float width, float height);
		bool IsClicked(float mouseX, float mouseY);
		int GetChoosenIndex() const { return m_ChoosenElementIndex; }
	private:
		void DrawElements(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, float width, float height);
	private:
		D2D1_RECT_F m_Rect{ 0.0f,0.0f,0.0f,0.0f };
		std::wstring m_MenuName = L"";
		std::vector<std::wstring> m_Elements;
		bool m_IsOpen = false;
		int m_ChoosenElementIndex = -1;
	};
}