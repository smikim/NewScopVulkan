#include "Transform.hpp"
#include "Animation.hpp"

namespace scop
{
	Transform::Transform()
		: Super(ComponentType::Transform)
	{
	}

	Transform::~Transform()
	{
	}
	void Transform::Init()
	{
	}
	void Transform::Update()
	{
	}

	void Transform::UpdateTransform()
	{
		mymath::Mat4 matScale = mymath::scale(mymath::Mat4{ 1.0f }, _localScale);

		mymath::Mat4 matRotation = mymath::rotate(mymath::Mat4{ 1.0f },_localRotation._x, mymath::Vec3(1.0f, 0.0f, 0.0f));
		matRotation = mymath::rotate(matRotation, _localRotation._y, mymath::Vec3(0.0f, 1.0f, 0.0f));
		matRotation = mymath::rotate(matRotation, _localRotation._z, mymath::Vec3(0.0f, 0.0f, 1.0f));
		
		mymath::Mat4 matTranslation = mymath::translate(mymath::Mat4{ 1.0f }, _localPosition);

		_matLocal = matScale * matRotation * matTranslation;

		if (HasParent())
		{
			_matWorld = _matLocal * _parent->GetWorldMatrix();
		}
		else
		{
			_matWorld = _matLocal;
		}

		//Quaternion quat;
		//_matWorld.Decompose(_scale, quat, _position);
		//_rotation = ToEulerAngles(quat);

		// Children
		for (const std::shared_ptr<Transform>& child : _children)
			child->UpdateTransform();
	}
	void Transform::SetScale(const mymath::Vec3& WorldScale)
	{
		if (HasParent())
		{
			mymath::Vec3 parentScale = _parent->GetScale();
			mymath::Vec3 scale = WorldScale;
			scale._x /= parentScale._x;
			scale._y /= parentScale._y;
			scale._z /= parentScale._z;
			SetLocalScale(scale);
		}
		else
			SetLocalScale(WorldScale);
	}
	void Transform::SetRotation(const mymath::Vec3& WorldRotation)
	{
		
	}
	void Transform::SetPosition(const mymath::Vec3& worldPosition)
	{
		if (HasParent())
		{
			//mymath::Mat4 worldToParentLocalMatrix = _parent->GetWorldMatrix().Invert();

			mymath::Vec3 position;
			
		}
		else
		{
			SetLocalPosition(worldPosition);
		}
	}
}