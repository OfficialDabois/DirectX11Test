#include "Camera.h"

Camera::Camera(XMVECTOR position, XMVECTOR target, XMVECTOR up): pos(position), target(target), up(up) {
	view = XMMatrixLookAtLH(position, target, up);
}

XMMATRIX Camera::GetView() {
	return view;
}

void Camera::Move(XMVECTOR pos) {
	this->pos = pos;

	view = XMMatrixLookAtLH(this->pos, this->target, this->up);
}