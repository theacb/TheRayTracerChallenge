#ifndef H_RAYMOND_QUADTREE
#define H_RAYMOND_QUADTREE

#include "Tuple.h"
#include <vector>

enum Quadrant { Southeast_qb, Southwest_qb, Northwest_qb, Northeast_qb };

// This Quadtree implementation is a modified version of one from Geeks For Geeks
// https://www.geeksforgeeks.org/quad-tree/
// https://jimkang.com/quadtreevis/

// ------------------------------------------------------------------------
//
// QuadNode
//
// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------

template <typename T>
struct QuadNode
{
	QuadNode();
	QuadNode(const Tuple position, T data);
	~QuadNode();

	T get_data();
	Tuple get_position();

	// Properties
	Tuple pos;
	T data;

};

// ------------------------------------------------------------------------
// Definition
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

template<typename T>
inline QuadNode<T>::QuadNode() : QuadNode(Tuple::Point2D(0.0, 0.0), T())
{
}

template<typename T>
inline QuadNode<T>::QuadNode(const Tuple position, T data)
{
	this->pos = position;
	this->data = data;
}

template<typename T>
inline QuadNode<T>::~QuadNode()
{
}

template<typename T>
inline T QuadNode<T>::get_data()
{
	return this->data;
}

template<typename T>
inline Tuple QuadNode<T>::get_position()
{
	return this->pos;
}

// ------------------------------------------------------------------------
//
// QuadBranch
//
// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------

template <typename T>
class QuadBranch
{
public:
	QuadBranch();
	QuadBranch(const Tuple northeast, const Tuple southwest, int max_depth);
	QuadBranch(const Tuple northeast, const Tuple southwest, int max_depth, int depth);
	~QuadBranch();

	void insert(std::shared_ptr<QuadNode<T>> node);
	std::shared_ptr<QuadNode<T>> search(Tuple point);
	bool contains_point(Tuple point);

	std::vector<std::shared_ptr<QuadNode<T>>> get_child_nodes();
	std::vector<std::shared_ptr<QuadNode<T>>> get_all_nodes();

private:
	// Bounding Box Points
	Tuple qb_top_left_, qb_bottom_right_;

	int qb_depth_, qb_max_depth_;

	// Children:
	// - Branches - "br" = branch; "ne, nw, sw, se" = cardinal directions (Northeast, Northwest, Southeast, Southwest)
	std::shared_ptr<QuadBranch<T>> qb_br_ne_, qb_br_nw_, qb_br_sw_, qb_br_se_;
	// - Leaves - "lf" = leaf
	std::shared_ptr<QuadNode<T>> qb_lf_ne_, qb_lf_nw_, qb_lf_sw_, qb_lf_se_;

	void qb_insert_quarter_(std::shared_ptr<QuadNode<T>> node, const Quadrant quadrant, const Tuple top_left, const Tuple bottom_right);
	bool qb_is_branch_empty_(const Quadrant quadrant);
	bool qb_is_leaf_empty_(const Quadrant quadrant);
	void qb_insert_to_branch_(std::shared_ptr<QuadNode<T>> node, const Quadrant quadrant);
	void qb_attach_leaf_(std::shared_ptr<QuadNode<T>> node, const Quadrant quadrant);
	void qb_null_branch_(const Quadrant quadrant);
	void qb_null_leaf_(const Quadrant quadrant);
	void qb_leaf_to_branch_(const Quadrant quadrant);
	void qb_nest_branch_(const Tuple top_left, const Tuple bottom_right, const Quadrant quadrant);
	void qb_print_leaf_name_(const Quadrant quadrant);

};

// ------------------------------------------------------------------------
// Definition
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

template<typename T>
inline QuadBranch<T>::QuadBranch() : QuadBranch<T>(Tuple::Point2D(0.0, 0.0), Tuple::Point2D(1.0, 1.0), 0, 7)
{
}

template<typename T>
inline QuadBranch<T>::QuadBranch(const Tuple northeast, const Tuple southwest, int max_depth) : QuadBranch<T>(northeast, southwest, max_depth, 0)
{
}

template<typename T>
inline QuadBranch<T>::QuadBranch(const Tuple northeast, const Tuple southwest, int max_depth, int depth)
{
	this->qb_top_left_ = northeast;
	this->qb_bottom_right_ = southwest;

	this->qb_depth_ = depth;
	this->qb_max_depth_ = max_depth;

	// Just in case?
	this->qb_br_ne_ = nullptr;
	this->qb_br_nw_ = nullptr;
	this->qb_br_sw_ = nullptr;
	this->qb_br_se_ = nullptr;

	this->qb_lf_ne_ = nullptr;
	this->qb_lf_nw_ = nullptr;
	this->qb_lf_sw_ = nullptr;
	this->qb_lf_se_ = nullptr;
}

