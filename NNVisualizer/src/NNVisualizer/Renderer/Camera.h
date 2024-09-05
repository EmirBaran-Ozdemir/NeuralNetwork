#pragma once

namespace Renderer {

	class Camera {
	public:
		Camera();
		void MoveLeft(float distance);
		void MoveRight(float distance);
		void MoveUp(float distance);
		void MoveDown(float distance);
		void Zoom(float zoomFactor, D2D1_POINT_2F cursorPos);
		float GetZoomFactor() const { return m_ZoomFactor; }
		D2D1_POINT_2F GetCursorWorldPosition(D2D1_POINT_2F cursorPos);
		D2D1_MATRIX_4X4_F GetViewMatrix() const;
		D2D1_POINT_2F GetPosition() const { return m_Position; }
	private:
		D2D1_POINT_2F m_Position;
		float m_ZoomFactor;

	};
}
