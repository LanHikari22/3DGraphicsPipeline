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
	this->color = rhs.color;
	this->pts = rhs.pts;
	
	return *this;
}

void Point::draw(GraphicsContext* gs) const
{
	// simply set the color and draw a pixel at the origin of the shape
	gs->setColor(this->color);
	gs->setPixel(this->pts[0][0], this->pts[1][0]);
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
	// ignore shape specifier, and parse the shape-specific data
	is.ignore(sizeof("p(")-1);
	Shape::in(is);
	
	// Done! Ignore the last ')'. It's part of the format.
	is.ignore(sizeof(")")-1);
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