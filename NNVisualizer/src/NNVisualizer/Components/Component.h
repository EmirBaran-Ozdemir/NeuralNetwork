#pragma once
#include <d2d1.h>
#include <string>
#include <objbase.h>
#include <sstream>
#include <iomanip>
#include "nnvpch.h"

namespace Components {

	struct DrawProperties
	{
		ID2D1HwndRenderTarget* RenderTarget = nullptr;
		ID2D1SolidColorBrush* Brush = nullptr;
		ID2D1SolidColorBrush* TextBrush = nullptr;
		IDWriteTextFormat* TextFormat = nullptr;
	};
	class Component {
	public:
		virtual ~Component() = default;
		Component(const Component&) = delete;
		Component& operator=(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator=(Component&&) = delete;

		std::wstring GetLabel() const { return m_Label; }
		void SetBounds(const D2D1_RECT_F& rect) {
			m_Rect = rect;
			OnBoundsSet();
		}

		void SetBounds(float x, float y, float width, float height) {
			m_Rect = D2D1::RectF(x, y, x + width, y + height);
			OnBoundsSet();
		}

		bool IsBoundsSet() const {
			return !(m_Rect.left == -1.0f && m_Rect.top == -1.0f && m_Rect.right == -1.0f && m_Rect.bottom == -1.0f);
		}

		D2D_RECT_F GetBounds() const { return m_Rect; }

		bool IsClickInBounds(float mouseX, float mouseY) const {
			return IsBoundsSet() && IsVisible() &&
				mouseX >= m_Rect.left && mouseX <= m_Rect.right &&
				mouseY >= m_Rect.top && mouseY <= m_Rect.bottom;
		}


		void SetVisibility(bool isVisible) { m_Visible = isVisible; }
		bool IsVisible() const { return m_Visible; }

		virtual bool OnClick(float mouseX, float mouseY) {
			return IsVisible() && IsClickInBounds(mouseX, mouseY);
		}

		void SetIndex(int index) { m_Index = index; }
		int GetIndex() const { return m_Index; }
		GUID GetGuid() const { return m_Id; }

		std::string GetGuidAsString() const {
			return m_GuidString;
		}

		void AddUpdateDrawProperties(ID2D1HwndRenderTarget* renderTarget,
			ID2D1SolidColorBrush* brush,
			ID2D1SolidColorBrush* textBrush,
			IDWriteTextFormat* textFormat) {

			if (!m_DrawProperties) {
				m_DrawProperties = new DrawProperties{
					renderTarget, brush, textBrush, textFormat
				};
			}
			else {
				m_DrawProperties->RenderTarget = renderTarget;
				m_DrawProperties->Brush = brush;
				m_DrawProperties->TextBrush = textBrush;
				m_DrawProperties->TextFormat = textFormat;
			}
		}

		void AddUpdateDrawProperties(DrawProperties* drawProperties) {
			m_DrawProperties = drawProperties;
		}

		bool IsDrawPropertiesSet() const { return m_DrawProperties != nullptr; }
		void SafeDraw() {
			if (!IsDrawPropertiesSet())
				return;
			Draw();
		}
		virtual bool OnKeyStroke(UINT message, WPARAM wParam, LPARAM lParam) = 0;
	protected:

		Component(const std::wstring& label) : m_Label(label) { Initialize(); }

		Component(const std::wstring& label, DrawProperties* drawProps, bool visible)
			: m_Label(label), m_Visible(visible)
		{
			static DrawProperties tempProps;
			tempProps.RenderTarget = drawProps->RenderTarget;
			tempProps.Brush = drawProps->Brush;
			tempProps.TextBrush = drawProps->TextBrush;
			tempProps.TextFormat = drawProps->TextFormat;
			m_DrawProperties = &tempProps;
			Initialize();
		}

		Component() = delete;
		virtual void OnBoundsSet() {};
		virtual void Draw() = 0;

	protected:
		D2D1_RECT_F m_Rect{ -1.0f, -1.0f, -1.0f, -1.0f };
		bool m_Visible = false;
		int m_Index = 0;
		GUID m_Id{};
		std::string m_GuidString;
		DrawProperties* m_DrawProperties = nullptr;
		std::wstring m_Label = L"";
	private:
		void Initialize() {
			if (FAILED(CoCreateGuid(&m_Id))) {
				throw std::runtime_error("Failed to create GUID");
			}
			m_GuidString = GuidToString(m_Id);
#ifdef _DEBUG
			std::cout << "Guid generated successfully: " << m_GuidString << std::endl;
#endif
		}

		std::string GuidToString(const GUID& guid) const {
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
