#pragma once
#include <memory>
#include "../App/HumanGLObject.hpp"

namespace humanGL
{
class HumanGLObject;
}

namespace scop
{
class Transform;

enum class ComponentType
{
	Transform,
	Camera,
	Animator,
	// ...
	End,
};

enum
{
	FIXED_COMPONENT_COUNT = static_cast<uint8_t>(ComponentType::End) - 1
};

class Component
{
public:
	Component(ComponentType type);
	virtual ~Component();

	virtual void Init() abstract;
	virtual void Update() abstract;

	std::shared_ptr<humanGL::HumanGLObject> GetGameObject() { return _gameObject.lock();
	}
	ComponentType GetType() { return _type; }
	//std::shared_ptr<Transform> GetTransform();
	void SetGameObject(std::shared_ptr<humanGL::HumanGLObject> gameObject) { _gameObject = gameObject; }
	friend class HumanGLObject;

protected:

	ComponentType _type;
	std::weak_ptr<humanGL::HumanGLObject> _gameObject;
};

}
	


