#include "../Component/Camera.hpp"
#include "../Component/Transform.hpp"
#include "../Includes/VulkanRenderer.hpp"

mymath::Mat4 Camera::S_MatView;
mymath::Mat4 Camera::S_MatProjection;

Camera::Camera(vks::VulkanRenderer* renderer)
	: Component(COMPONENT_TYPE::CAMERA, renderer)
{
}
Camera::~Camera()
{
}


void Camera::FinalUpdate()
{
	// 1)
/*	Vec3 eyePosition = GetTransform()->GetPosition();
	Vec3 focusPosition = eyePosition + GetTransform()->GetLook();
	Vec3 upDirection = GetTransform()->GetUp();
	S_MatView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);*/

	// 2)
	 _matView = GetTransform()->GetWorldMatrix().invert();
	//_matView = mymath::lookAtGLM(mymath::Vec3(0.0f, 0.0f, 8.0f), mymath::Vec3(0.0f, 0.0f, 0.0f), mymath::Vec3(0.0f, 1.0f, 0.0f));

	if (_type == PROJECTION_TYPE::PERSPECTIVE)
		_matProjection = mymath::perspective(mymath::radians(45.0f), _renderer->getAspectRatio(), 0.1f, 100.0f);
	
	_matProjection[5] *= -1;

	S_MatView = _matView;
	S_MatProjection = _matProjection;

}
void Camera::Render()
{
}

