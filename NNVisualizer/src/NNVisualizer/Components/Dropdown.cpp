#pragma once
#include "Dropdown.h"

namespace Components {

	void Dropdown::Draw()
	{
		m_DrawProperties->RenderTarget->DrawRectangle(m_BaseBounds, m_DrawProperties->Brush);
		const std::wstring& textToDraw = m_ChoosenElementIndex == -1 ? m_Label : m_Elements[m_ChoosenElementIndex];
		m_DrawProperties->RenderTarget->DrawText(textToDraw.c_str(), static_cast<UINT32>(textToDraw.size()), m_DrawProperties->TextFormat, m_BaseBounds, m_DrawProperties->Brush);

		if (m_IsOpen)
		{
			this->DrawElements();
		}
	}

	void Dropdown::DrawElements() {
		D2D1_RECT_F elementRect = m_BaseBounds;
		for (const auto& element : m_Elements) {
			elementRect.top += (m_BaseBounds.bottom - m_BaseBounds.top);
			elementRect.bottom += (m_BaseBounds.bottom - m_BaseBounds.top);
			m_DrawProperties->RenderTarget->DrawRectangle(elementRect, m_DrawProperties->Brush);
			m_DrawProperties->RenderTarget->DrawText(element.c_str(), static_cast<UINT32>(element.size()), m_DrawProperties->TextFormat, elementRect, m_DrawProperties->Brush);
		}
	}

	void Dropdown::SetComponentBounds()
	{
		m_BaseBounds = m_Rect;
		float elementHeight = m_BaseBounds.bottom - m_BaseBounds.top;
		m_ExpandedBounds = m_BaseBounds;
		m_ExpandedBounds.bottom = m_BaseBounds.bottom + elementHeight * m_Elements.size();
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
		// If the dropdown is open, check each element
		if (m_IsOpen) {
			float elementHeight = m_BaseBounds.bottom - m_BaseBounds.top;
			D2D1_RECT_F elementRect = m_BaseBounds;

			// Iterate through each element's bounding box
			for (int elementIndex = 0; elementIndex < m_Elements.size(); ++elementIndex) {
				elementRect.top = m_BaseBounds.bottom + (elementIndex * elementHeight);
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
		// Check if the main dropdown rectangle is clicked
		if (mouseX >= m_BaseBounds.left && mouseX <= m_BaseBounds.right &&
			mouseY >= m_BaseBounds.top && mouseY <= m_BaseBounds.bottom)
		{
			m_IsOpen = !m_IsOpen;
			if (m_IsOpen) // If opened expand bounds
				m_Rect = m_ExpandedBounds;
			else
				m_Rect = m_BaseBounds;
			return true;
		}

		// Close the dropdown if click is outside
		m_IsOpen = false;
		return false;  // No element was clicked
	}
}