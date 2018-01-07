#pragma once

#include <Eigen/Eigen>


class BlinkStickColor: 
	public Eigen::Vector3f
{
public:
	BlinkStickColor()
		:Eigen::Vector3f(0,0,0) {}

	BlinkStickColor(int r, int g, int b)
		:Eigen::Vector3f((float) r/255.0,(float) g/255.0,(float) b/255.0) {}

	BlinkStickColor(float r, float g, float b)
		:Eigen::Vector3f(r,g,b) {}

	BlinkStickColor(const Eigen::Vector3f& other)
		:Eigen::Vector3f(other) {}

	BlinkStickColor dim(float brightness) const;
	BlinkStickColor transition(const BlinkStickColor& other, float brightness);

	const int r()
	{
		int t = floor(x() * 255.0 + 0.5 );
		if ( t>255) t=255;
		return t ;
	}

	const int g()
	{
		int t = floor(y() * 255.0 + 0.5 );
		if ( t>255) t=255;
		return t ;
	}

	const int b()
	{
		int t = floor(z() * 255.0 + 0.5 );
		if ( t>255) t=255;
		return t ;
	}

	static const BlinkStickColor red;
	static const BlinkStickColor orange;
	static const BlinkStickColor blue;
	static const BlinkStickColor green;
	static const BlinkStickColor yellow;
	static const BlinkStickColor magenta;
	static const BlinkStickColor cyan;
	static const BlinkStickColor black;
	static const BlinkStickColor white;
};

inline BlinkStickColor BlinkStickColor::dim(float brightness) const
{
	Eigen::Vector3f c = *this;

	c = c*brightness;
	return BlinkStickColor(c);
}

inline BlinkStickColor BlinkStickColor::transition(const BlinkStickColor& other, float brightness)
{
	Eigen::Vector3f this_color = *this;
	Eigen::Vector3f other_color = other;
	Eigen::Vector3f gradient = other_color - this_color;

	return BlinkStickColor(this_color + gradient*brightness);



}
