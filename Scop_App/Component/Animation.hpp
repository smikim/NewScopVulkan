#pragma once
#include <vector>
#include "../Library/Math/Vector.hpp"
#include "../Library/Math/Matrix.hpp"
#include "../Library/Math/math.hpp"

namespace humanGL
{
	// body parts , body color
	class Node;

	class NodeTransform
	{
	public:
		NodeTransform(Node& node);
		~NodeTransform();

		NodeTransform(const NodeTransform& other);
		NodeTransform(NodeTransform&& other) noexcept;

		NodeTransform& operator=(const NodeTransform& other);
		NodeTransform& operator=(NodeTransform&& other) noexcept;

		Node& get_node();

		//void set_translation(const glm::vec3& translation);
		void set_translation(const mymath::Vec3 translation);

		void set_rotation(const mymath::Vec3 rotation);

		void set_scale(const mymath::Vec3 scale);

		const  mymath::Vec3& get_translation() const;

		const  mymath::Vec3& get_rotation() const;

		const  mymath::Vec3& get_scale() const;

		//void set_matrix(const glm::mat4& matrix);

		mymath::Mat4 get_matrix() const;

		mymath::Mat4 get_world_matrix();

		/**
		 * @brief Marks the world transform invalid if any of
		 *        the local transform are changed or the parent
		 *        world transform has changed.
		 */
		//void invalidate_world_matrix();

	private:
		Node& _node;
		
		mymath::Vec3 _translation{};
		mymath::Vec3 _scale{ 1.f, 1.f, 1.f };
		mymath::Vec3 _rotation{};
		
		mymath::Mat4 _worldMatrix{ 1.0f };

		//bool update_world_matrix = false;

		void update_world_transform();
	};
	

class Node
{
public:
	Node() : _transform{ *this } { ; }
	Node(const std::string& name);

	Node(const Node& other) = delete;
	// 이동 생성자
	Node(Node&& other) noexcept;
	
	Node& operator=(const Node& other) = delete;
	Node& operator=(Node&& other) noexcept;

	~Node();

	const std::string& get_name() const;
	void set_name(const std::string& name);

	NodeTransform& get_transform() { return _transform; };
	const NodeTransform& get_transform() const
	{
		return _transform;
	}

	void set_parent(Node* parent);

	Node* get_parent() const;

	void add_child(std::unique_ptr<Node> child);

	const std::vector<std::unique_ptr<Node>>& get_children() const;

private:
	std::string _name;

	NodeTransform _transform;
	
	Node* _parent{ nullptr };

	std::vector<std::unique_ptr<Node>> _children;
};

}