// @author Mohammed Alzakariya
// @file Shape.cpp
// Implementation cpp file for the Shape abstract class and operator<</>> overloads

#include "Shape.h"
#include <iomanip>


Shape::Shape(double x, double y, double z, int color, int numPoints) 
	: color(color), pts(4,numPoints), spaceLevel(0)
{
	// set up x,y,z in the matrix
	pts[0][0] = x;
	pts[1][0] = y;
	pts[2][0] = z;
	pts[3][0] = 1.0; // TODO: for now.
}

Shape::Shape(const Shape &s)
	: color(s.color), pts(s.pts), spaceLevel(s.spaceLevel)
{	}

Shape::~Shape()
{
	// does nothing, but is needed!
}

Shape& Shape::operator=(const Shape& rhs)
{
	this->color = rhs.color;
	this->pts = rhs.pts;
	this->spaceLevel = rhs.spaceLevel;
	return *this;
}

void Shape::setSpaceLevel(unsigned int spaceLevel)
{
	this->spaceLevel = spaceLevel;
}


void Shape::out(std::ostream & os) const
{
	// output format: "(color=<RGB_int> p1=[<x> <y> <z> <a>]'"
	// outputting in zero padded hex for color...
	os  << "color=0x" << std::setw(6) << std::setfill('0') << std::uppercase 
			<< std::hex << color << " p1=[";
	// output x, y, z, and the 4th compoenent of p1
	for (int i=0; i<4; i++)
	{
		os << pts[i][0];
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
	is.ignore(sizeof("color=")-1);
	is >> std::hex >> this->color;
	
	// parse p1
	is.ignore(sizeof(" p1=[")-1);
	for (int i = 0; i<4; i++)
	{
		is >> this->pts[i][0];
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