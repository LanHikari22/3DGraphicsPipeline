// @author Mohammed Alzakariya
// @file Point.cpp
// Implementation cpp file for the Circle concrete class and operator<</>> overloads

#include "Circle.h"
#include <string>
// need this to calculate radius given origin and an arbitrary point on circle
#include <cmath> 

Circle::Circle(const matrix &pts, int color)
	: Shape(pts[0][0], pts[1][0], pts[2][0], color, 2)
{ 
	// assign origin and radius point. matrix is assumed 3x2 (or bigger), 
	// otherwise this fails
	for (int r=0; r<3; r++)
	{
		for (int c=0; c<2; c++)
		{
			this->pts[r][c] = pts[r][c];
		}
	}
	// default row 3 is 1.0 for now (component 4)
	this->pts[3][0] = 1.0;
	this->pts[3][1] = 1.0;
}

Circle::Circle(const Circle &s)
	: Shape(s)
{ }

Circle::~Circle()
{
	// does nothing, but must be defined
}

Circle& Circle::operator=(const Circle& rhs)
{	
	// Shape data
	assignShapeData(rhs);
	
	return *this;
}

void Circle::draw(GraphicsContext* gs) const
{
	// Make sure the z component is zero. 3D is not supported yet...
	if (pts[2][0] != 0 || pts[2][1] != 0)
	{
		throw shapeException("3D Drawing Not implemented yet");
	}

	// set the color to the shape's
	gs->setColor(this->color);
	
	// Compute the radius...
	double dx = (pts[0][0] - pts[0][1]);
	double dy = (pts[1][0] - pts[1][1]);
	// Check if one of them is zero. Please no sqrt!
	double r;
	if (dx == 0) r = std::abs(dy);
	else if (dy == 0) r = std::abs(dx);
	else r = std::sqrt(dx*dx + dy*dy);
	
	// utilize Circle drawing algorithm in GraphicsContext
	gs->drawCircle(this->pts[0][0], this->pts[1][0], r);
}

void Circle::out(std::ostream & os) const
{	
	// output shape specifier
	os << "c(";
	
	// output shape-specific data
	Shape::out(os);
	
	// compute the string for a new line, spaceLevel accounts for if previous
	// level was tabbed
	std::string lineTab(sizeof("s(color=0xFFFFFF ")-1 + this->spaceLevel, ' ');

	// output point on the circle
	os << std::endl << lineTab << "p2=[";
	for (int i=0; i<4; i++)
	{
		os << pts[i][1];
		// append a space except for last element
		if (i != 3)
		{
			os << " ";
		}
	}
	
	// End of output report
	os << "]')";
}

void Circle::in(std::istream & is)
{
	// Parse shape-specific data
	Shape::in(is);
	
	// skip until you get to a point
	char cskip = '\0';
	is >> cskip;
	if (cskip != 'p')
	{
		throw shapeException("Invalid shape Format: Expected endpoint after " \
				"start point...");
	}
	is.ignore(sizeof("1=[")-1);
	
	// parse point on the circle
	for (int i = 0; i<4; i++)
	{
		is >> this->pts[i][1];
	}

	// discard the rest out of the stream ("]'")
	is.ignore(sizeof("]'")-1);
	
	// Done! Parse the last ')'. It's part of the format.
	is >> cskip;
	
	if (cskip != ')')
	{
		throw shapeException("Invalid shape Format: Expected End Parenthesis");
	}	
}

Shape* Circle::clone() const
{
	Circle *c = new Circle(*this);
	return c;
}

std::ostream& operator<<(std::ostream &os, const Circle &c)
{
	c.out(os);
	return os;
}

std::istream& operator>>(std::istream &is, Circle &c)
{
	c.in(is);
	return is;
}