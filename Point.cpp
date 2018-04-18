// @author Mohammed Alzakariya
// @file Point.cpp
// Implementation cpp file for the Point concrete class

#include "Point.h"

Point::Point(double x, double y, double z, int color)
	: Shape(x,y,z,color)
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
	// TODO: test
	
	// Shape data
	this->color = rhs.color;
	this->p1 = rhs.p1;
	
	return *this;
}

void Point::draw(GraphicsContext* gs) const
{
	// TODO: test

	// simply set the color and draw a pixel at the origin of the shape
	gs->setColor(this->color);
	gs->setPixel(this->p1[0][0], this->p1[1][0]);
}

void Point::out(std::ostream & os) const
{
	// TODO: test
	
	// output shape specifier
	os << "p";
	
	// output shape-specific data
	Shape::out(os);
	
	// close parenthesis! End of output report
	os << ")";
}

void Point::in(std::istream & is)
{
	// TODO: test

	// ignore shape specifier, and parse the shape-specific data
	is.ignore(sizeof("p")-1);
	Shape::in(is);
	
	// Done! Ignore the last ')'. It's part of the format.
	is.ignore(sizeof(")")-1);
}

Shape* Point::clone() const
{
	// TODO: test
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