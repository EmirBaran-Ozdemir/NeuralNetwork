#include "nnvpch.h"
#include "Component.h"

namespace Components {

	class TextField :public Component
	{
	public:
		TextField(const std::wstring& placeholder, bool visible) :Component(visible), m_Placeholder(placeholder) {};

		void Draw(ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* brush, ID2D1SolidColorBrush* textBrush, IDWriteTextFormat* textFormat) override;
		bool OnKeyStroke(UINT message, WPARAM wParam, LPARAM lParam) override;
		bool OnClick(float mouseX, float mouseY) override;
		std::wstring GetText() const { return m_TextInput; }
		std::wstring GetPlaceHolder() { return m_Placeholder; };

	private:
		std::wstring m_TextInput = L"";
		std::wstring m_Placeholder = L"Placeholder";
		bool m_Choosen = false;
	};
}