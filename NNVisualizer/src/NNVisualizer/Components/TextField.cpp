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
    bool TextField::KeyStroke(WPARAM wParam, LPARAM lParam)
    {
        BYTE keyboardState[256];
        bool result = GetKeyboardState(keyboardState);
        if (!result) {
            return false;
        }
        wchar_t translatedChar[2] = { 0 };
        int unicodeResult= ToUnicode(wParam, (lParam >> 16) & 0xFF, keyboardState, translatedChar, 2, 0);

        if(unicodeResult == 1) // A single character was translated
        {
            // Handle backspace
            if(wParam == VK_BACK)
            {
                if(!m_TextInput.empty())
                {
                    m_TextInput.pop_back();
                }
                return true;
            }
            // Add the translated character to the input
            else if((translatedChar[0] >= L'0' && translatedChar[0] <= L'9') ||
                (translatedChar[0] >= L'A' && translatedChar[0] <= L'Z') ||
                (translatedChar[0] >= L'a' && translatedChar[0] <= L'z') ||
                translatedChar[0] == L' ' || // space
                translatedChar[0] == L',' || // comma
                translatedChar[0] == L'.')   // period
            {
                m_TextInput += translatedChar[0];
                return true;
            }
        }
        return false;
    }


	bool TextField::IsClicked(float mouseX, float mouseY) const
	{
		return mouseX >= m_Rect.left && mouseX <= m_Rect.right &&
			mouseY >= m_Rect.top && mouseY <= m_Rect.bottom;
	}
}
