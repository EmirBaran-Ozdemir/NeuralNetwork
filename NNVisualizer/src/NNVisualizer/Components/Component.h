#pragma once
#include <d2d1.h>
#include <string>
#include <objbase.h>
#include <sstream>
#include <iomanip>

namespace Components {

	class Component {
	public:
		virtual ~Component() = default;

		void SetBounds(const D2D1_RECT_F& rect) { m_Rect = rect; }

		void SetBounds(float x, float y, float width, float height) {
			m_Rect = D2D1::RectF(x, y, x + width, y + height);
		}

		bool IsClickInBounds(float mouseX, float mouseY) const {
			if (!IsVisible()) return false;
			return mouseX >= m_Rect.left && mouseX <= m_Rect.right &&
				mouseY >= m_Rect.top && mouseY <= m_Rect.bottom;
		}

		void SetVisibility(bool isVisible) { m_Visible = isVisible; }
		bool IsVisible() const { return m_Visible; }

		virtual bool OnClick(float mouseX, float mouseY) {
			if (!IsVisible()) return false;
			if (!IsClickInBounds(mouseX, mouseY)) return false;
			return true;
		}

		void SetIndex(int index) { m_Index = index; }
		int GetIndex() const { return m_Index; }
		GUID GetGuid() const { return m_Id; }

		std::string GetGuidAsString() const {
			return GuidToString(m_Id);
		}

		virtual void Draw(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush,
			ID2D1SolidColorBrush* textBrush, IDWriteTextFormat* textFormat) = 0;

		virtual bool OnKeyStroke(UINT message, WPARAM wParam, LPARAM lParam) = 0;

	protected:
		Component() {
			CoCreateGuid(&m_Id);
			std::cout << "Guid generated succesfully ";
			m_GuidString = GuidToString(m_Id);
			std::cout << m_GuidString << std::endl;
		}

	protected:
		D2D1_RECT_F m_Rect{ 0.0f, 0.0f, 0.0f, 0.0f };
		bool m_Visible = false;
		int m_Index = 0;
		GUID m_Id;
		std::string m_GuidString;

	private:
		inline std::string GuidToString(const GUID& guid) const {
			std::ostringstream oss;
			oss << std::hex << std::setfill('0') << std::nouppercase
				<< std::setw(8) << guid.Data1 << '-'
				<< std::setw(4) << guid.Data2 << '-'
				<< std::setw(4) << guid.Data3 << '-'
				<< std::setw(2) << static_cast<unsigned>(guid.Data4[0])
				<< std::setw(2) << static_cast<unsigned>(guid.Data4[1]) << '-';

			for (int i = 2; i < 8; ++i)
				oss << std::setw(2) << static_cast<unsigned>(guid.Data4[i]);

			return oss.str();
		}
	};
}
