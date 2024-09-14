#pragma once
#include "Dropdown.h"

namespace Components {
	Dropdown::Dropdown(const std::wstring& menuName, const std::vector<std::wstring>& elements) : m_MenuName(menuName), m_Elements(elements)
	{
	}
	void Dropdown::Draw(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, float x, float y, float width, float height)
	{
		m_Rect = D2D1::RectF(x, y, x + width, y + height);
		renderTarget->DrawRectangle(m_Rect, brush);
		const std::wstring& textToDraw = m_ChoosenElementIndex == -1 ? m_MenuName : m_Elements[m_ChoosenElementIndex];
		renderTarget->DrawText(textToDraw.c_str(), static_cast<UINT32>(textToDraw.size()), textFormat, m_Rect, brush);

		if(m_IsOpen)
		{
			this->DrawElements(renderTarget, brush, textFormat, width, height);
		}
	}
	void Dropdown::DrawElements(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat, float width, float height)
	{
		D2D1_RECT_F elementRect = D2D1::RectF(m_Rect.left, m_Rect.top, m_Rect.right, m_Rect.bottom);
		for(int elementIndex = 0; elementIndex < m_Elements.size(); ++elementIndex)
		{
			// Move every element lower
			elementRect.bottom += height;
			elementRect.top += height;
			renderTarget->DrawRectangle(elementRect, brush);
			const std::wstring& elementString = m_Elements[elementIndex];
			renderTarget->DrawText(elementString.c_str(), static_cast<UINT32>(elementString.size()), textFormat, elementRect, brush);
		}
	}
	bool Dropdown::IsClicked(float mouseX, float mouseY)
	{
		if(mouseX >= m_Rect.left && mouseX <= m_Rect.right && mouseY >= m_Rect.top && mouseY <= m_Rect.bottom)
		{
			m_IsOpen = !m_IsOpen;
			return true;
		}

		if(m_IsOpen)
		{
			float elementHeight = m_Rect.bottom - m_Rect.top;
			D2D1_RECT_F elementRect = D2D1::RectF(m_Rect.left, m_Rect.top, m_Rect.right, m_Rect.bottom);

			for(int elementIndex = 0; elementIndex < m_Elements.size(); ++elementIndex)
			{
				elementRect.top = m_Rect.bottom + (elementIndex * elementHeight);
				elementRect.bottom = elementRect.top + elementHeight;

				if(mouseX >= elementRect.left && mouseX <= elementRect.right &&
					mouseY >= elementRect.top && mouseY <= elementRect.bottom)
				{
					m_ChoosenElementIndex = elementIndex;
					return true;
				}
			}
		}
		m_IsOpen = false;
		return false;  // No click detected
	}
}