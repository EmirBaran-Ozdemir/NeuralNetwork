#pragma once
#include "ComponentBuilderBase.h"
#include "TextField.h"

namespace Components {
	class TextFieldBuilder : public ComponentBuilderBase<TextFieldBuilder> {
	public:
		std::shared_ptr<Component> Build() override {
			if (m_DrawProps) {
				return std::make_shared<TextField>(m_Label, m_DrawProps, m_Visible);
			}
			return std::make_shared<TextField>(m_Label);
		}
	};
}