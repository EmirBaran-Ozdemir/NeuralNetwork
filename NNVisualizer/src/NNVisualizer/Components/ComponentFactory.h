#pragma once
#include <vector>
#include <memory>
#include <string>
#include <d2d1.h>
#include <dwrite.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include "ButtonBuilder.h"
#include "TextFieldBuilder.h"
#include "DropdownBuilder.h"

namespace Components {
	class ComponentFactory {
	public:
		//! Node brushes
		static ID2D1SolidColorBrush* s_DefaultNodeBrush;
		static ID2D1SolidColorBrush* s_ActiveNodeBrush;
		static ID2D1SolidColorBrush* s_DeactiveNodeBrush;

		//! Weight brushes
		static ID2D1SolidColorBrush* s_DefaultWeightBrush;
		static ID2D1SolidColorBrush* s_LooseWeightBrush;
		static ID2D1SolidColorBrush* s_MediumWeightBrush;
		static ID2D1SolidColorBrush* s_TightWeightBrush;

		//! Component Brushes
		static ID2D1SolidColorBrush* s_DefaultComponentTextBrush;
		static ID2D1SolidColorBrush* s_SelectedComponentTextBrush;
		static ID2D1SolidColorBrush* s_DefaultComponentBorderBrush;
		static ID2D1SolidColorBrush* s_SelectedComponentBorderBrush;
		static ID2D1SolidColorBrush* s_DefaultComponentBackgroundBrush;
		static ID2D1SolidColorBrush* s_SelectedComponentBackgroundBrush;

		//! Display element brushes
		static ID2D1SolidColorBrush* s_ErrorTextBrush;
		static ID2D1SolidColorBrush* s_PropertyTextBrush;

		static IDWriteTextFormat* s_TextFormat;
		static IDWriteTextFormat* s_ErrorTextFormat;
		static IDWriteTextFormat* s_MenuTextFormat;

		static ID2D1HwndRenderTarget* s_RenderTarget;

		static void InitializeBrushes(ID2D1HwndRenderTarget* renderTarget, const std::string& configFilePath);
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