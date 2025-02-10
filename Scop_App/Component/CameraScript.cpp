#include "CameraScript.hpp"

#include "../Library/Math/Vector.hpp"
#include "../App/GameObject.hpp"
#include "../Includes/Input.hpp"
#include "Component.hpp"

#include "Transform.hpp"

CameraScript::CameraScript(vks::VulkanRenderer* renderer)
	: MonoBehaviour(renderer)
{
}

CameraScript::~CameraScript()
{
}

void CameraScript::LateUpdate(Input& input, float deltaTime)
{
	mymath::Vec3 pos = GetTransform()->GetLocalPosition();

	if (input.GetButton(KEY_TYPE::W))
		pos += GetTransform()->GetLook() * _speed * deltaTime;

	if (input.GetButton(KEY_TYPE::S))
		pos -= GetTransform()->GetLook() * _speed * deltaTime;

	if (input.GetButton(KEY_TYPE::A))
		pos -= GetTransform()->GetRight() * _speed * deltaTime;

	if (input.GetButton(KEY_TYPE::D))
		pos += GetTransform()->GetRight() * _speed * deltaTime;

	if (input.GetButton(KEY_TYPE::Q))
	{
		mymath::Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation._x += deltaTime * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (input.GetButton(KEY_TYPE::E))
	{
		mymath::Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation._x -= deltaTime * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	GetTransform()->SetLocalPosition(pos);
}
