// @author Mohammed Alzakariya
// @file Point.cpp
// Implementation cpp file for the Point concrete class and operator<</>> overloads

#include "Point.h"

Point::Point(double x, double y, double z, int color)
	: Shape(x,y,z,color,1)
{ }

Point::Point(const Point &s)
	: Shape(s)
{ }

Point::~Point()
{
	// does nothing, but must be defined
}

Point& Point::operator=(const Point& rhs)
{
	
	// Shape data
	assignShapeData(rhs);
	
	return *this;
}

void Point::draw(GraphicsContext *gc, ViewContext *vc) const
{	
	// Set the color and draw the device converted point
	gc->setColor(this->color);
	matrix devPts = vc->modelToDevice(this->pts);
	gc->setPixel(devPts[0][0], devPts[1][0]);
}

void Point::out(std::ostream & os) const
{	
	// output shape specifier
	os << "p(";
	
	// output shape-specific data
	Shape::out(os);
	
	// close parenthesis! End of output report
	os << ")";
}

void Point::in(std::istream & is)
{
	// Input Format: "(color=<RGB_int> p1=[<x> <y> <z> <a>]')"
	// No additional data, just parse the shape data
	Shape::in(is);
	
	// Done! Parse the last ')'. It's part of the format.
	char cskip = '\0';
	is >> cskip;
	if (cskip != ')' ) throw shapeException("Invalid shape Format:");

}

Shape* Point::clone() const
{
	Point *p = new Point(*this);
	return p;
}

std::ostream& operator<<(std::ostream &os, const Point &p)
{
	p.out(os);
	return os;
}

std::istream& operator>>(std::istream &is, Point &p)
{
	p.in(is);
	return is;
}