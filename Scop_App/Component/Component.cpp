#include "Component.hpp"
#include "../App/GameObject.hpp"
#include "../Scop_App/Includes/VulkanRenderer.hpp"

Component::Component(COMPONENT_TYPE type, vks::VulkanRenderer* renderer)
	:_type(type), _renderer(renderer)
{
}

Component::~Component()
{
}

std::shared_ptr<Transform> Component::GetTransform()
{
	return _gameObject.lock()->GetTransform();
}
/*std::shared_ptr<scop::Transform> Component::GetTransform()
{
	return _gameObject.lock()->GetTransform();
}*/
	
