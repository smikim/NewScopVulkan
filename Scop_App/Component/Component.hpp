#pragma once
#include <memory>

class GameObject;

namespace vks
{
class VulkanRenderer;
}

class Transform;
class Input;

enum class COMPONENT_TYPE
{
	TRANSFORM,
	CAMERA,
	MONO_BEHAVIOUR,
	// ...
	End,
};

enum
{
	FIXED_COMPONENT_COUNT = static_cast<uint8_t>(COMPONENT_TYPE::End) - 1
};

class Component
{
public:
	Component(COMPONENT_TYPE type, vks::VulkanRenderer* renderer);
	virtual ~Component();


	//virtual void Awake() { }
	//virtual void Start() { }
	//virtual void Update() { }
	virtual void LateUpdate(Input& input, float deltaTime) { }
	virtual void FinalUpdate() { }

	std::shared_ptr<GameObject> GetGameObject() { return _gameObject.lock();
	}
	std::shared_ptr<Transform> GetTransform();

	COMPONENT_TYPE GetType() { return _type; }
	//std::shared_ptr<Transform> GetTransform();
	void SetGameObject(std::shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }
	friend class GameObGameObjectject;

protected:

	COMPONENT_TYPE _type;
	std::weak_ptr<GameObject> _gameObject;
	vks::VulkanRenderer* _renderer;
};

	


