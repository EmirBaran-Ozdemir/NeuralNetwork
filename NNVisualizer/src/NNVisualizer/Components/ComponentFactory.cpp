#include "ComponentFactory.h"

namespace Components {

	ID2D1SolidColorBrush* ComponentFactory::s_ButtonBrush = nullptr;
	ID2D1SolidColorBrush* ComponentFactory::s_TextFieldBrush = nullptr;
	ID2D1SolidColorBrush* ComponentFactory::s_DropdownBrush = nullptr;

	D2D1_COLOR_F ParseColor(const nlohmann::json& colorJson) {
		float r = colorJson["r"].get<float>();
		float g = colorJson["g"].get<float>();
		float b = colorJson["b"].get<float>();
		float a = colorJson["a"].get<float>();
		return D2D1::ColorF(r, g, b, a);
	}

	void ComponentFactory::InitializeBrushes(ID2D1HwndRenderTarget* renderTarget, const nlohmann::json& config) {
		auto colors = config["Colors"];

		auto nodeColors = colors["Node"];
		D2D1_COLOR_F activeColor = ParseColor(nodeColors["Active"]);
		D2D1_COLOR_F deactiveColor = ParseColor(nodeColors["Deactive"]);

		renderTarget->CreateSolidColorBrush(activeColor, &s_ButtonBrush);
		renderTarget->CreateSolidColorBrush(deactiveColor, &s_TextFieldBrush);

		auto weights = colors["Weights"];
		D2D1_COLOR_F mediumColor = ParseColor(weights["Medium"]);
		renderTarget->CreateSolidColorBrush(mediumColor, &s_DropdownBrush);
	}

	std::unique_ptr<Button> ComponentFactory::CreateButton(const std::wstring& text) {
		auto button = std::make_unique<Button>(text);
		return button;
	}

	std::unique_ptr<TextField> ComponentFactory::CreateTextField(const std::wstring& text) {
		auto textField = std::make_unique<TextField>(text);
		return textField;
	}

	std::unique_ptr<Dropdown> ComponentFactory::CreateDropdown(const std::wstring& text, const std::vector<std::wstring>& items) {
		auto dropdown = std::make_unique<Dropdown>(text, items);
		return dropdown;
	}

}
