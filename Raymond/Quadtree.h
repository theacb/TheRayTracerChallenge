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
// AABB
//
// ------------------------------------------------------------------------
// Declaration
// ------------------------------------------------------------------------

class AABB2D
{
public:
	AABB2D();
	AABB2D(Tuple northeast, Tuple southwest);
	AABB2D(Tuple center, double radius);
	AABB2D(const AABB2D & src);
	~AABB2D();

	Tuple ne_corner, sw_corner, center, half;
	double width, height;

	[[nodiscard]] bool intersects_aabb(AABB2D box) const;
	[[nodiscard]] bool contains_point(Tuple point) const;

};

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
	QuadNode(Tuple position, T data);
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
= default;

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
	QuadBranch(const AABB2D bounds, int max_depth);
	QuadBranch(const AABB2D bounds, int max_depth, int depth);
	~QuadBranch();

	void insert(std::shared_ptr<QuadNode<T>> node);
	std::shared_ptr<QuadNode<T>> closest_point_to_point(Tuple point);
	std::vector<std::shared_ptr<QuadNode<T>>> query_range(const Tuple northeast, const Tuple southwest);
	std::vector<std::shared_ptr<QuadNode<T>>> query_range(const AABB2D range);
	bool contains_point(const Tuple point);

	std::vector<std::shared_ptr<QuadNode<T>>> get_child_nodes();
	std::vector<std::shared_ptr<QuadNode<T>>> get_all_nodes();

private:
	// Bounding Box Points
	AABB2D qb_bounds_;

	//depth
	int qb_depth_, qb_max_depth_;

	// Children:
	// - Branches - "br" = branch; "ne, nw, sw, se" = cardinal directions (Northeast, Northwest, Southeast, Southwest)
	std::shared_ptr<QuadBranch<T>> qb_br_ne_, qb_br_nw_, qb_br_sw_, qb_br_se_;
	// - Leaves - "lf" = leaf
	std::shared_ptr<QuadNode<T>> qb_lf_ne_, qb_lf_nw_, qb_lf_sw_, qb_lf_se_;

	void qb_insert_quarter_(std::shared_ptr<QuadNode<T>> node, const Quadrant quadrant, const AABB2D branch_AABB);
	bool qb_is_branch_empty_(const Quadrant quadrant);
	bool qb_is_leaf_empty_(const Quadrant quadrant);
	void qb_insert_to_branch_(std::shared_ptr<QuadNode<T>> node, const Quadrant quadrant);
	void qb_attach_leaf_(std::shared_ptr<QuadNode<T>> node, const Quadrant quadrant);
	void qb_null_branch_(const Quadrant quadrant);
	void qb_null_leaf_(const Quadrant quadrant);
	void qb_leaf_to_branch_(const Quadrant quadrant);
	void qb_nest_branch_(const AABB2D branch_AABB, const Quadrant quadrant);

};

// ------------------------------------------------------------------------
// Definition
// ------------------------------------------------------------------------
// Constructors
// ------------------------------------------------------------------------

template<typename T>
inline QuadBranch<T>::QuadBranch() : QuadBranch<T>(AABB2D(Tuple::Point2D(0.0, 0.0), Tuple::Point2D(1.0, 1.0)), 0, 7)
{
}

template<typename T>
inline QuadBranch<T>::QuadBranch(const Tuple northeast, const Tuple southwest, int max_depth) : QuadBranch<T>(AABB2D(northeast, southwest), max_depth, 0)
{
}

template<typename T>
inline QuadBranch<T>::QuadBranch(const Tuple northeast, const Tuple southwest, int max_depth, int depth) : QuadBranch<T>(AABB2D(northeast, southwest), max_depth, depth)
{
}

template<typename T>
inline QuadBranch<T>::QuadBranch(const AABB2D bounds, int max_depth) : QuadBranch<T>(bounds, max_depth, 0)
{
}

