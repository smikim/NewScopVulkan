#include "GameObject.hpp"

#include "../Component/Transform.hpp"
#include "../Component/Animation.hpp"
#include "../Component/Camera.hpp"
#include "../Component/MonoBehaviour.hpp"

#include <cassert>

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::LateUpdate(Input& input, float deltaTime)
{
	for (std::shared_ptr<Component>& component : _components)
	{
		if (component)
			component->LateUpdate(input, deltaTime);
	}

	for (std::shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->LateUpdate(input, deltaTime);
	}
}

void GameObject::FixedUpdate()
{
	for (std::shared_ptr<Component>& component : _components)
	{
		if (component)
			component->FinalUpdate();
	}
}

std::shared_ptr<Component> GameObject::GetFixedComponent(COMPONENT_TYPE type)
{
	uint8_t index = static_cast<uint8_t>(type);
	assert(index < FIXED_COMPONENT_COUNT);
	return _components[index];
}

std::shared_ptr<Transform> GameObject::GetTransform()
{
	std::shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::TRANSFORM);
	return static_pointer_cast<Transform>(component);
}

std::shared_ptr<Camera> GameObject::GetCamera()
{
	std::shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::CAMERA);
	return static_pointer_cast<Camera>(component);
}

void GameObject::AddComponent(std::shared_ptr<Component> component)
{
	component->SetGameObject(shared_from_this());

	uint8_t index = static_cast<uint8_t>(component->GetType());
	if (index < FIXED_COMPONENT_COUNT)
	{
		_components[index] = component;
	}
	else
	{
		_scripts.push_back(dynamic_pointer_cast<MonoBehaviour>(component));
	}
}
