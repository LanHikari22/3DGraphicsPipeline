// @author Mohammed Alzakariya
// @file Shape.cpp
// Implementation cpp file for the Shape abstract class

#include "Shape.h"


Shape::Shape(double x, double y, double z, int color) 
	: color(color), p1(3,1)
{
	// set up x,y,z in the matrix
	p1[0][0] = x;
	p1[1][0] = y;
	p1[2][0] = z;
}

Shape::Shape(const Shape &s)
	: color(s.color), p1(s.p1)
{	}

Shape::~Shape()
{
	// does nothing, but is needed!
}

Shape& Shape::operator=(const Shape& rhs)
{
	
}
