#pragma once
#include "ComponentBuilderBase.h"
#include "Button.h"

namespace Components {
	class ButtonBuilder : public ComponentBuilderBase<ButtonBuilder> {
	public:
		std::shared_ptr<Component> Build() override {
			if (m_DrawProps) {
				return std::make_shared<Button>(m_Label, m_DrawProps, m_Visible);
			}
			return std::make_shared<Button>(m_Label);
		}
	};
}