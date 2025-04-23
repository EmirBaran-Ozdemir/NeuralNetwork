#pragma once
#include <vector>
#include <memory>
#include <string>

namespace Components {
	class Component;
	struct DrawProperties;
	template <typename TBuilder>
	class ComponentBuilderBase {
	public:
		TBuilder& WithLabel(const std::wstring& label) {
			m_Label = label;
			return static_cast<TBuilder&>(*this);
		}

		TBuilder& AddDrawProperties(DrawProperties* drawProps) {
			//m_DrawProps = drawProps;
			return static_cast<TBuilder&>(*this);
		}

		TBuilder& Visible(bool visible) {
			m_Visible = visible;
			return static_cast<TBuilder&>(*this);
		}

		TBuilder& RegisterTo(std::vector<std::shared_ptr<Component>>& registry) {
			m_Registry = &registry;
			return static_cast<TBuilder&>(*this);
		}

		virtual std::shared_ptr<Component> Build() = 0;

		std::shared_ptr<Component> BuildAndRegister() {
			auto component = Build();
			if (m_Registry) {
				m_Registry->push_back(component);
			}
			return component;
		}

		virtual ~ComponentBuilderBase() = default;

	protected:
		std::wstring m_Label;
		DrawProperties* m_DrawProps = nullptr;
		bool m_Visible = true;
		std::vector<std::shared_ptr<Component>>* m_Registry = nullptr;
	};
}