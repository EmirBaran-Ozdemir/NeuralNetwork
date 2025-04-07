#pragma once
#include "nnvpch.h"
#include "NNVisualizer/Components/Component.h"

namespace Events
{
	class EventHandler {
	public:
		bool HandleKeyStroke(std::shared_ptr<Components::Component> component, UINT message, WPARAM wParam, LPARAM lParam);
		bool HandleMouseClick(std::shared_ptr<Components::Component> component, const POINT& worldCursorPos);
	};

}