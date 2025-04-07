#pragma once
#include "Dropdown.h"

namespace Components {

	void Dropdown::Draw(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush, ID2D1SolidColorBrush* textBrush, IDWriteTextFormat* textFormat)
	{
		renderTarget->DrawRectangle(m_Rect, brush);
		const std::wstring& textToDraw = m_ChoosenElementIndex == -1 ? m_MenuName : m_Elements[m_ChoosenElementIndex];
		renderTarget->DrawText(textToDraw.c_str(), static_cast<UINT32>(textToDraw.size()), textFormat, m_Rect, brush);

		if (m_IsOpen)
		{
			this->DrawElements(renderTarget, brush, textFormat);
		}
	}

	void Dropdown::DrawElements(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush, IDWriteTextFormat* textFormat) {
		D2D1_RECT_F elementRect = m_Rect;
		for (const auto& element : m_Elements) {
			elementRect.top += (m_Rect.bottom - m_Rect.top);
			elementRect.bottom += (m_Rect.bottom - m_Rect.top);
			renderTarget->DrawRectangle(elementRect, brush);
			renderTarget->DrawText(element.c_str(), static_cast<UINT32>(element.size()), textFormat, elementRect, brush);
		}
	}

	bool Dropdown::OnKeyStroke(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case VK_RETURN:
			break;
		}
		return false;
	}

	bool Dropdown::OnClick(float mouseX, float mouseY) {
		if (!IsVisible())
		{
			m_IsOpen = false;
			return false;
		}
		// Check if the main dropdown rectangle is clicked
		if (Component::OnClick(mouseX, mouseY)) {
			m_IsOpen = !m_IsOpen;
			return true;
		}

		// If the dropdown is open, check each element
		if (m_IsOpen) {
			float elementHeight = m_Rect.bottom - m_Rect.top;
			D2D1_RECT_F elementRect = m_Rect;

			// Iterate through each element's bounding box
			for (int elementIndex = 0; elementIndex < m_Elements.size(); ++elementIndex) {
				elementRect.top = m_Rect.bottom + (elementIndex * elementHeight);
				elementRect.bottom = elementRect.top + elementHeight;

				// Check if click is within the current element's rectangle
				if (mouseX >= elementRect.left && mouseX <= elementRect.right &&
					mouseY >= elementRect.top && mouseY <= elementRect.bottom) {

					m_ChoosenElementIndex = elementIndex;
					m_IsOpen = false; // Close the dropdown after selection

					// Trigger the callback if defined
					if (m_OnSelect) {
						m_OnSelect(m_ChoosenElementIndex);
					}

					return true;
				}
			}
		}

		// Close the dropdown if click is outside
		m_IsOpen = false;
		return false;  // No element was clicked
	}
}