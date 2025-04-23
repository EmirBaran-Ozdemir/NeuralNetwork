#pragma once
#include <vector>
#include <memory>
#include <string>
#include <d2d1.h>
#include <dwrite.h>
#include <nlohmann/json.hpp>
#include "ButtonBuilder.h"
#include "TextFieldBuilder.h"
#include "DropdownBuilder.h"

namespace Components {
	class ComponentFactory {
	public:
		static ID2D1SolidColorBrush* s_ButtonBrush;
		static ID2D1SolidColorBrush* s_TextFieldBrush;
		static ID2D1SolidColorBrush* s_DropdownBrush;
		static ID2D1SolidColorBrush* s_TextBrush;
		static ID2D1SolidColorBrush* s_GrayBrush;
		static ID2D1SolidColorBrush* s_BlackBrush;
		static ID2D1SolidColorBrush* s_LimeGreenBrush;
		static IDWriteTextFormat* s_TextFormat;
		static IDWriteTextFormat* s_ErrorTextFormat;
		static IDWriteTextFormat* s_MenuTextFormat;
		static ID2D1HwndRenderTarget* s_RenderTarget;

		static void InitializeBrushes(ID2D1HwndRenderTarget* renderTarget, const nlohmann::json& config);
		static void ReleaseResources();

		static ButtonBuilder CreateButtonBuilder();
		static std::shared_ptr<Button> CreateButton(const std::wstring& text, bool visible);
		static std::shared_ptr<Button> CreateAndRegisterButton(
			std::vector<std::shared_ptr<Component>>& registry,
			const std::wstring& text,
			bool visible
		);

		static TextFieldBuilder CreateTextFieldBuilder();
		static std::shared_ptr<TextField> CreateTextField(const std::wstring& text, bool visible);
		static std::shared_ptr<TextField> CreateAndRegisterTextField(
			std::vector<std::shared_ptr<Component>>& registry,
			const std::wstring& text,
			bool visible
		);

		static DropdownBuilder CreateDropdownBuilder();
		static std::shared_ptr<Dropdown> CreateDropdown(
			const std::wstring& menuName,
			const std::vector<std::wstring>& elements
		);
		static std::shared_ptr<Dropdown> CreateAndRegisterDropdown(
			std::vector<std::shared_ptr<Component>>& registry,
			const std::wstring& menuName,
			const std::vector<std::wstring>& elements
		);

	private:
		static D2D1_COLOR_F ParseColor(const nlohmann::json& colorJson);
	};
}