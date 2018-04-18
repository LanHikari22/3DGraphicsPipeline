// @author Mohammed Alzakariya
// @file Shape.cpp
// Implementation cpp file for the Shape abstract class

#include "Shape.h"
#include <iomanip>


Shape::Shape(double x, double y, double z, int color) 
	: color(color), p1(4,1)
{
	// set up x,y,z in the matrix
	p1[0][0] = x;
	p1[1][0] = y;
	p1[2][0] = z;
	p1[3][0] = 1.0; // TODO: for now.
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
	this->color = rhs.color;
	this->p1 = rhs.p1;
	return *this;
}

void Shape::out(std::ostream & os) const
{
	// TODO: test
	// output format: "(color=<RGB_int> p1=[<x> <y> <z> <a>]'"
	os << "(color=0x" << std::uppercase << std::hex << color << " p1=[";
	// output x, y, z, and the 4th compoenent of p1
	for (int i=0; i<4; i++)
	{
		os << p1[i][0];
		// append a space except for last element
		if (i != 3)
		{
			os << " ";
		}
	}
	os << "]'";
}

void Shape::in(std::istream & is)
{
	// TODO: test
	// input format: "(color=<RGB_int> p1=[<x> <y> <z> <a>]'"
	
	// parse color
	is.ignore(sizeof("(color=")-1);
	is >> std::hex >> this->color;
	
	// parse p1
	is.ignore(sizeof(" p1=[")-1);
	for (int i = 0; i<4; i++)
	{
		is >> this->p1[i][0];
	}
	
	// discard the rest out of the stream
	is.ignore(sizeof("]'")-1);		
}

std::ostream& operator<<(std::ostream &os, const Shape &s)
{
	s.out(os);
	return os;
}

std::istream& operator>>(std::istream &is, Shape &s)
{
	s.in(is);
	return is;
}