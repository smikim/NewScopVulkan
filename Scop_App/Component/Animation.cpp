#include "Animation.hpp"
#include <string>

namespace humanGL
{
	NodeTransform::NodeTransform(Node& node)
		: _node{node}
	{
	}
	
	NodeTransform::~NodeTransform()
	{
	}

	NodeTransform::NodeTransform(const NodeTransform& other)
		: _node(other._node), _translation(other._translation), _scale(other._scale), _rotation(other._rotation), _worldMatrix(other._worldMatrix) {}

	NodeTransform::NodeTransform(NodeTransform&& other) noexcept
		: _node(other._node), _translation(std::move(other._translation)), 
		_scale(std::move(other._scale)), _rotation(std::move(other._rotation)), 
		_worldMatrix(std::move(other._worldMatrix)) 
	{}


	NodeTransform& NodeTransform::operator=(const NodeTransform& other) {
		if (this == &other) {
			return *this;
		}

		_translation = other._translation;
		_scale = other._scale;
		_rotation = other._rotation;
		_worldMatrix = other._worldMatrix;

		return *this;
	}

	NodeTransform& NodeTransform::operator=(NodeTransform&& other) noexcept
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

	Node& NodeTransform::get_node()
	{
		return _node;
	}

	void NodeTransform::set_translation(const mymath::Vec3 translation)
	{
		_translation = translation;
	}

	void NodeTransform::set_rotation(const mymath::Vec3 rotation)
	{
		_rotation = rotation;
	}

	void NodeTransform::set_scale(const mymath::Vec3 scale)
	{
		_scale = scale;
	}

	const mymath::Vec3& NodeTransform::get_translation() const
	{
		return _translation;
	}

	const mymath::Vec3& NodeTransform::get_rotation() const
	{
		return _rotation;
	}

	const mymath::Vec3& NodeTransform::get_scale() const
	{
		return _scale;
	}

	mymath::Mat4 NodeTransform::get_matrix() const
	{
		return  (mymath::translate(mymath::Mat4{ 1.0f }, _translation) *
			(mymath::rotate(mymath::Mat4{ 1.0f }, _rotation._y, mymath::Vec3(0.0f, 1.0f, 0.0f)) *
				mymath::rotate(mymath::Mat4{ 1.0f }, _rotation._x, mymath::Vec3(1.0f, 0.0f, 0.0f)) *
				mymath::rotate(mymath::Mat4{ 1.0f }, _rotation._z, mymath::Vec3(0.0f, 0.0f, 1.0f))) *
				mymath::scale(mymath::Mat4{ 1.0f }, _scale));
	}

	mymath::Mat4 NodeTransform::get_world_matrix()
	{
		//update_world_transform();

		_worldMatrix = get_matrix();
		return _worldMatrix;
	}

	void NodeTransform::update_world_transform()
	{

		_worldMatrix = get_matrix();

		auto parent = _node.get_parent();

		if (parent)
		{
			auto& transform = parent->get_transform();
			_worldMatrix = transform.get_world_matrix() * _worldMatrix;
		}

	}

	Node::Node(const std::string& name)
		: _name{ name }, _transform{ *this }
	{
	}


	Node::~Node()
	{
	}


	Node::Node(Node&& other) noexcept
		: _name(std::move(other._name)), _transform(*this), _parent(other._parent), _children(std::move(other._children))
	{
		_transform = NodeTransform(*this);
	}


	Node& Node::operator=(Node&& other) noexcept
	{
		if (this != &other)
		{
			_name = std::move(other._name);
			_parent = other._parent;
			_children = std::move(other._children);

			_transform = NodeTransform(*this);
			_transform = std::move(other._transform);
		}
		return *this;
	}

	const std::string& Node::get_name() const
	{
		return _name;
	}

	void Node::set_name(const std::string& name)
	{
		_name = name;
	}

	void Node::set_parent(Node* parent)
	{
		_parent = parent;
		if (parent) {
			parent->add_child(std::unique_ptr<Node>(this));
		}
	}

	Node* Node::get_parent() const
	{
		return _parent;
	}

	void Node::add_child(std::unique_ptr<Node> child)
	{
		std::cout << "add_child" << std::endl;
		std::cout << child.get() << std::endl;
		child->_parent = this;
		_children.push_back(std::move(child));
	}

	const std::vector<std::unique_ptr<Node>>& Node::get_children() const
	{
		return _children;
	}

	

}