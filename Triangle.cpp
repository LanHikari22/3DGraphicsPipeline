// @author Mohammed Alzakariya
// @file Point.cpp
// Implementation cpp file for the Triangle concrete class and operator<</>> overloads

#include "Triangle.h"
#include <string>

Triangle::Triangle(const matrix &pts, int color)
	: Shape(pts[0][0], pts[1][0], pts[2][0], color, 3)
{ 
	// assign startpoint and endpoint. matrix is assumed 3x3 (or bigger), otherwise this fails
	for (int r=0; r<3; r++)
	{
		for (int c=0; c<3; c++)
		{
			this->pts[r][c] = pts[r][c];
		}
	}
	// default row 3 is 1.0 for now (component 4)
	this->pts[3][0] = 1.0;
	this->pts[3][1] = 1.0;
	this->pts[3][2] = 1.0;
}

Triangle::Triangle(const Triangle &s)
	: Shape(s)
{ }

Triangle::~Triangle()
{
	// does nothing, but must be defined
}

Triangle& Triangle::operator=(const Triangle& rhs)
{
	// TODO: test
	
	// Shape data
	this->color = rhs.color;
	this->pts = rhs.pts;
	this->spaceLevel = rhs.spaceLevel;
	
	return *this;
}

void Triangle::draw(GraphicsContext* gs) const
{
	// TODO: test

	// set the color to the shape's
	gs->setColor(this->color);
	
	// utilize the line drawing algorithm in GraphicsContext
	// connect all three vertices together
	gs->drawLine(this->pts[0][0], this->pts[1][0], this->pts[0][1], this->pts[1][1]);
	gs->drawLine(this->pts[0][1], this->pts[1][1], this->pts[0][2], this->pts[1][2]);
	gs->drawLine(this->pts[0][2], this->pts[1][2], this->pts[0][0], this->pts[1][0]);

}

void Triangle::out(std::ostream & os) const
{
	// TODO: test
	
	// output shape specifier
	os << "t(";
	
	// output shape-specific data
	Shape::out(os);
	
	// compute the string for a new Triangle, spaceLevel accounts for if previous level was tabbed
	std::string TriangleTab(sizeof("s(color=0xFFFFFF ")-1 + this->spaceLevel, ' ');

	// output p2 and p3
	for (int c=1; c<3; c++)
	{
		os << std::endl << TriangleTab << "p" << c+1 << "=[";
		for (int r=0; r<4; r++)
		{
			os << pts[r][c];
			// append a space except for last element
			if (r != 3)
			{
				os << " ";
			}
		}
		os << "]'";
	}
	
	// End of output report
	os << "]')";
}

void Triangle::in(std::istream & is)
{
	// TODO: test

	// ignore shape specifier, and parse the shape-specific data
	is.ignore(sizeof("t(")-1);
	Shape::in(is);
	
	// Parse p2 and p3
	for (int c=1; c<3; c++)
	{
		// skip until you get to a point
		is.ignore(100, '[');
		
		// parse endpoint
		for (int i = 0; i<4; i++)
		{
			is >> this->pts[i][c];
		}
	}
	
	// Done! Ignore the last ')'. It's part of the format.
	is.ignore(sizeof(")")-1);
}

Shape* Triangle::clone() const
{
	// TODO: test
	Triangle *t = new Triangle(*this);
	return t;
}

std::ostream& operator<<(std::ostream &os, const Triangle &t)
{
	t.out(os);
	return os;
}

std::istream& operator>>(std::istream &is, Triangle &t)
{
	t.in(is);
	return is;
}