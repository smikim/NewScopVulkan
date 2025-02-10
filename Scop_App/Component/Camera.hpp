#pragma once

#include "Component.hpp"

#include "../Library/Math/Vector.hpp"
#include "../Library/Math/Matrix.hpp"
#include "../Library/Math/math.hpp"

namespace vks
{
	class VulkanRenderer;
}

enum class PROJECTION_TYPE
{
	PERSPECTIVE, // 원근 투영
	ORTHOGRAPHIC, // 직교 투영
};

class Camera : public Component
{
public:
	Camera(vks::VulkanRenderer* renderer);
	~Camera();

	virtual void FinalUpdate() override;

	mymath::Mat4 get_ViewMat() const { return S_MatView; }
	mymath::Mat4 get_ProjMat() const { return S_MatProjection; }

	void Render();

private:
	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	float _near = 1.f;
	float _far = 1000.f;
	float _fov = M_PI / 4.f;
	float _scale = 1.f;

	mymath::Mat4 _matView = {};
	mymath::Mat4 _matProjection = {};

public:
	// TEMP
	static mymath::Mat4 S_MatView;
	static mymath::Mat4 S_MatProjection;
};
	


