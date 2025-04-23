#pragma once
#include "nnvpch.h"
#include "Component.h"

namespace Components {
	class Dropdown : public Component {
	public:
		Dropdown(const std::wstring& label, const std::vector<std::wstring>& elements) : Component(label), m_Elements(elements) {}
		Dropdown(const std::wstring& label, const std::vector<std::wstring>& elements, DrawProperties* drawProps, bool visible) : Component(label, drawProps, visible), m_Elements(elements) {}

		bool OnClick(float mouseX, float mouseY) override;
		bool OnKeyStroke(UINT message, WPARAM wParam, LPARAM lParam) override;
		int GetChoosenIndex() const { return m_ChoosenElementIndex; }
		void OnBoundsSet() override {
			SetComponentBounds();
		}

	private:
		void SetComponentBounds();
		void Draw() override;
		void DrawElements();

	private:
		std::vector<std::wstring> m_Elements;
		bool m_IsOpen = false;
		int m_ChoosenElementIndex = -1;
		std::function<void(int)> m_OnSelect = nullptr;
		D2D1_RECT_F m_ExpandedBounds{ -1.0f,-1.0f,-1.0f,-1.0f };
		D2D1_RECT_F m_BaseBounds{ -1.0f,-1.0f,-1.0f,-1.0f };
	};
}