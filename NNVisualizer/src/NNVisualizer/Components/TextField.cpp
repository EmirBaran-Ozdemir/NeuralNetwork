#include "TextField.h"

namespace Components {
	TextField::TextField(const std::wstring& placeholder) :m_Placeholder(placeholder)
	{
	}
	void TextField::Draw(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, float x, float y, float width, float height, bool isChoosen)
	{
		m_Rect = D2D1::RectF(x, y, x + width, y + height);
        if(isChoosen)
        {
            renderTarget->FillRectangle(m_Rect, brush);
        }
        else
        {
            renderTarget->DrawRectangle(m_Rect, brush);
        }
        const std::wstring& textToDraw = m_TextInput.empty() ? m_Placeholder : m_TextInput;
        renderTarget->DrawText(textToDraw.c_str(), static_cast<UINT32>(textToDraw.size()), textFormat, m_Rect, brush);
	}
    void TextField::KeyStroke(WPARAM wParam)
    {
        if(wParam == VK_BACK)
        {
            if(!m_TextInput.empty())
            {
                m_TextInput.pop_back();  
            }
        }
        else if((wParam >= '0' && wParam <= '9') ||
            (wParam >= 'A' && wParam <= 'Z') ||
            (wParam >= 'a' && wParam <= 'z') ||
            wParam == VK_SPACE ||
            wParam == VK_OEM_PERIOD ||
            wParam == VK_OEM_COMMA ||
            wParam == VK_OEM_MINUS ||
            wParam == VK_OEM_PLUS) 
        {
            m_TextInput += static_cast<wchar_t>(wParam);
        }
    }

	bool TextField::IsClicked(float mouseX, float mouseY) const
	{
		return mouseX >= m_Rect.left && mouseX <= m_Rect.right &&
			mouseY >= m_Rect.top && mouseY <= m_Rect.bottom;
	}
}
