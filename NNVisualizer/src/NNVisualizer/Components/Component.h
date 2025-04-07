#pragma once
#include <d2d1.h>
#include <string>

namespace Components {

	class Component {
	public:
		virtual ~Component() = default;

		void SetBounds(const D2D1_RECT_F& rect) { m_Rect = rect; }

		void SetBounds(float x, float y, float width, float height) {
			m_Rect = D2D1::RectF(x, y, x + width, y + height);
		}

		bool IsClickInBounds(float mouseX, float mouseY) const
		{
			return mouseX >= m_Rect.left && mouseX <= m_Rect.right && mouseY >= m_Rect.top && mouseY <= m_Rect.bottom;
		}


		void SetVisible(bool isVisible) { m_Visible = isVisible; }
		bool IsVisible() const { return m_Visible; }

		virtual bool OnClick(float mouseX, float mouseY)
		{
			if (!IsVisible()) return false;
			if (!IsClickInBounds(mouseX, mouseY)) return false;
			return true;
		}

		virtual void Draw(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush, ID2D1SolidColorBrush* textBrush, IDWriteTextFormat* textFormat) = 0;
		virtual bool OnKeyStroke(UINT message, WPARAM wParam, LPARAM lParam) = 0;
	protected:
		Component() = default;
	protected:
		D2D1_RECT_F m_Rect{ 0.0f, 0.0f, 0.0f, 0.0f };
		bool m_Visible = false;
	};
}