#include "ComponentFactory.h"

namespace Components {
	// Definitions of static members
	ID2D1SolidColorBrush* ComponentFactory::s_ButtonBrush = nullptr;
	ID2D1SolidColorBrush* ComponentFactory::s_TextFieldBrush = nullptr;
	ID2D1SolidColorBrush* ComponentFactory::s_DropdownBrush = nullptr;
	ID2D1SolidColorBrush* ComponentFactory::s_TextBrush = nullptr;
	ID2D1SolidColorBrush* ComponentFactory::s_GrayBrush = nullptr;
	ID2D1SolidColorBrush* ComponentFactory::s_BlackBrush = nullptr;
	ID2D1SolidColorBrush* ComponentFactory::s_LimeGreenBrush = nullptr;
	IDWriteTextFormat* ComponentFactory::s_TextFormat = nullptr;
	IDWriteTextFormat* ComponentFactory::s_ErrorTextFormat = nullptr;
	IDWriteTextFormat* ComponentFactory::s_MenuTextFormat = nullptr;
	ID2D1HwndRenderTarget* ComponentFactory::s_RenderTarget = nullptr;

	void ComponentFactory::InitializeBrushes(ID2D1HwndRenderTarget* renderTarget, const nlohmann::json& config) {
		s_RenderTarget = renderTarget;

		auto colors = config["Colors"];
		D2D1_COLOR_F activeColor = ParseColor(colors["Node"]["Active"]);
		D2D1_COLOR_F deactiveColor = ParseColor(colors["Node"]["Deactive"]);
		D2D1_COLOR_F mediumColor = ParseColor(colors["Weights"]["Medium"]);
		D2D1_COLOR_F textColor = ParseColor(colors["Text"]);
		D2D1_COLOR_F grayColor = ParseColor(colors["Gray"]);
		D2D1_COLOR_F blackColor = ParseColor(colors["Black"]);
		D2D1_COLOR_F limeGreenColor = ParseColor(colors["LimeGreen"]);

		HRESULT hr = S_OK;
		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(activeColor, &s_ButtonBrush);
		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(deactiveColor, &s_TextFieldBrush);
		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(mediumColor, &s_DropdownBrush);
		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(textColor, &s_TextBrush);
		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(grayColor, &s_GrayBrush);
		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(blackColor, &s_BlackBrush);
		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(limeGreenColor, &s_LimeGreenBrush);

		if (FAILED(hr)) {
			OutputDebugString("Failed to create brushes\n");
			return;
		}

		auto textFormats = config["TextFormats"];
		IDWriteFactory* dWriteFactory = nullptr;
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&dWriteFactory));
		if (SUCCEEDED(hr)) {
			auto defaultFormat = textFormats["Default"];
			hr = dWriteFactory->CreateTextFormat(
				defaultFormat["Font"].get<std::wstring>().c_str(),
				nullptr,
				DWRITE_FONT_WEIGHT_REGULAR,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				defaultFormat["Size"].get<float>(),
				defaultFormat["Locale"].get<std::wstring>().c_str(),
				&s_TextFormat
			);
			if (FAILED(hr)) OutputDebugString("Failed to create TextFormat\n");

			auto errorFormat = textFormats["Error"];
			hr = dWriteFactory->CreateTextFormat(
				errorFormat["Font"].get<std::wstring>().c_str(),
				nullptr,
				DWRITE_FONT_WEIGHT_REGULAR,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				errorFormat["Size"].get<float>(),
				errorFormat["Locale"].get<std::wstring>().c_str(),
				&s_ErrorTextFormat
			);
			if (FAILED(hr)) OutputDebugString("Failed to create ErrorTextFormat\n");

			auto menuFormat = textFormats["Menu"];
			hr = dWriteFactory->CreateTextFormat(
				menuFormat["Font"].get<std::wstring>().c_str(),
				nullptr,
				DWRITE_FONT_WEIGHT_REGULAR,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				menuFormat["Size"].get<float>(),
				menuFormat["Locale"].get<std::wstring>().c_str(),
				&s_MenuTextFormat
			);
			if (FAILED(hr)) OutputDebugString("Failed to create MenuTextFormat\n");

			dWriteFactory->Release();
		}
	}

	void ComponentFactory::ReleaseResources() {
		if (s_ButtonBrush) s_ButtonBrush->Release();
		if (s_TextFieldBrush) s_TextFieldBrush->Release();
		if (s_DropdownBrush) s_DropdownBrush->Release();
		if (s_TextBrush) s_TextBrush->Release();
		if (s_GrayBrush) s_GrayBrush->Release();
		if (s_BlackBrush) s_BlackBrush->Release();
		if (s_LimeGreenBrush) s_LimeGreenBrush->Release();
		if (s_TextFormat) s_TextFormat->Release();
		if (s_ErrorTextFormat) s_ErrorTextFormat->Release();
		if (s_MenuTextFormat) s_MenuTextFormat->Release();
		s_ButtonBrush = s_TextFieldBrush = s_DropdownBrush = s_TextBrush = nullptr;
		s_GrayBrush = s_BlackBrush = s_LimeGreenBrush = nullptr;
		s_TextFormat = s_ErrorTextFormat = s_MenuTextFormat = nullptr;
		s_RenderTarget = nullptr;
	}

	ButtonBuilder ComponentFactory::CreateButtonBuilder() {
		ButtonBuilder builder;
		DrawProperties* props = new DrawProperties{
			s_RenderTarget, s_ButtonBrush, s_TextBrush, s_MenuTextFormat
		};
		builder.AddDrawProperties(props);
		return builder;
	}

	std::shared_ptr<Button> ComponentFactory::CreateButton(const std::wstring& text, bool visible) {
		ButtonBuilder builder = CreateButtonBuilder();
		auto component = builder
			.WithLabel(text)
			.Visible(visible)
			.Build();
		return std::static_pointer_cast<Button>(component);
	}

	std::shared_ptr<Button> ComponentFactory::CreateAndRegisterButton(
		std::vector<std::shared_ptr<Component>>& registry,
		const std::wstring& text,
		bool visible
	) {
		ButtonBuilder builder = CreateButtonBuilder();
		auto component = builder
			.WithLabel(text)
			.Visible(visible)
			.RegisterTo(registry)
			.BuildAndRegister();
		return std::static_pointer_cast<Button>(component);
	}

	TextFieldBuilder ComponentFactory::CreateTextFieldBuilder() {
		TextFieldBuilder builder;
		DrawProperties* props = new DrawProperties{
			s_RenderTarget, s_TextFieldBrush, s_TextBrush, s_TextFormat
		};
		builder.AddDrawProperties(props);
		return builder;
	}

	std::shared_ptr<TextField> ComponentFactory::CreateTextField(const std::wstring& text, bool visible) {
		TextFieldBuilder builder = CreateTextFieldBuilder();
		auto component = builder
			.WithLabel(text)
			.Visible(visible)
			.Build();
		return std::static_pointer_cast<TextField>(component);
	}

	std::shared_ptr<TextField> ComponentFactory::CreateAndRegisterTextField(
		std::vector<std::shared_ptr<Component>>& registry,
		const std::wstring& text,
		bool visible
	) {
		TextFieldBuilder builder = CreateTextFieldBuilder();
		auto component = builder
			.WithLabel(text)
			.Visible(visible)
			.RegisterTo(registry)
			.BuildAndRegister();
		return std::static_pointer_cast<TextField>(component);
	}

	DropdownBuilder ComponentFactory::CreateDropdownBuilder() {
		DropdownBuilder builder;
		DrawProperties* props = new DrawProperties{
			s_RenderTarget, s_DropdownBrush, s_TextBrush, s_MenuTextFormat
		};
		builder.AddDrawProperties(props);
		return builder;
	}

	std::shared_ptr<Dropdown> ComponentFactory::CreateDropdown(
		const std::wstring& menuName,
		const std::vector<std::wstring>& elements
	) {
		DropdownBuilder builder = CreateDropdownBuilder();
		auto component = builder
			.WithLabel(menuName)
			.WithItems(elements)
			.Build();
		return std::static_pointer_cast<Dropdown>(component);
	}

	std::shared_ptr<Dropdown> ComponentFactory::CreateAndRegisterDropdown(
		std::vector<std::shared_ptr<Component>>& registry,
		const std::wstring& menuName,
		const std::vector<std::wstring>& elements
	) {
		DropdownBuilder builder = CreateDropdownBuilder();
		auto component = builder
			.WithLabel(menuName)
			.WithItems(elements)
			.RegisterTo(registry)
			.BuildAndRegister();
		return std::static_pointer_cast<Dropdown>(component);
	}

	D2D1_COLOR_F ComponentFactory::ParseColor(const nlohmann::json& colorJson) {
		float r = colorJson["r"].get<float>();
		float g = colorJson["g"].get<float>();
		float b = colorJson["b"].get<float>();
		float a = colorJson["a"].get<float>();
		return D2D1::ColorF(r, g, b, a);
	}
}