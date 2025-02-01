#pragma once
#include <vector>
#include "../Library/Math/Vector.hpp"
#include "../Library/Math/Matrix.hpp"
#include "../Library/Math/math.hpp"

namespace humanGL
{
	// body parts , body color
	template<typename VertexType>
	class Node;

	template<typename VertexType>
	class NodeTransform
	{
	public:
		NodeTransform(Node<VertexType>& node);
		~NodeTransform();

		NodeTransform(const NodeTransform& other);
		NodeTransform(NodeTransform&& other) noexcept;

		NodeTransform& operator=(const NodeTransform& other);
		NodeTransform& operator=(NodeTransform&& other) noexcept;

		Node<VertexType>& get_node();

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
		Node<VertexType>& _node;
		
		mymath::Vec3 _translation{};
		mymath::Vec3 _scale{ 1.f, 1.f, 1.f };
		mymath::Vec3 _rotation{};
		
		mymath::Mat4 _worldMatrix{ 1.0f };

		//bool update_world_matrix = false;

		void update_world_transform();
	};
	
template<typename VertexType>
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

	NodeTransform<VertexType>& get_transform() { return _transform; };
	const NodeTransform<VertexType>& get_transform() const
	{
		return _transform;
	}

	void set_parent(Node<VertexType>* parent);

	Node<VertexType>* get_parent() const;

	void add_child(std::unique_ptr<Node<VertexType>> child);

	const std::vector<std::unique_ptr<Node<VertexType>>>& get_children() const;

	// 노드의 개수를 반환하는 함수
	size_t get_node_count() const;

	void set_vertices(std::vector<VertexType>& vertices);
	void set_indices(std::vector<uint32_t>& indices);
	std::vector<VertexType> get_vertices() const;
	std::vector<uint32_t> get_indices() const;

private:
	std::string _name;

	NodeTransform<VertexType> _transform;
	
	Node* _parent{ nullptr };

	std::vector<std::unique_ptr<Node<VertexType>>> _children;

	std::vector<VertexType> _Vertices;

	std::vector<uint32_t> _Indices;
	// 재귀적으로 자식 노드의 개수를 계산하는 헬퍼 함수
	size_t get_node_count_recursive() const;
};

template<typename VertexType>
NodeTransform<VertexType>::NodeTransform(Node<VertexType>& node)
	: _node{ node }
{
}

template<typename VertexType>
NodeTransform<VertexType>::~NodeTransform()
{
}

template<typename VertexType>
NodeTransform<VertexType>::NodeTransform(const NodeTransform<VertexType>& other)
	: _node(other._node), _translation(other._translation), _scale(other._scale), _rotation(other._rotation), _worldMatrix(other._worldMatrix) {}

template<typename VertexType>
NodeTransform<VertexType>::NodeTransform(NodeTransform<VertexType>&& other) noexcept
	: _node(other._node), _translation(std::move(other._translation)),
	_scale(std::move(other._scale)), _rotation(std::move(other._rotation)),
	_worldMatrix(std::move(other._worldMatrix))
{}

template<typename VertexType>
NodeTransform<VertexType>& NodeTransform<VertexType>::operator=(const NodeTransform<VertexType>& other) {
	if (this == &other) {
		return *this;
	}

	_translation = other._translation;
	_scale = other._scale;
	_rotation = other._rotation;
	_worldMatrix = other._worldMatrix;

	return *this;
}

template<typename VertexType>
NodeTransform<VertexType>& NodeTransform<VertexType>::operator=(NodeTransform<VertexType>&& other) noexcept
{
	if (this != &other)
	{
		_translation = std::move(other._translation);
		_scale = std::move(other._scale);
		_rotation = std::move(other._rotation);
		_worldMatrix = std::move(other._worldMatrix);
	}
	return *this;
}

template<typename VertexType>
Node<VertexType>& NodeTransform<VertexType>::get_node()
{
	return _node;
}

template<typename VertexType>
void NodeTransform<VertexType>::set_translation(const mymath::Vec3 translation)
{
	_translation = translation;
}

template<typename VertexType>
void NodeTransform<VertexType>::set_rotation(const mymath::Vec3 rotation)
{
	_rotation = rotation;
}

template<typename VertexType>
void NodeTransform<VertexType>::set_scale(const mymath::Vec3 scale)
{
	_scale = scale;
}

template<typename VertexType>
const mymath::Vec3& NodeTransform<VertexType>::get_translation() const
{
	return _translation;
}

