#include "ComponentFactory.h"

namespace Components {
	//! Brushes
	ID2D1SolidColorBrush* ComponentFactory::s_DefaultNodeBrush = nullptr;
	ID2D1SolidColorBrush* ComponentFactory::s_ActiveNodeBrush = nullptr;
	ID2D1SolidColorBrush* ComponentFactory::s_DeactiveNodeBrush = nullptr;

	ID2D1SolidColorBrush* ComponentFactory::s_DefaultWeightBrush = nullptr;
	ID2D1SolidColorBrush* ComponentFactory::s_LooseWeightBrush = nullptr;
	ID2D1SolidColorBrush* ComponentFactory::s_MediumWeightBrush = nullptr;
	ID2D1SolidColorBrush* ComponentFactory::s_TightWeightBrush = nullptr;

	ID2D1SolidColorBrush* ComponentFactory::s_DefaultComponentTextBrush = nullptr;
	ID2D1SolidColorBrush* ComponentFactory::s_SelectedComponentTextBrush = nullptr;
	ID2D1SolidColorBrush* ComponentFactory::s_DefaultComponentBorderBrush = nullptr;
	ID2D1SolidColorBrush* ComponentFactory::s_SelectedComponentBorderBrush = nullptr;
	ID2D1SolidColorBrush* ComponentFactory::s_DefaultComponentBackgroundBrush = nullptr;
	ID2D1SolidColorBrush* ComponentFactory::s_SelectedComponentBackgroundBrush = nullptr;

	ID2D1SolidColorBrush* ComponentFactory::s_ErrorTextBrush = nullptr;
	ID2D1SolidColorBrush* ComponentFactory::s_PropertyTextBrush = nullptr;

	IDWriteTextFormat* ComponentFactory::s_TextFormat = nullptr;
	IDWriteTextFormat* ComponentFactory::s_ErrorTextFormat = nullptr;
	IDWriteTextFormat* ComponentFactory::s_MenuTextFormat = nullptr;
	ID2D1HwndRenderTarget* ComponentFactory::s_RenderTarget = nullptr;


