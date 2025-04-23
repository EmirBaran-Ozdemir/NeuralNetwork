#pragma once
#include "ComponentBuilderBase.h"
#include "Dropdown.h"

namespace Components {
	class DropdownBuilder : public ComponentBuilderBase<DropdownBuilder> {
	private:
		std::vector<std::wstring> m_Items;

	public:
		DropdownBuilder& WithItems(const std::vector<std::wstring>& items) {
			m_Items = items;
			return *this;
		}

		std::shared_ptr<Component> Build() override {
			if (m_DrawProps) {
				return std::make_shared<Dropdown>(m_Label, m_Items, m_DrawProps, m_Visible);
			}
			return std::make_shared<Dropdown>(m_Label, m_Items);
		}
	};
}