template<typename VertexType>
const mymath::Vec3& NodeTransform<VertexType>::get_rotation() const
{
	return _rotation;
}

template<typename VertexType>
const mymath::Vec3& NodeTransform<VertexType>::get_scale() const
{
	return _scale;
}

template<typename VertexType>
mymath::Mat4 NodeTransform<VertexType>::get_matrix() const
{
	return  (mymath::translate(mymath::Mat4{ 1.0f }, _translation) *
		(mymath::rotate(mymath::Mat4{ 1.0f }, _rotation._y, mymath::Vec3(0.0f, 1.0f, 0.0f)) *
			mymath::rotate(mymath::Mat4{ 1.0f }, _rotation._x, mymath::Vec3(1.0f, 0.0f, 0.0f)) *
			mymath::rotate(mymath::Mat4{ 1.0f }, _rotation._z, mymath::Vec3(0.0f, 0.0f, 1.0f))) *
		mymath::scale(mymath::Mat4{ 1.0f }, _scale));
}

template<typename VertexType>
mymath::Mat4 NodeTransform<VertexType>::get_world_matrix()
{
	//update_world_transform();

	_worldMatrix = get_matrix();
	return _worldMatrix;
}

template<typename VertexType>
void NodeTransform<VertexType>::update_world_transform()
{

	_worldMatrix = get_matrix();

	auto parent = _node.get_parent();

	if (parent)
	{
		auto& transform = parent->get_transform();
		_worldMatrix = transform.get_world_matrix() * _worldMatrix;
	}

}

template<typename VertexType>
Node<VertexType>::Node(const std::string& name)
	: _name{ name }, _transform{ *this }
{
}

template<typename VertexType>
Node<VertexType>::~Node()
{
}


template<typename VertexType>
Node<VertexType>::Node(Node<VertexType>&& other) noexcept
	: _name(std::move(other._name)), _transform(*this), _parent(other._parent), _children(std::move(other._children)),
	  _Vertices(std::move(other._Vertices)), _Indices(std::move(other._Indices))
{
	_transform = NodeTransform(*this);
}

template<typename VertexType>
Node<VertexType>& Node<VertexType>::operator=(Node<VertexType>&& other) noexcept
{
	if (this != &other)
	{
		_name = std::move(other._name);
		_parent = other._parent;
		_children = std::move(other._children);
		_Vertices = std::move(other._Vertices);
		_Indices = std::move(other._Indices);

		_transform = NodeTransform(*this);
		_transform = std::move(other._transform);
	}
	return *this;
}

template<typename VertexType>
const std::string& Node<VertexType>::get_name() const
{
	return _name;
}

template<typename VertexType>
void Node<VertexType>::set_name(const std::string& name)
{
	_name = name;
}

template<typename VertexType>
void Node<VertexType>::set_parent(Node<VertexType>* parent)
{
	_parent = parent;
	if (parent) {
		parent->add_child(std::unique_ptr<Node<VertexType>>(this));
	}
}

template<typename VertexType>
Node<VertexType>* Node<VertexType>::get_parent() const
{
	return _parent;
}

template<typename VertexType>
void Node<VertexType>::add_child(std::unique_ptr<Node<VertexType>> child)
{
	std::cout << "add_child" << std::endl;
	std::cout << child.get() << std::endl;
	child->_parent = this;
	_children.push_back(std::move(child));
}

template<typename VertexType>
const std::vector<std::unique_ptr<Node<VertexType>>>& Node<VertexType>::get_children() const
{
	return _children;
}

template<typename VertexType>
size_t Node<VertexType>::get_node_count() const
{
	return get_node_count_recursive();
}

template<typename VertexType>
inline void Node<VertexType>::set_vertices(std::vector<VertexType>& vertices)
{
	_Vertices = vertices;
}

template<typename VertexType>
inline void Node<VertexType>::set_indices(std::vector<uint32_t>& indices)
{
	_Indices = indices;
}

template<typename VertexType>
inline std::vector<VertexType> Node<VertexType>::get_vertices() const
{
	return _Vertices;
}

template<typename VertexType>
inline std::vector<uint32_t> Node<VertexType>::get_indices() const
{
	return _Indices;
}

template<typename VertexType>
size_t Node<VertexType>::get_node_count_recursive() const
{
	size_t count = 1; // 현재 노드

	for (const auto& child : _children)
	{
		count += child->get_node_count_recursive();
	}

	return count;
}

}