template<typename T>
inline QuadBranch<T>::QuadBranch(const AABB2D bounds, int max_depth, int depth)
{
	this->qb_bounds_ = bounds;

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
	if (node == nullptr)
	{
		return;
	}

	// Check if the current quad can contain this point
	if (!this->contains_point(node->pos))
	{
		return;
	}

	Tuple tl = this->qb_bounds_.ne_corner;
	Tuple br = this->qb_bounds_.sw_corner;
	
	// Split into the West half if true, or the East if false
	if ((tl.x + br.x) / 2 >= node->pos.x)
	{
		// Split into the Northwest quarter if true, Southwest quarter if false
		if ((tl.y + br.y) / 2 >= node->pos.y)
		{
			AABB2D bounds = AABB2D(
				Tuple::Point2D(tl.x, tl.y), // (0, 0) - Solved for ((0, 0), (1, 1))
				Tuple::Point2D((tl.x + br.x) / 2, (tl.y + br.y) / 2) // (0.5, 0.5)
			);

			this->qb_insert_quarter_(
				node,
				Northwest_qb,
				bounds
			);
			
		}
		// Southwest
		else 
		{
			AABB2D bounds = AABB2D(
				Tuple::Point2D(tl.x, (tl.y + br.y) / 2), // (0, 0.5)
				Tuple::Point2D((tl.x + br.x) / 2, br.y) // (0.5, 1)
			);

			this->qb_insert_quarter_(
				node,
				Southwest_qb,
				bounds
			);
		}
	}
	// East Half
	else
	{
		// Split into the Northeast quarter if true, Southeast quarter if false
		if ((tl.y + br.y) / 2 >= node->pos.y)
		{
			AABB2D bounds = AABB2D(
				Tuple::Point2D((tl.x + br.x) / 2, tl.y), // (0.5, 0.0)
				Tuple::Point2D(br.x, (tl.y + br.y) / 2) // (1, 0.5)
			);

			this->qb_insert_quarter_(
				node,
				Northeast_qb,
				bounds
			);
		}
		// Southwest
		else
		{
			AABB2D bounds = AABB2D(
				Tuple::Point2D((tl.x + br.x) / 2, (tl.y + br.y) / 2), // (0.5, 0.5)
				Tuple::Point2D(br.x, br.y) // (1, 1)
			);

			this->qb_insert_quarter_(
				node,
				Southeast_qb,
				bounds
			);
		}
	}
}

template<typename T>
inline std::shared_ptr<QuadNode<T>> QuadBranch<T>::closest_point_to_point(Tuple point)
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
				return this->qb_br_ne_->closest_point_to_point(point);
			else
				return nullptr;
			
		}
		// Southeast Quarter
		else
		{
			if (this->qb_lf_se_ != nullptr)
				return this->qb_lf_se_;
			else if (this->qb_br_se_ != nullptr)
				return this->qb_br_se_->closest_point_to_point(point);
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
				return this->qb_br_nw_->closest_point_to_point(point);
			else
				return nullptr;
			
		}
		// Southwest Quarter
		else
		{
			if (this->qb_lf_sw_ != nullptr)
				return this->qb_lf_sw_;
			else if (this->qb_br_sw_ != nullptr)
				return this->qb_br_sw_->closest_point_to_point(point);
			else
				return nullptr;
		}
	}
}

template<typename T>
inline std::vector<std::shared_ptr<QuadNode<T>>> QuadBranch<T>::query_range(const Tuple northeast, const Tuple southwest)
{
	return this->query_range(AABB2D(northeast, southwest));
}

template<typename T>
inline std::vector<std::shared_ptr<QuadNode<T>>> QuadBranch<T>::query_range(const AABB2D range)
{
	// Declare return vector
	std::vector<std::shared_ptr<QuadNode<T>>> vec = std::vector<std::shared_ptr<QuadNode<T>>>();

	// declare variables
	std::vector<std::shared_ptr<QuadNode<T>>> vec_ne, vec_se, vec_nw, vec_sw;

	// Abort if the quad is not within the range
	if (!this->qb_bounds_.intersects_aabb(range))
	{
		// Empty vector
		return vec;
	}

	// RESERVE
	// Inverted depth
	int d = this->qb_max_depth_ - this->qb_depth_;
	vec.reserve(((d * d) * 4) + 4);

	// Append Children if they exist
	if (this->qb_lf_ne_ != nullptr)
	{
		Tuple point = this->qb_lf_ne_->pos;

		if (range.contains_point(point))
		{
			vec.push_back(this->qb_lf_ne_);
		}
	}
	if (this->qb_lf_nw_ != nullptr)
	{
		Tuple point = this->qb_lf_nw_->pos;

		if (range.contains_point(point))
		{
			vec.push_back(this->qb_lf_nw_);
		}
	}
	if (this->qb_lf_sw_ != nullptr)
	{
		Tuple point = this->qb_lf_sw_->pos;

		if (range.contains_point(point))
		{
			vec.push_back(this->qb_lf_sw_);
		}
	}
	if (this->qb_lf_se_ != nullptr)
	{
		Tuple point = this->qb_lf_se_->pos;

		if (range.contains_point(point))
		{
			vec.push_back(this->qb_lf_se_);
		}
	}

	// Append the children of branches recursively
	if (this->qb_br_ne_ != nullptr)
	{
		vec_ne = this->qb_br_ne_->query_range(range);
		vec.insert(vec.end(), vec_ne.begin(), vec_ne.end());
	}
	if (this->qb_br_se_ != nullptr)
	{
		vec_se = this->qb_br_se_->query_range(range);
		vec.insert(vec.end(), vec_se.begin(), vec_se.end());
	}
	if (this->qb_br_nw_ != nullptr)
	{
		vec_nw = this->qb_br_nw_->query_range(range);
		vec.insert(vec.end(), vec_nw.begin(), vec_nw.end());
	}
	if (this->qb_br_sw_ != nullptr)
	{
		vec_sw = this->qb_br_sw_->query_range(range);
		vec.insert(vec.end(), vec_sw.begin(), vec_sw.end());
	}

	return vec;
}

