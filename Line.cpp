// @author Mohammed Alzakariya
// @file Point.cpp
// Implementation cpp file for the Line concrete class and operator<</>> overloads

#include "Line.h"

Line(const Point &p1, const Point &p2, int color);
	: color(color), pts(3,2), spaceLevel(0)
{ 
	for (int i=0 i<4; i++)
	{
		// first column: startpoint
		this->pts[i][0] = startpoint.pts[i][0];
		// second column: endpoint
		this->pts[i][1] = endpoint.pts[i][0];
	}		
}

Line::Line(const Line &s)
	: color(color), pts(3,2), spaceLevel(s.spaceLevel)
{
	this->pts = s.pts;
}

Line::~Line()
{
	// does nothing, but must be defined
}

Line& operator=(const Line& rhs);
{
	// TODO: test
	
	// Shape data
	this->color = rhs.color;
	this->pts = rhs.pts;
	this->spaceLevel = rhs.spaceLevel;
	
	return *this;
}

void Point::draw(GraphicsContext* gs) const
{
	// TODO: test

	// set the color to the shape's
	gs->setColor(this->color);
	
	// utilize line drawing algorithm in GraphicsContext
	gs->drawLine(this->pts[0][0], this->pts[1][0], this->pts[0][1], this->pts[1][1]);
}

void Point::out(std::ostream & os) const
{
	// TODO: test
	
	// output shape specifier
	os << "p(";
	
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