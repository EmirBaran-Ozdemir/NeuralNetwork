#include "Button.h"

namespace Components {
	Button::Button(const std::wstring& text)
		:m_Text(text)
	{
	}
	void Button::Draw(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, float x, float y, float width, float height)
	{
		m_Rect = D2D1::RectF(x, y, x + width, y + height);
		renderTarget->DrawRectangle(m_Rect, brush);
		renderTarget->DrawTextA(m_Text.c_str(), static_cast<UINT32>(m_Text.size()), textFormat, m_Rect, brush);
	}
	bool Button::IsClicked(float mouseX, float mouseY) const
	{
		return mouseX >= m_Rect.left && mouseX <= m_Rect.right &&
			mouseY >= m_Rect.top && mouseY <= m_Rect.bottom;
		return false;
	}
}