template<typename T>
inline QuadBranch<T>::~QuadBranch()
{
}

// ------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------

template<typename T>
inline void QuadBranch<T>::insert(std::shared_ptr<QuadNode<T>> node)
{
	// Print Prefix
	std::string prefix("");
	prefix.insert(0, this->qb_depth_ * 4, ' ');

	std::cout << prefix << "Insert Start - " << node->data.name << " Position: " << node->pos << std::endl;

	Tuple tl = this->qb_top_left_;
	Tuple br = this->qb_bottom_right_;

	if (node == nullptr)
	{
		std::cout << prefix << "Node is nullptr" << std::endl;
		return;
	}

	// Check if the current quad can contain this point
	if (!this->contains_point(node->pos))
	{
		std::cout << prefix << "Point out of Bounds" << std::endl;
		return;
	}

	
	// Split into the West half if true, or the East if false
	if ((tl.x + br.x) / 2 >= node->pos.x)
	{
		// Split into the Northwest quarter if true, Southwest quarter if false
		if ((tl.y + br.y) / 2 >= node->pos.y)
		{
			std::cout << prefix << "Insert NW" << std::endl;
			this->qb_insert_quarter_(
				node,
				Northwest_qb,
				Tuple::Point2D(tl.x, tl.y), // (0, 0) - Solved for ((0, 0), (1, 1))
				Tuple::Point2D((tl.x + br.x) / 2, (tl.y + br.y) / 2) // (0.5, 0.5)
			);
			
		}
		// Southwest
		else 
		{
			std::cout << prefix << "Insert SW" << std::endl;
			this->qb_insert_quarter_(
				node,
				Southwest_qb,
				Tuple::Point2D(tl.x, (tl.y + br.y) / 2), // (0, 0.5)
				Tuple::Point2D((tl.x + br.x) / 2, br.y) // (0.5, 1)
			);
		}
	}
	// East Half
	else
	{
		// Split into the Northeast quarter if true, Southeast quarter if false
		if ((tl.y + br.y) / 2 >= node->pos.y)
		{
			std::cout << prefix << "Insert NE" << std::endl;
			this->qb_insert_quarter_(
				node,
				Northeast_qb,
				Tuple::Point2D((tl.x + br.x) / 2, tl.y), // (0.5, 0.0)
				Tuple::Point2D(br.x, (tl.y + br.y) / 2) // (1, 0.5)
			);
		}
		// Southwest
		else
		{
			std::cout << prefix << "Insert SE" << std::endl;
			this->qb_insert_quarter_(
				node,
				Southeast_qb,
				Tuple::Point2D((tl.x + br.x) / 2, (tl.y + br.y) / 2), // (0.5, 0.5)
				Tuple::Point2D(br.x, br.y) // (1, 1)
			);
		}
	}

	std::cout << prefix << "Insert Complete" << std::endl;
}

template<typename T>
inline std::shared_ptr<QuadNode<T>> QuadBranch<T>::search(Tuple point)
{
	// if the point isn't in the quad, return null
	if (!this->contains_point(point))
		return nullptr;

	// Split into the East half if true, or the West if false
	if ((this->qb_top_left_.x + this->qb_bottom_right_.x) / 2 >= point.x)
	{
		// Split into the Northeast quarter if true, Southeast quarter if false
		if ((this->qb_top_left_.y + this->qb_bottom_right_.y) / 2 >= point.y)
		{
			if (this->qb_lf_ne_ != nullptr)
				return this->qb_lf_ne_;
			else if (this->qb_br_ne_ != nullptr)
				return this->qb_br_ne_->search(point);
			else
				return nullptr;
			
		}
		// Southeast Quarter
		else
		{
			if (this->qb_lf_se_ != nullptr)
				return this->qb_lf_se_;
			else if (this->qb_br_se_ != nullptr)
				return this->qb_br_se_->search(point);
			else
				return nullptr;
			
		}
	}
	// West Half
	else
	{
		// Split into the Northwest quarter if true, Southwest quarter if false
		if ((this->qb_top_left_.y + this->qb_bottom_right_.y) / 2 >= point.y)
		{
			if (this->qb_lf_nw_ != nullptr)
				return this->qb_lf_nw_;
			else if (this->qb_br_nw_ != nullptr)
				return this->qb_br_nw_->search(point);
			else
				return nullptr;
			
		}
		// Southwest Quarter
		else
		{
			if (this->qb_lf_sw_ != nullptr)
				return this->qb_lf_sw_;
			else if (this->qb_br_sw_ != nullptr)
				return this->qb_br_sw_->search(point);
			else
				return nullptr;
		}
	}
}

