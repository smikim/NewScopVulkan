#include "Component.hpp"


namespace scop
{
	Component::Component(ComponentType type)
		:_type(type)
	{
	}
	Component::~Component()
	{
	}
	/*std::shared_ptr<scop::Transform> Component::GetTransform()
	{
		return _gameObject.lock()->GetTransform();
	}*/
}