template<typename T>
inline bool QuadBranch<T>::contains_point(const Tuple point)
{
	return this->qb_bounds_.contains_point(point);
}

template<typename T>
inline std::vector<std::shared_ptr<QuadNode<T>>> QuadBranch<T>::get_child_nodes()
{

	// Directly assign children to indices
	std::vector<std::shared_ptr<QuadNode<T>>> vec;
	vec.reserve(4);
	if (this->qb_lf_ne_ != nullptr)
	{
		vec.push_back(this->qb_lf_ne_);
	}
	if (this->qb_lf_nw_ != nullptr)
	{
		vec.push_back(this->qb_lf_nw_);
	}
	if (this->qb_lf_sw_ != nullptr)
	{
		vec.push_back(this->qb_lf_sw_);
	}
	if (this->qb_lf_se_ != nullptr)
	{
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

	// declare variables
	std::vector<std::shared_ptr<QuadNode<T>>> vec_ne, vec_se, vec_nw, vec_sw, children;

	// get values from branches
	if (this->qb_br_ne_ != nullptr)
	{
		vec_ne = this->qb_br_ne_->get_all_nodes();
		vec.insert(vec.end(), vec_ne.begin(), vec_ne.end());
	}
	if (this->qb_br_se_ != nullptr)
	{
		vec_se = this->qb_br_se_->get_all_nodes();
		vec.insert(vec.end(), vec_se.begin(), vec_se.end());
	}
	if (this->qb_br_nw_ != nullptr)
	{
		vec_nw = this->qb_br_nw_->get_all_nodes();
		vec.insert(vec.end(), vec_nw.begin(), vec_nw.end());
	}
	if (this->qb_br_sw_ != nullptr)
	{
		vec_sw = this->qb_br_sw_->get_all_nodes();
		vec.insert(vec.end(), vec_sw.begin(), vec_sw.end());
	}

	// Get direct children as well
	children = this->get_child_nodes();
	if (children.size() > 0)
		vec.insert(vec.end(), children.begin(), children.end());

	return vec;
}

template<typename T>
inline void QuadBranch<T>::qb_insert_quarter_(std::shared_ptr<QuadNode<T>> node, const Quadrant quadrant, const AABB2D branch_AABB)
{
	// empty Branches start by filling leaves, then create new branches if the node comes into conflict with the leaf
	if (this->qb_is_branch_empty_(quadrant) && this->qb_is_leaf_empty_(quadrant))
	{
		this->qb_attach_leaf_(node, quadrant);
		return;
	}
	else if (this->qb_depth_ <= this->qb_max_depth_)
	{
		// If the quad is unoccupied, fill it with a new quad
		if (this->qb_is_branch_empty_(quadrant))
		{
			this->qb_nest_branch_(branch_AABB, quadrant);
		}

		// If the leaf was occupied, move it's contents to the branch and declare it NULL
		if (!this->qb_is_leaf_empty_(quadrant))
		{
			this->qb_leaf_to_branch_(quadrant);
		}

		// Recursively insert 
		this->qb_insert_to_branch_(node, quadrant);
		return;
	}
	else
	{
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
	switch (quadrant)
	{
	case Northeast_qb:
		this->qb_br_ne_->insert(node);
		break;

	case Northwest_qb:
		this->qb_br_nw_->insert(node);
		break;

	case Southeast_qb:
		this->qb_br_se_->insert(node);
		break;

	case Southwest_qb:
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
	switch (quadrant)
	{
	case Northeast_qb:
		this->qb_br_ne_->insert(this->qb_lf_ne_);
		this->qb_lf_ne_ = nullptr;
		break;

	case Northwest_qb:
		this->qb_br_nw_->insert(this->qb_lf_nw_);
		this->qb_lf_nw_ = nullptr;
		break;

	case Southeast_qb:
		this->qb_br_se_->insert(this->qb_lf_se_);
		this->qb_lf_se_ = nullptr;
		break;

	case Southwest_qb:
		this->qb_br_sw_->insert(this->qb_lf_sw_);
		this->qb_lf_sw_ = nullptr;
		break;
	}
}

template<typename T>
inline void QuadBranch<T>::qb_nest_branch_(const AABB2D branch_AABB, const Quadrant quadrant)
{
	auto br = std::make_shared<QuadBranch<T>>(branch_AABB, this->qb_max_depth_, this->qb_depth_ + 1);

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

#endif
