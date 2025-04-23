#pragma once
#include "nnvpch.h"
#include "Component.h"

namespace Components {

	class Button : public Component {
	public:
		Button(const std::wstring& label) : Component(label) {}
		Button(const std::wstring& label, DrawProperties* drawProps, bool visible) : Component(label, drawProps, visible) {}

		bool OnClick(float mouseX, float mouseY) override;
		void SetFunction(std::function<void()> onClick);
		bool OnKeyStroke(UINT message, WPARAM wParam, LPARAM lParam) override;
	private:
		void Draw() override;

	private:
		std::function<void()> m_OnClick = nullptr;
	};
}