template<typename T>
inline bool QuadBranch<T>::contains_point(Tuple point)
{
	// Greater and less than are used on each dimension to determine if the point is within the quadtree
	return (
		point.x >= this->qb_top_left_.x &&
		point.x <= this->qb_bottom_right_.x &&
		point.y >= this->qb_top_left_.y &&
		point.y <= this->qb_bottom_right_.y
		);
}

template<typename T>
inline std::vector<std::shared_ptr<QuadNode<T>>> QuadBranch<T>::get_child_nodes()
{
	// Print Prefix
	std::string prefix("|");
	prefix.insert(1, this->qb_depth_ * 4, '-');

	std::cout << prefix << " Get Children: Depth:  " << this->qb_depth_ << std::endl;

	// Directly assign children to indices
	std::vector<std::shared_ptr<QuadNode<T>>> vec;
	vec.reserve(4);
	if (this->qb_lf_ne_ != nullptr)
	{
		std::cout << prefix << " |Get NE leaf node" << std::endl;
		std::cout << prefix << "---- |" << this->qb_lf_ne_->data.name << std::endl;
		vec.push_back(this->qb_lf_ne_);
	}
	if (this->qb_lf_nw_ != nullptr)
	{
		std::cout << prefix << " |Get NW leaf node" << std::endl;
		std::cout << prefix << "---- |" << this->qb_lf_nw_->data.name << std::endl;
		vec.push_back(this->qb_lf_nw_);
	}
	if (this->qb_lf_sw_ != nullptr)
	{
		std::cout << prefix << " |Get SW leaf node" << std::endl;
		std::cout << prefix << "---- |" << this->qb_lf_sw_->data.name << std::endl;
		vec.push_back(this->qb_lf_sw_);
	}
	if (this->qb_lf_se_ != nullptr)
	{
		std::cout << prefix << " |Get SE leaf node" << std::endl;
		std::cout << prefix << "---- |" << this->qb_lf_se_->data.name << std::endl;
		vec.push_back(this->qb_lf_se_);
	}

	return vec;
}

template<typename T>
inline std::vector<std::shared_ptr<QuadNode<T>>> QuadBranch<T>::get_all_nodes()
{
	// Declare return vector
	std::vector<std::shared_ptr<QuadNode<T>>> vec = std::vector<std::shared_ptr<QuadNode<T>>>();
	// Inverted depth
	int d = this->qb_max_depth_ - this->qb_depth_;
	vec.reserve(((d * d) * 4) + 4);

	// Print Prefix
	std::string prefix("|");
	prefix.insert(1, this->qb_depth_ * 4, '-');

	std::cout << prefix << " Get All Nodes: Depth:  " << this->qb_depth_ << std::endl;

	// declare variables
	std::vector<std::shared_ptr<QuadNode<T>>> vec_ne, vec_se, vec_nw, vec_sw, children;

	// get values from branches
	if (this->qb_br_ne_ != nullptr)
	{
		std::cout << prefix << " |Get NE Branch's nodes" << std::endl;
		vec_ne = this->qb_br_ne_->get_all_nodes();
		vec.insert(vec.end(), vec_ne.begin(), vec_ne.end());
		std::cout << prefix << " |Size: " << vec_ne.size() << std::endl;
	}
	if (this->qb_br_se_ != nullptr)
	{
		std::cout << prefix << " |Get SE Branch's nodes" << std::endl;
		vec_se = this->qb_br_se_->get_all_nodes();
		vec.insert(vec.end(), vec_se.begin(), vec_se.end());
		std::cout << prefix << " |Size: " << vec_se.size() << std::endl;
	}
	if (this->qb_br_nw_ != nullptr)
	{
		std::cout << prefix << " |Get NW Branch's nodes" << std::endl;
		vec_nw = this->qb_br_nw_->get_all_nodes();
		vec.insert(vec.end(), vec_nw.begin(), vec_nw.end());
		std::cout << prefix << " |Size: " << vec_nw.size() << std::endl;
	}
	if (this->qb_br_sw_ != nullptr)
	{
		std::cout << prefix << " |Get SW Branch's nodes" << std::endl;
		vec_sw = this->qb_br_sw_->get_all_nodes();
		vec.insert(vec.end(), vec_sw.begin(), vec_sw.end());
		std::cout << prefix << " |Size: " << vec_sw.size() << std::endl;
	}

	// Get direct children as well
	children = this->get_child_nodes();
	if (children.size() > 0)
		std::cout << prefix << " |Leaf Size: " << children.size() << std::endl;
		vec.insert(vec.end(), children.begin(), children.end());

	return vec;
}

