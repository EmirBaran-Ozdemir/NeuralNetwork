#include "Button.h"

namespace Components {

	void Button::Draw(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush, ID2D1SolidColorBrush* textBrush, IDWriteTextFormat* textFormat)
	{
		renderTarget->DrawRectangle(m_Rect, brush);
		renderTarget->DrawText(m_Text.c_str(), static_cast<UINT32>(m_Text.size()), textFormat, m_Rect, textBrush);
	}

	void Button::SetOnClick(std::function<bool()> onClick)
	{
		m_OnClick = onClick;
	}

	bool Button::OnKeyStroke(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case VK_RETURN:
			return m_OnClick();
		}
		return false;
	}

	bool Button::OnClick(float mouseX, float mouseY)
	{
		if (Component::OnClick(mouseX, mouseY)) {
			if (m_OnClick) {
				m_OnClick();
			}
			return true;
		}
		return false;
	}
}