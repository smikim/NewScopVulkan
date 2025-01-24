#pragma once
#include "Component.hpp"
#include "../Library/Math/Vector.hpp"
#include "../Library/Math/Matrix.hpp"
#include "../Library/Math/math.hpp"
#include <vector>

namespace scop
{

class Transform : public Component
{
	using Super = Component;

public:
	Transform();
	~Transform();

	virtual void Init() override;
	virtual void Update() override;

	void UpdateTransform();

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

	// World
	mymath::Vec3 GetScale() { return _scale; }
	void SetScale(const mymath::Vec3& scale);
	mymath::Vec3 GetRotation() { return _rotation; }
	void SetRotation(const mymath::Vec3& rotation);


	mymath::Vec3 GetPosition() { return _position; }
	void SetPosition(const mymath::Vec3& position);

	mymath::Mat4 GetWorldMatrix() { return _matWorld; }

	//
	bool HasParent() { return _parent != nullptr; }
	std::shared_ptr<Transform> GetParent() { return _parent; }

	void SetParent(std::shared_ptr<Transform> parent) { _parent = parent; }

	const std::vector<std::shared_ptr<Transform>>& GetChildren() { return _children; }
	void AddChild(std::shared_ptr<Transform> child) { _children.push_back(child); }

private:
	mymath::Vec3 _localScale{ 1.f, 1.f, 1.f };
	mymath::Vec3 _localRotation{};
	mymath::Vec3 _localPosition{};

	mymath::Mat4 _matLocal{ 1.0f };
	mymath::Mat4 _matWorld{ 1.0f };

	// Cache
	mymath::Vec3 _scale{ 1.f, 1.f, 1.f };
	mymath::Vec3 _rotation{};
	mymath::Vec3 _position{};

	mymath::Vec3 _right{ 1.f, 0.f, 0.f };
	mymath::Vec3 _up{0.0f, -1.0f, 0.0f};
	mymath::Vec3 _look{0.0f, 0.0f, 1.0f};

private:
	std::shared_ptr<Transform> _parent;
	std::vector<std::shared_ptr<Transform>> _children;
};
}