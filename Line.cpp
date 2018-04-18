// @author Mohammed Alzakariya
// @file Point.cpp
// Implementation cpp file for the Line concrete class and operator<</>> overloads

#include "Line.h"
#include <string>

Line::Line(const matrix &pts, int color)
	: Shape(pts[0][0], pts[1][0], pts[2][0], color, 2)
{ 
	// assign startpoint and endpoint. matrix is assumed 3x2 (or bigger), otherwise this fails
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

Line::Line(const Line &s)
	: Shape(s)
{ }

Line::~Line()
{
	// does nothing, but must be defined
}

Line& Line::operator=(const Line& rhs)
{
	// TODO: test
	
	// Shape data
	this->color = rhs.color;
	this->pts = rhs.pts;
	this->spaceLevel = rhs.spaceLevel;
	
	return *this;
}

void Line::draw(GraphicsContext* gs) const
{
	// TODO: test

	// set the color to the shape's
	gs->setColor(this->color);
	
	// utilize line drawing algorithm in GraphicsContext
	gs->drawLine(this->pts[0][0], this->pts[1][0], this->pts[0][1], this->pts[1][1]);
}

void Line::out(std::ostream & os) const
{
	// TODO: test
	
	// output shape specifier
	os << "l(";
	
	// output shape-specific data
	Shape::out(os);
	
	// compute the string for a new line, spaceLevel accounts for if previous level was tabbed
	std::string lineTab(sizeof("p(color=0xFFFFFF ")-1 + this->spaceLevel, ' ');

	// output endpoint
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

void Line::in(std::istream & is)
{
	// TODO: test

	// ignore shape specifier, and parse the shape-specific data
	is.ignore(sizeof("l(")-1);
	Shape::in(is);
	
	// skip until you get to a point
	is.ignore(100, '[');
	
	// parse endpoint
	for (int i = 0; i<4; i++)
	{
		is >> this->pts[i][1];
	}
	
	// Done! Ignore the last ')'. It's part of the format.
	is.ignore(sizeof(")")-1);
}

Shape* Line::clone() const
{
	// TODO: test
	Line *l = new Line(*this);
	return l;
}

std::ostream& operator<<(std::ostream &os, const Line &l)
{
	l.out(os);
	return os;
}

std::istream& operator>>(std::istream &is, Line &l)
{
	l.in(is);
	return is;
}