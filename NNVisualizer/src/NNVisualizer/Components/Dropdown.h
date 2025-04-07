#pragma once
#include "nnvpch.h"
#include "Component.h"

namespace Components {
	class Dropdown : public Component {
	public:
		Dropdown(const std::wstring& menuName, const std::vector<std::wstring>& elements) : m_MenuName(menuName), m_Elements(elements), m_ChoosenElementIndex(-1), m_IsOpen(false) {}

		void Draw(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush, ID2D1SolidColorBrush* textBrush, IDWriteTextFormat* textFormat) override;
		bool OnClick(float mouseX, float mouseY) override;
		bool OnKeyStroke(UINT message, WPARAM wParam, LPARAM lParam) override;
		int GetChoosenIndex() const { return m_ChoosenElementIndex; }

	private:
		void DrawElements(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat);

	private:
		std::wstring m_MenuName = L"";
		std::vector<std::wstring> m_Elements;
		bool m_IsOpen = false;
		int m_ChoosenElementIndex;
		std::function<void(int)> m_OnSelect;

	};
}