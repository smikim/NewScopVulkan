#include "MonoBehaviour.hpp"


MonoBehaviour::MonoBehaviour(vks::VulkanRenderer* renderer)
	: Component(COMPONENT_TYPE::MONO_BEHAVIOUR, renderer)
{
}
MonoBehaviour::~MonoBehaviour()
{
}

