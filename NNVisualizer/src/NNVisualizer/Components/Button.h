#include "nnvpch.h"
#include "Component.h"

namespace Components {

	class Button : public Component {
	public:
		Button(const std::wstring& text, bool visible) :Component(visible), m_Text(text) {}
		Button(const std::wstring& text, std::function<void()> onClick, bool visible) : Component(visible), m_Text(text), m_OnClick(onClick) {}
		void Draw(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush, ID2D1SolidColorBrush* textBrush, IDWriteTextFormat* textFormat) override;
		bool OnClick(float mouseX, float mouseY) override;

		void SetOnClick(std::function<void()> onClick);
		bool OnKeyStroke(UINT message, WPARAM wParam, LPARAM lParam) override;
	private:
		std::wstring m_Text;
		std::function<void()> m_OnClick = nullptr;
	};
}