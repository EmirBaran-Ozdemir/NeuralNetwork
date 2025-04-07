#pragma once
#include "EventHandler.h"

namespace Events {
	bool EventHandler::HandleKeyStroke(std::shared_ptr<Components::Component> component, UINT message, WPARAM wParam, LPARAM lParam) {

		if (component != nullptr) {
			bool handled = component->OnKeyStroke(message, wParam, lParam);
			if (handled) return true;
			return false;
		}
		return false;
	}

	bool EventHandler::HandleMouseClick(std::shared_ptr<Components::Component> component, const POINT& worldCursorPos)
	{
		if (component != nullptr) {
			bool handled = component->OnClick(worldCursorPos.x, worldCursorPos.y);
			if (handled) return true;
			return false;
		}
		return false;
	}

}
