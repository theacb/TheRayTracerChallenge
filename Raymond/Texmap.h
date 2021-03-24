#ifndef H_RAYMOND_TEXMAP
#define H_RAYMOND_TEXMAP

#include "IxComps.h"
#include "Color.h"
#include "Utilities.h"

enum MappingSpace {WorldSpace, ObjectSpace};
enum CompositeMode { CompBlend, CompAdd, CompMultiply, CompDivide, CompSubtract, CompOverlay, CompScreen
};

class TexMap
{
public:
	TexMap();
	~TexMap();

	void set_transform(Matrix4 m);
	Matrix4 get_transform() const;
	Matrix4 get_inverse_transform() const;

	void set_mapping_space(MappingSpace space);

	// Methods
	virtual Color local_sample_at(const IxComps & comps) const = 0;
	Color sample_at(const IxComps & comps) const;
	Color sample_at_point(const Tuple & point) const;

private:
	//properties
	Matrix4 transform_;
	Matrix4 inverse_transform_;
	MappingSpace mapping_space_;
};

class TestMap :
	public TexMap
{
public:
	TestMap();
	~TestMap();

	// Methods
	virtual Color local_sample_at(const IxComps & comps) const override;
};

class StripeMap :
	public TexMap
{
public:
	StripeMap();
	StripeMap(std::shared_ptr<TexMap> a, std::shared_ptr<TexMap> b);
	StripeMap(Color a, Color b);
	~StripeMap();

	// Methods
	virtual Color local_sample_at(const IxComps & comps) const override;

	// Properties
	std::shared_ptr<TexMap> a, b;
};

class SolidColorMap :
	public TexMap
{
public:
	SolidColorMap();
	SolidColorMap(Color col);
	~SolidColorMap();

	// Methods
	virtual Color local_sample_at(const IxComps & comps) const override;

	// Properties
	Color col;
};

class GradientMap :
	public TexMap
{
public:
	GradientMap();
	GradientMap(std::shared_ptr<TexMap> a, std::shared_ptr<TexMap> b);
	GradientMap(Color a, Color b);
	~GradientMap();

	// Methods
	virtual Color local_sample_at(const IxComps & comps) const override;

	// Properties
	std::shared_ptr<TexMap> a, b;
	bool clamp_fraction;
};

class RingMap :
	public TexMap
{
public:
	RingMap();
	RingMap(std::shared_ptr<TexMap> a, std::shared_ptr<TexMap> b);
	RingMap(Color a, Color b);
	~RingMap();

	// Methods
	virtual Color local_sample_at(const IxComps & comps) const override;

	// Properties
	std::shared_ptr<TexMap> a, b;
};

class CheckerMap :
	public TexMap
{
public:
	CheckerMap();
	CheckerMap(std::shared_ptr<TexMap> a, std::shared_ptr<TexMap> b);
	CheckerMap(Color a, Color b);
	~CheckerMap();

	// Methods
	virtual Color local_sample_at(const IxComps & comps) const override;

	// Properties
	std::shared_ptr<TexMap> a, b;
};

class CompositeMap :
	public TexMap
{
public:
	CompositeMap();
	CompositeMap(std::shared_ptr<TexMap> a, std::shared_ptr<TexMap> b);
	CompositeMap(std::shared_ptr<TexMap> a, std::shared_ptr<TexMap> b, CompositeMode mode);
	CompositeMap(std::shared_ptr<TexMap> a, std::shared_ptr<TexMap> b, CompositeMode mode, double factor);
	CompositeMap(Color a, Color b);
	CompositeMap(Color a, Color b, CompositeMode mode);
	CompositeMap(Color a, Color b, CompositeMode mode, double factor);
	~CompositeMap();

	// Methods
	virtual Color local_sample_at(const IxComps & comps) const override;

	// Properties
	CompositeMode composite_mode;
	std::shared_ptr<TexMap> a, b;
	double factor;
};

#endif
