#include "nnvpch.h"

namespace Components {

	class TextField
	{
	public:
		TextField(const std::wstring& placeholder);
		void Draw(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush, ID2D1SolidColorBrush* textBrush, IDWriteTextFormat* textFormat, float x, float y, float width, float height, bool isChoosen = false);
		void Draw(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush, ID2D1SolidColorBrush* textBrush, IDWriteTextFormat* textFormat, D2D1_RECT_F* rectangle, bool isChoosen = false);
		bool KeyStroke(WPARAM wParam, LPARAM lParam);
		bool IsClicked(float mouseX, float mouseY) const;
		std::wstring GetText() const { return m_TextInput; }
	private:
		D2D1_RECT_F m_Rect{ 0.0f,0.0f,0.0f,0.0f };
		std::wstring m_TextInput = L"";
		std::wstring m_Placeholder = L"Placeholder";
	};
}