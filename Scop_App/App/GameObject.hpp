#pragma once
#include <array>
#include <vector>
#include <memory>
#include "../Component/Component.hpp"

class MonoBehaviour;
class Transform;
class Camera;
class Input;

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

	//virtual void Awake();
	//virtual void Start();
	//virtual void Update();
	
	virtual void LateUpdate(Input& input, float deltaTime);
	virtual void FixedUpdate();

	std::shared_ptr<Component> GetFixedComponent(COMPONENT_TYPE type);
	std::shared_ptr<Transform> GetTransform();
	std::shared_ptr<Camera> GetCamera();
	
	//std::shared_ptr<MeshRenderer> GetMeshRenderer();
	//shared_ptr<Animator> GetAnimator();

	//std::shared_ptr<Transform> GetOrAddTransform();
	void AddComponent(std::shared_ptr<Component> component);

protected:
	std::array<std::shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	std::vector<std::shared_ptr<MonoBehaviour>> _scripts;
};