	void ComponentFactory::InitializeBrushes(ID2D1HwndRenderTarget* renderTarget, const std::string& configFilePath)
	{
		s_RenderTarget = renderTarget;

		std::vector<std::filesystem::path> searchPaths =
		{
			std::filesystem::current_path() / "../../../NNVisualizer/src/NNVisualizer/Theme",
			std::filesystem::current_path() / "../../../../NNVisualizer/src/NNVisualizer/Theme"
		};

		std::ifstream ifs;
		for (const auto& path : searchPaths)
		{
			auto fullPath = path / "Configuration.nnc";
			ifs.open(fullPath);
			if (ifs.is_open())
			{
				break;
			}
		}
		std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		auto config = nlohmann::json::parse(content);


		// Parse Colors
		if (!config.contains("Colors") || !config["Colors"].is_object())
		{
			THROW_ERROR("Error: Colors is missing or not an object\n");
			return;
		}
		auto colors = config["Colors"];
		D2D1_COLOR_F defaultNodeColor = ParseColor(colors["Node"]["Default"]);
		D2D1_COLOR_F activeNodeColor = ParseColor(colors["Node"]["Active"]);
		D2D1_COLOR_F deactiveNodeColor = ParseColor(colors["Node"]["Deactive"]);
		D2D1_COLOR_F defaultWeightBrush = ParseColor(colors["Weights"]["Default"]);
		D2D1_COLOR_F looseWeightColor = ParseColor(colors["Weights"]["Loose"]);
		D2D1_COLOR_F mediumWeightColor = ParseColor(colors["Weights"]["Medium"]);
		D2D1_COLOR_F tightWeightColor = ParseColor(colors["Weights"]["Tight"]);
		D2D1_COLOR_F defaultComponentTextBrush = ParseColor(colors["DefaultComponentTextBrush"]);
		D2D1_COLOR_F selectedComponentTextBrush = ParseColor(colors["SelectedComponentTextBrush"]);
		D2D1_COLOR_F defaultComponentBorderBrush = ParseColor(colors["DefaultComponentBorderBrush"]);
		D2D1_COLOR_F selectedComponentBorderBrush = ParseColor(colors["SelectedComponentBorderBrush"]);
		D2D1_COLOR_F defaultComponentBackgroundBrush = ParseColor(colors["DefaultComponentBackgroundBrush"]);
		D2D1_COLOR_F selectedComponentBackgroundBrush = ParseColor(colors["SelectedComponentBackgroundBrush"]);
		D2D1_COLOR_F errorTextBrush = ParseColor(colors["ErrorTextBrush"]);
		D2D1_COLOR_F propertyTextBrush = ParseColor(colors["PropertyTextBrush"]);

		HRESULT hr = S_OK;
		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(defaultNodeColor, &s_DefaultNodeBrush);
		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(activeNodeColor, &s_ActiveNodeBrush);
		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(deactiveNodeColor, &s_DeactiveNodeBrush);

		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(defaultWeightBrush, &s_DefaultWeightBrush);
		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(looseWeightColor, &s_LooseWeightBrush);
		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(mediumWeightColor, &s_MediumWeightBrush);
		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(tightWeightColor, &s_TightWeightBrush);

		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(defaultComponentTextBrush, &s_DefaultComponentTextBrush);
		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(selectedComponentTextBrush, &s_SelectedComponentTextBrush);
		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(defaultComponentBorderBrush, &s_DefaultComponentBorderBrush);
		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(selectedComponentBorderBrush, &s_SelectedComponentBorderBrush);
		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(defaultComponentBackgroundBrush, &s_DefaultComponentBackgroundBrush);
		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(selectedComponentBackgroundBrush, &s_SelectedComponentBackgroundBrush);

		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(errorTextBrush, &s_ErrorTextBrush);
		if (SUCCEEDED(hr)) hr = renderTarget->CreateSolidColorBrush(propertyTextBrush, &s_PropertyTextBrush);

		if (FAILED(hr))
		{
			THROW_ERROR("Failed to create brushes\n");
			return;
		}

		// Parse TextFormats
		if (!config.contains("TextFormats") || !config["TextFormats"].is_object())
		{
			THROW_ERROR("Error: TextFormats is missing or not an object\n");
			return;
		}
		auto& textFormats = config["TextFormats"];

		IDWriteFactory* dWriteFactory = nullptr;
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&dWriteFactory));
		if (SUCCEEDED(hr))
		{
			// Default TextFormat
			if (textFormats.contains("Default") && textFormats["Default"].is_object())
			{
				auto defaultFormat = textFormats["Default"];
				if (defaultFormat.contains("Font") && defaultFormat["Font"].is_string() &&
					defaultFormat.contains("Size") && defaultFormat["Size"].is_number() &&
					defaultFormat.contains("Locale") && defaultFormat["Locale"].is_string())
				{
					std::wstring font = Utils::ValueParser::ConvertStringToWstring(defaultFormat["Font"].get<std::string>());
					std::wstring locale = Utils::ValueParser::ConvertStringToWstring((defaultFormat["Locale"].get<std::string>()));

					hr = dWriteFactory->CreateTextFormat(
						font.c_str(),
						nullptr,
						DWRITE_FONT_WEIGHT_REGULAR,
						DWRITE_FONT_STYLE_NORMAL,
						DWRITE_FONT_STRETCH_NORMAL,
						defaultFormat["Size"].get<float>(),
						locale.c_str(),
						&s_TextFormat
					);
					if (FAILED(hr))
					{
						THROW_ERROR("Failed to create TextFormat\n", hr);
					}
				}
				else {
					THROW_ERROR("Error: Invalid Default TextFormat structure\n");
				}
			}
			else {
				THROW_ERROR("Error: Default TextFormat is missing or not an object\n");
			}

			// Error TextFormat
			if (textFormats.contains("Error") && textFormats["Error"].is_object())
			{
				auto errorFormat = textFormats["Error"];
				if (errorFormat.contains("Font") && errorFormat["Font"].is_string() &&
					errorFormat.contains("Size") && errorFormat["Size"].is_number() &&
					errorFormat.contains("Locale") && errorFormat["Locale"].is_string())
				{
					std::wstring font = Utils::ValueParser::ConvertStringToWstring(errorFormat["Font"].get<std::string>());
					std::wstring locale = Utils::ValueParser::ConvertStringToWstring((errorFormat["Locale"].get<std::string>()));
					hr = dWriteFactory->CreateTextFormat(
						font.c_str(),
						nullptr,
						DWRITE_FONT_WEIGHT_REGULAR,
						DWRITE_FONT_STYLE_NORMAL,
						DWRITE_FONT_STRETCH_NORMAL,
						errorFormat["Size"].get<float>(),
						locale.c_str(),
						&s_ErrorTextFormat
					);
					if (FAILED(hr))
					{
						THROW_ERROR("Failed to create ErrorTextFormat\n", hr);
					}
				}
				else
				{
					THROW_ERROR("Error: Invalid Error TextFormat structure\n");
				}
			}
			else
			{
				THROW_ERROR("Error: Error TextFormat is missing or not an object\n");
			}

			// Menu TextFormat
			if (textFormats.contains("Menu") && textFormats["Menu"].is_object())
			{
				auto menuFormat = textFormats["Menu"];
				if (menuFormat.contains("Font") && menuFormat["Font"].is_string() &&
					menuFormat.contains("Size") && menuFormat["Size"].is_number() &&
					menuFormat.contains("Locale") && menuFormat["Locale"].is_string())
				{
					std::wstring font = Utils::ValueParser::ConvertStringToWstring(menuFormat["Font"].get<std::string>());
					std::wstring locale = Utils::ValueParser::ConvertStringToWstring((menuFormat["Locale"].get<std::string>()));
					hr = dWriteFactory->CreateTextFormat(
						font.c_str(),
						nullptr,
						DWRITE_FONT_WEIGHT_REGULAR,
						DWRITE_FONT_STYLE_NORMAL,
						DWRITE_FONT_STRETCH_NORMAL,
						menuFormat["Size"].get<float>(),
						locale.c_str(),
						&s_MenuTextFormat
					);
					if (FAILED(hr))
					{
						THROW_ERROR("Failed to create MenuTextFormat\n");
					}
				}
				else
				{
					THROW_ERROR("Error: Invalid Menu TextFormat structure\n");
				}
			}
			else
			{
				THROW_ERROR("Error: Menu TextFormat is missing or not an object\n");
			}

			dWriteFactory->Release();
		}
		else
		{
			THROW_ERROR("Failed to create DWriteFactory\n");
		}
	}

	void ComponentFactory::ReleaseResources()
	{
		if (s_DefaultNodeBrush) s_DefaultNodeBrush->Release();
		if (s_ActiveNodeBrush) s_ActiveNodeBrush->Release();
		if (s_DeactiveNodeBrush) s_DeactiveNodeBrush->Release();

		if (s_DefaultWeightBrush) s_DefaultWeightBrush->Release();
		if (s_LooseWeightBrush) s_LooseWeightBrush->Release();
		if (s_MediumWeightBrush) s_MediumWeightBrush->Release();
		if (s_TightWeightBrush) s_TightWeightBrush->Release();

		if (s_DefaultComponentTextBrush) s_DefaultComponentTextBrush->Release();
		if (s_SelectedComponentTextBrush) s_SelectedComponentTextBrush->Release();
		if (s_DefaultComponentBorderBrush) s_DefaultComponentBorderBrush->Release();
		if (s_SelectedComponentBorderBrush) s_SelectedComponentBorderBrush->Release();
		if (s_DefaultComponentBackgroundBrush) s_DefaultComponentBackgroundBrush->Release();
		if (s_SelectedComponentBackgroundBrush) s_SelectedComponentBackgroundBrush->Release();

		if (s_ErrorTextBrush) s_ErrorTextBrush->Release();
		if (s_PropertyTextBrush) s_PropertyTextBrush->Release();

		if (s_TextFormat) s_TextFormat->Release();
		if (s_ErrorTextFormat) s_ErrorTextFormat->Release();
		if (s_MenuTextFormat) s_MenuTextFormat->Release();

		s_DefaultComponentTextBrush = s_SelectedComponentTextBrush = s_SelectedComponentBorderBrush = s_SelectedComponentBackgroundBrush = nullptr;
		s_TextFormat = s_ErrorTextFormat = s_MenuTextFormat = nullptr;
		s_RenderTarget = nullptr;
	}

	ButtonBuilder ComponentFactory::CreateButtonBuilder() {
		ButtonBuilder builder;
		DrawProperties* props = new DrawProperties{
			s_RenderTarget, s_DefaultComponentBorderBrush, s_DefaultComponentTextBrush, s_MenuTextFormat
		};
		builder.AddDrawProperties(props);
		return builder;
	}

	std::shared_ptr<Button> ComponentFactory::CreateButton(const std::wstring& text, bool visible)
	{
		ButtonBuilder builder = CreateButtonBuilder();
		auto component = builder
			.WithLabel(text)
			.Visible(visible)
			.Build();
		return std::static_pointer_cast<Button>(component);
	}

	std::shared_ptr<Button> ComponentFactory::CreateAndRegisterButton(std::vector<std::shared_ptr<Component>>& registry, const std::wstring& text, bool visible)
	{
		ButtonBuilder builder = CreateButtonBuilder();
		auto component = builder
			.WithLabel(text)
			.Visible(visible)
			.RegisterTo(registry)
			.BuildAndRegister();
		return std::static_pointer_cast<Button>(component);
	}

	TextFieldBuilder ComponentFactory::CreateTextFieldBuilder()
	{
		TextFieldBuilder builder;
		DrawProperties* props = new DrawProperties{
			s_RenderTarget, s_DefaultComponentBorderBrush, s_DefaultComponentTextBrush, s_TextFormat
		};
		builder.AddDrawProperties(props);
		return builder;
	}

	std::shared_ptr<TextField> ComponentFactory::CreateTextField(const std::wstring& text, bool visible)
	{
		TextFieldBuilder builder = CreateTextFieldBuilder();
		auto component = builder
			.WithLabel(text)
			.Visible(visible)
			.Build();
		return std::static_pointer_cast<TextField>(component);
	}

	std::shared_ptr<TextField> ComponentFactory::CreateAndRegisterTextField(std::vector<std::shared_ptr<Component>>& registry, const std::wstring& text, bool visible)
	{
		TextFieldBuilder builder = CreateTextFieldBuilder();
		auto component = builder
			.WithLabel(text)
			.Visible(visible)
			.RegisterTo(registry)
			.BuildAndRegister();
		return std::static_pointer_cast<TextField>(component);
	}

	DropdownBuilder ComponentFactory::CreateDropdownBuilder()
	{
		DropdownBuilder builder;
		DrawProperties* props = new DrawProperties{
			s_RenderTarget, s_DefaultComponentBorderBrush, s_DefaultComponentTextBrush, s_MenuTextFormat
		};
		builder.AddDrawProperties(props);
		return builder;
	}

	std::shared_ptr<Dropdown> ComponentFactory::CreateDropdown(const std::wstring& menuName, const std::vector<std::wstring>& elements)
	{
		DropdownBuilder builder = CreateDropdownBuilder();
		auto component = builder
			.WithLabel(menuName)
			.WithItems(elements)
			.Build();
		return std::static_pointer_cast<Dropdown>(component);
	}

	std::shared_ptr<Dropdown> ComponentFactory::CreateAndRegisterDropdown(std::vector<std::shared_ptr<Component>>& registry, const std::wstring& menuName, const std::vector<std::wstring>& elements)
	{
		DropdownBuilder builder = CreateDropdownBuilder();
		auto component = builder
			.WithLabel(menuName)
			.WithItems(elements)
			.RegisterTo(registry)
			.BuildAndRegister();
		return std::static_pointer_cast<Dropdown>(component);
	}

	D2D1_COLOR_F ComponentFactory::ParseColor(const nlohmann::json& colorJson)
	{
		if (!colorJson.is_object())
		{
			THROW_ERROR("Expected a JSON object for color");
		}

		float r = colorJson.at("r").get<float>();
		float g = colorJson.at("g").get<float>();
		float b = colorJson.at("b").get<float>();
		float a = colorJson.at("a").get<float>();

		return D2D1::ColorF(r, g, b, a);
	}
}