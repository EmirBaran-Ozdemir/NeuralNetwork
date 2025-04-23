#include "Button.h"

namespace Components {

	void Button::Draw()
	{
		m_DrawProperties->RenderTarget->DrawRectangle(m_Rect, m_DrawProperties->Brush);
		m_DrawProperties->RenderTarget->DrawText(m_Label.c_str(), static_cast<UINT32>(m_Label.size()), m_DrawProperties->TextFormat, m_Rect, m_DrawProperties->TextBrush);
	}

	void Button::SetFunction(std::function<void()> onClick)
	{
		m_OnClick = onClick;
	}

	bool Button::OnKeyStroke(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case VK_RETURN:
			m_OnClick();
			return true;
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