template<typename T>
inline void QuadBranch<T>::qb_insert_quarter_(std::shared_ptr<QuadNode<T>> node, const Quadrant quadrant, const Tuple top_left, const Tuple bottom_right)
{
	// Print Prefix
	std::string prefix("");
	prefix.insert(0, this->qb_depth_ * 4, ' ');

	std::cout << prefix << "Insert Quarter - Quadrant: " << quadrant << " (" << top_left << ", " << bottom_right << ")" << std::endl;
	if (this->qb_is_branch_empty_(quadrant) && this->qb_is_leaf_empty_(quadrant))
	{
		std::cout << prefix << "    - Insert Quarter - Empty Quarter" << std::endl;
		this->qb_attach_leaf_(node, quadrant);
		return;
	}
	else if (this->qb_depth_ <= this->qb_max_depth_)
	{
		std::cout << prefix << "    - Insert Quarter - Occupied Quarter" << std::endl;
		// If the quad is unoccupied, fill it with a new quad
		if (this->qb_is_branch_empty_(quadrant))
		{
			std::cout << prefix << "    - Insert Quarter - Insert new Branch" << std::endl;
			this->qb_nest_branch_(top_left, bottom_right, quadrant);
		}

		// If the leaf was occupied, move it's contents to the branch and declare it NULL
		if (!this->qb_is_leaf_empty_(quadrant))
		{
			this->qb_print_leaf_name_(quadrant);
			std::cout << prefix << "    - Insert Quarter - Move LEAF to BRANCH" << std::endl;
			this->qb_leaf_to_branch_(quadrant);
		}

		std::cout << prefix << "    - Insert Quarter - Insert NODE to BRANCH" << std::endl;
		// Recursively insert 
		this->qb_insert_to_branch_(node, quadrant);
		return;
	}
	else
	{
		std::cout << prefix << "    - Insert Quarter - DEPTH CHECK FAILED - Node Lost" << std::endl;
		return;
	}
}

template<typename T>
inline bool QuadBranch<T>::qb_is_branch_empty_(const Quadrant quadrant)
{
	switch (quadrant)
	{
	case Northeast_qb:
		return this->qb_br_ne_ == nullptr;
		break;

	case Northwest_qb:
		return this->qb_br_nw_ == nullptr;
		break;

	case Southeast_qb:
		return this->qb_br_se_ == nullptr;
		break;

	case Southwest_qb:
		return this->qb_br_sw_ == nullptr;
		break;

	}

	return false;
}

template<typename T>
inline bool QuadBranch<T>::qb_is_leaf_empty_(const Quadrant quadrant)
{
	switch (quadrant)
	{
	case Northeast_qb:
		return this->qb_lf_ne_ == nullptr;
		break;

	case Northwest_qb:
		return this->qb_lf_nw_ == nullptr;
		break;

	case Southeast_qb:
		return this->qb_lf_se_ == nullptr;
		break;

	case Southwest_qb:
		return this->qb_lf_sw_ == nullptr;
		break;

	}

	return false;
}

template<typename T>
inline void QuadBranch<T>::qb_insert_to_branch_(std::shared_ptr<QuadNode<T>> node, const Quadrant quadrant)
{
	// Print Prefix
	std::string prefix("");
	prefix.insert(0, this->qb_depth_ * 4, ' ');

	std::cout << prefix << "    * Node To Branch - Quadrant: " << quadrant << std::endl;

	switch (quadrant)
	{
	case Northeast_qb:
		std::cout << prefix << "    *     Northeast" << std::endl;
		this->qb_br_ne_->insert(node);
		break;

	case Northwest_qb:
		std::cout << prefix << "    *     Northwest" << std::endl;
		this->qb_br_nw_->insert(node);
		break;

	case Southeast_qb:
		std::cout << prefix << "    *     Southeast" << std::endl;
		this->qb_br_se_->insert(node);
		break;

	case Southwest_qb:
		std::cout << prefix << "    *     Southwest" << std::endl;
		this->qb_br_sw_->insert(node);
		break;

	}
}

