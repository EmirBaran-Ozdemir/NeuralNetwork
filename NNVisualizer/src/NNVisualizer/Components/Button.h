#include "nnvpch.h"

namespace Components {

	class Button
	{
	public:
		Button(const std::wstring& text);
		void Draw(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, float x, float y, float width, float height);
		bool IsClicked(float mouseX, float mouseY) const;
	private:
		D2D1_RECT_F m_Rect;
		std::wstring m_Text;
	};
}