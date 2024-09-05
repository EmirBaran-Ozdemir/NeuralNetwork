#include "Camera.h"

namespace Renderer {
	Camera::Camera()
		: m_Position({ 0.0f, 0.0f }), m_ZoomFactor(1.0f) {}

	void Camera::MoveLeft(float distance) {
		m_Position.x -= distance;
	}

	void Camera::MoveRight(float distance) {
		m_Position.x += distance;
	}

	void Camera::MoveUp(float distance) {
		m_Position.y -= distance;
	}

	void Camera::MoveDown(float distance) {
		m_Position.y += distance;
	}

	void Camera::Zoom(float zoomFactor, float cursorX, float cursorY)
	{
		// Calculate the cursor position in the world space before zoom
		float worldPosXBeforeZoom = m_Position.x + cursorX / m_ZoomFactor;
		float worldPosYBeforeZoom = m_Position.y + cursorY / m_ZoomFactor;

		// Apply the zoom
		m_ZoomFactor = zoomFactor;

		// Calculate the cursor position in the world space after zoom
		float worldPosXAfterZoom = m_Position.x + cursorX / m_ZoomFactor;
		float worldPosYAfterZoom = m_Position.y + cursorY / m_ZoomFactor;

		// Adjust the camera position to keep the cursor's world position consistent
		m_Position.x += worldPosXBeforeZoom - worldPosXAfterZoom;
		m_Position.y += worldPosYBeforeZoom - worldPosYAfterZoom;
	}


	D2D1_MATRIX_4X4_F Camera::GetViewMatrix() const {
		return D2D1::Matrix4x4F::Translation(-m_Position.x, -m_Position.y, 0.0f) *
			D2D1::Matrix4x4F::Scale(m_ZoomFactor, m_ZoomFactor, 1.0f);
	}
}