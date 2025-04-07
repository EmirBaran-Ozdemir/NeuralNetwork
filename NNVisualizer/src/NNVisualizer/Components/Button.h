#include "nnvpch.h"
#include "Component.h"

namespace Components {

	class Button : public Component {
	public:
		Button(const std::wstring& text) : m_Text(text) {}
		Button(const std::wstring& text, std::function<bool()> onClick) : m_Text(text), m_OnClick(onClick) {}
		void Draw(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush, ID2D1SolidColorBrush* textBrush, IDWriteTextFormat* textFormat) override;
		bool OnClick(float mouseX, float mouseY) override;

		void SetOnClick(std::function<bool()> onClick);
		bool OnKeyStroke(UINT message, WPARAM wParam, LPARAM lParam) override;
	private:
		std::wstring m_Text;
		std::function<bool()> m_OnClick = nullptr;
	};
}