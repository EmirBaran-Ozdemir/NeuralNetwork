#include "TextField.h"

namespace Components {

	void TextField::Draw()
	{
		if (!IsDrawPropertiesSet()) return;
		m_DrawProperties->RenderTarget->DrawRectangle(m_Rect, m_DrawProperties->Brush);
		const std::wstring& textToDraw = m_TextInput.empty() ? m_Label : m_TextInput;
		m_DrawProperties->RenderTarget->DrawText(textToDraw.c_str(), static_cast<UINT32>(textToDraw.size()), m_DrawProperties->TextFormat, m_Rect, m_DrawProperties->TextBrush);
	}

	bool TextField::OnKeyStroke(UINT message, WPARAM wParam, LPARAM lParam)
	{
		BYTE keyboardState[256];
		bool result = GetKeyboardState(keyboardState);
		if (!result) {
			return false;
		}
		wchar_t translatedChar[2] = { 0 };
		int unicodeResult = ToUnicode(wParam, (lParam >> 16) & 0xFF, keyboardState, translatedChar, 2, 0);

		if (unicodeResult == 1) // A single character was translated
		{
			// Handle backspace
			if (wParam == VK_BACK)
			{
				if (!m_TextInput.empty())
				{
					m_TextInput.pop_back();
				}
				return true;
			}
			else if (wParam == VK_DELETE && !m_TextInput.empty()) {
				m_TextInput.clear(); // Clear all text (or add specific logic)
				return true;
			}
			// Add the translated character to the input
			else if ((translatedChar[0] >= L'0' && translatedChar[0] <= L'9') ||
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

	bool TextField::OnClick(float mouseX, float mouseY) {
		if (Component::OnClick(mouseX, mouseY)) {
			return true;
		}
		return false;
	}
}
