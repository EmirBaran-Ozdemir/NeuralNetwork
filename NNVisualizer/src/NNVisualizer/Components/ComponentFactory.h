#pragma once

#include "nnpch.h"
#include "Button.h"
#include "TextField.h"
#include "Dropdown.h"

namespace Components {

	class ComponentFactory {
	public:
		static void InitializeBrushes(ID2D1HwndRenderTarget* renderTarget, const nlohmann::json& config);
		static std::unique_ptr<Button> CreateButton(const std::wstring& text, bool visible);
		static std::unique_ptr<Button> CreateButton(const std::wstring& text, std::function<void()> onClick, bool visible);
		static std::unique_ptr<TextField> CreateTextField(const std::wstring& text, bool visible);
		static std::unique_ptr<Dropdown> CreateDropdown(const std::wstring& text, const std::vector<std::wstring>& items, bool visible);

	private:
		static ID2D1SolidColorBrush* s_ButtonBrush;
		static ID2D1SolidColorBrush* s_TextFieldBrush;
		static ID2D1SolidColorBrush* s_DropdownBrush;
	};

}