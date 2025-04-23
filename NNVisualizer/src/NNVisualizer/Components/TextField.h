#pragma once
#include "nnvpch.h"
#include "Component.h"

namespace Components {

	class TextField : public Component {
	public:
		TextField(const std::wstring& label) :Component(label) {};
		TextField(const std::wstring& label, DrawProperties* drawProps, bool visible) : Component(label, drawProps, visible) {}

		bool OnKeyStroke(UINT message, WPARAM wParam, LPARAM lParam) override;
		bool OnClick(float mouseX, float mouseY) override;
		std::wstring GetText() const { return m_TextInput; }
	private:
		void Draw() override;
	private:
		std::wstring m_TextInput = L"";
	};
}