template<typename T>
inline void QuadBranch<T>::qb_attach_leaf_(std::shared_ptr<QuadNode<T>> node, const Quadrant quadrant)
{
	switch (quadrant)
	{
	case Northeast_qb:
		this->qb_lf_ne_ = node;
		break;

	case Northwest_qb:
		this->qb_lf_nw_ = node;
		break;

	case Southeast_qb:
		this->qb_lf_se_ = node;
		break;

	case Southwest_qb:
		this->qb_lf_sw_ = node;
		break;

	}
}

template<typename T>
inline void QuadBranch<T>::qb_null_branch_(const Quadrant quadrant)
{
	switch (quadrant)
	{
	case Northeast_qb:
		this->qb_br_ne_ = nullptr;
		break;

	case Northwest_qb:
		this->qb_br_nw_ = nullptr;
		break;

	case Southeast_qb:
		this->qb_br_se_ = nullptr;
		break;

	case Southwest_qb:
		this->qb_br_sw_ = nullptr;
		break;

	}
}

template<typename T>
inline void QuadBranch<T>::qb_null_leaf_(const Quadrant quadrant)
{
	switch (quadrant)
	{
	case Northeast_qb:
		this->qb_lf_ne_ = nullptr;
		break;

	case Northwest_qb:
		this->qb_lf_nw_ = nullptr;
		break;

	case Southeast_qb:
		this->qb_lf_se_ = nullptr;
		break;

	case Southwest_qb:
		this->qb_lf_sw_ = nullptr;
		break;
	}
}

template<typename T>
inline void QuadBranch<T>::qb_leaf_to_branch_(const Quadrant quadrant)
{
	// Print Prefix
	std::string prefix("");
	prefix.insert(0, this->qb_depth_ * 4, ' ');

	std::cout << prefix << "    * Leaf To Branch - Quadrant: " << quadrant << std::endl;

	switch (quadrant)
	{
	case Northeast_qb:
		std::cout << prefix << "    *     Northeast" << std::endl;
		this->qb_br_ne_->insert(this->qb_lf_ne_);
		this->qb_lf_ne_ = nullptr;
		break;

	case Northwest_qb:
		std::cout << prefix << "    *     Northwest" << std::endl;
		this->qb_br_nw_->insert(this->qb_lf_nw_);
		this->qb_lf_nw_ = nullptr;
		break;

	case Southeast_qb:
		std::cout << prefix << "    *     Southeast" << std::endl;
		this->qb_br_se_->insert(this->qb_lf_se_);
		this->qb_lf_se_ = nullptr;
		break;

	case Southwest_qb:
		std::cout << prefix << "    *     Southwest" << std::endl;
		this->qb_br_sw_->insert(this->qb_lf_sw_);
		this->qb_lf_sw_ = nullptr;
		break;
	}
}

template<typename T>
inline void QuadBranch<T>::qb_nest_branch_(const Tuple top_left, const Tuple bottom_right, const Quadrant quadrant)
{
	auto br = std::make_shared<QuadBranch<T>>(top_left, bottom_right, this->qb_max_depth_, this->qb_depth_ + 1);

	switch (quadrant)
	{
	case Northeast_qb:
		this->qb_br_ne_ = br;
		break;

	case Northwest_qb:
		this->qb_br_nw_ = br;
		break;

	case Southeast_qb:
		this->qb_br_se_ = br;
		break;

	case Southwest_qb:
		this->qb_br_sw_ = br;
		break;

	}
}

template<typename T>
inline void QuadBranch<T>::qb_print_leaf_name_(const Quadrant quadrant)
{
	// Print Prefix
	std::string prefix("");
	prefix.insert(0, this->qb_depth_ * 4, ' ');

	switch (quadrant)
	{
	case Northeast_qb:
		std::cout << prefix << "    * Current Leaf: " << this->qb_lf_ne_->data.name << std::endl;
		break;

	case Northwest_qb:
		std::cout << prefix << "    * Current Leaf: " << this->qb_lf_nw_->data.name << std::endl;
		break;

	case Southeast_qb:
		std::cout << prefix << "    * Current Leaf: " << this->qb_lf_se_->data.name << std::endl;
		break;

	case Southwest_qb:
		std::cout << prefix << "    * Current Leaf: " << this->qb_lf_sw_->data.name << std::endl;
		break;
	}
}

#endif
