#pragma once

namespace Renderer {

	class Camera {
	public:
		Camera(float width, float height);
		void MoveLeft(float distance);
		void MoveRight(float distance);
		void MoveUp(float distance);
		void MoveDown(float distance);
		void Zoom(float factor);
		D2D1_MATRIX_4X4_F GetViewMatrix() const;
		D2D1_POINT_2F GetPosition() const { return m_Position; }
	private:
		D2D1_POINT_2F m_Position;
		float m_ZoomFactor;
		float m_Width;
		float m_Height;
	};
}
