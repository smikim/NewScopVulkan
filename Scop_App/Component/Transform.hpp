#pragma once
#include "Component.hpp"
#include "../Library/Math/Vector.hpp"
#include "../Library/Math/Matrix.hpp"
#include "../Library/Math/math.hpp"
#include <vector>

namespace vks
{
	class VulkanRenderer;
}


class Transform : public Component
{
	using Super = Component;

public:
	Transform(vks::VulkanRenderer* renderer);
	~Transform();

	virtual void FinalUpdate() override;

	void UpdateTransform();
	mymath::Mat4 getData();

	// Local
	mymath::Vec3 GetLocalScale() { return _localScale; }

	void SetLocalScale(const mymath::Vec3& localScale) {
		_localScale = localScale;
		UpdateTransform();
	}
	
	mymath::Vec3 GetLocalRotation() { return _localRotation; }
	void SetLocalRotation(const mymath::Vec3& localRotation) {
		_localRotation = localRotation;
		UpdateTransform();
	}

	mymath::Vec3 GetLocalPosition() { return _localPosition; }
	void SetLocalPosition(const mymath::Vec3& localPosition) {
		_localPosition = localPosition;
		UpdateTransform();
	}

	mymath::Vec3 GetRight() { return _matWorld.Right(); }
	mymath::Vec3 GetUp() { return _matWorld.Up(); }
	mymath::Vec3 GetLook() { return _matWorld.Backward(); }

	mymath::Mat4 GetWorldMatrix() { return _matWorld; }

	//
	bool HasParent() { return _parent != nullptr; }
	std::shared_ptr<Transform> GetParent() { return _parent; }

	void SetParent(std::shared_ptr<Transform> parent) { _parent = parent; }


private:
	mymath::Vec3 _localScale{ 1.f, 1.f, 1.f };
	mymath::Vec3 _localRotation{};
	mymath::Vec3 _localPosition{};

	mymath::Mat4 _matLocal{ 1.0f };
	mymath::Mat4 _matWorld{ 1.0f };


private:
	std::shared_ptr<Transform> _parent;

};
