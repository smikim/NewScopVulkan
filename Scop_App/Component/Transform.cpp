#include "Transform.hpp"
#include "Animation.hpp"
#include "../Component/Camera.hpp"


Transform::Transform(vks::VulkanRenderer* renderer)
	: Super(COMPONENT_TYPE::TRANSFORM, renderer)
{
}

Transform::~Transform()
{
}

void Transform::FinalUpdate()
{
	UpdateTransform();
}

void Transform::UpdateTransform()
{
	mymath::Mat4 matScale = mymath::scale(mymath::Mat4{ 1.0f }, _localScale);

	mymath::Mat4 matRotation = mymath::rotate(mymath::Mat4{ 1.0f }, _localRotation._x, mymath::Vec3(1.0f, 0.0f, 0.0f));
	matRotation = mymath::rotate(matRotation, _localRotation._y, mymath::Vec3(0.0f, 1.0f, 0.0f));
	matRotation = mymath::rotate(matRotation, _localRotation._z, mymath::Vec3(0.0f, 0.0f, 1.0f));

	mymath::Mat4 matTranslation = mymath::translate(mymath::Mat4{ 1.0f }, _localPosition);

	_matLocal = matScale * matRotation * matTranslation;
	_matWorld = _matLocal;

	if (HasParent())
	{
		_matWorld = _matLocal * _parent->GetWorldMatrix();
	}
}

mymath::Mat4 Transform::getData()
{
	//mymath::Mat4 matWVP = _matWorld * Camera::S_MatView * Camera::S_MatProjection;
	mymath::Mat4 matWVP = _matWorld * Camera::S_MatView * Camera::S_MatProjection;

	return matWVP;
}
	
