#include "Camera.h"

namespace Renderer {
	Camera::Camera(float width, float height)
		: m_Position({ 0.0f, 0.0f }), m_ZoomFactor(1.0f), m_Width(width), m_Height(height) {}

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

	void Camera::Zoom(float factor) {
		m_ZoomFactor *= factor;
	}

	D2D1_MATRIX_4X4_F Camera::GetViewMatrix() const {
		return D2D1::Matrix4x4F::Translation(-m_Position.x, -m_Position.y, 0.0f) *
			D2D1::Matrix4x4F::Scale(m_ZoomFactor, m_ZoomFactor, 1.0f);
	}
}