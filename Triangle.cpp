// @author Mohammed Alzakariya
// @file Point.cpp
// Implementation cpp file for the Triangle concrete class and operator<</>> overloads

#include "Triangle.h"
#include <string>

Triangle::Triangle(const matrix &pts, int color)
	: Shape(pts[0][0], pts[1][0], pts[2][0], color, 3)
{ 
	// assign p2/p3. matrix is assumed 3x3 (or bigger), otherwise this fails
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
	// Shape data
	assignShapeData(rhs);
	
	return *this;
}

void Triangle::draw(GraphicsContext* gs) const
{
	// Make sure all z components is zero. 3D is not supported yet...
	if (pts[2][0] != 0 || pts[2][1] != 0 || pts[2][2] != 0)
	{
		throw shapeException("3D Drawing Not implemented yet");
	}
	
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
	// output shape specifier
	os << "t(";
	
	// output shape-specific data
	Shape::out(os);
	
	// compute the string for a new Line, spaceLevel accounts for if previous level 
	// was tabbed
	std::string LineTab(sizeof("s(color=0xFFFFFF ")-1 + this->spaceLevel, ' ');

	// output p2 and p3
	for (int c=1; c<3; c++)
	{
		os << std::endl << LineTab << "p" << c+1 << "=[";
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
	os << ")";
}

void Triangle::in(std::istream & is)
{
	// parse the shape-specific data
	Shape::in(is);
	
	char cskip = '\0';
	// Parse p2 and p3
	for (int c=1; c<3; c++)
	{
		// skip until you get to a point
		is >> cskip;
		if (cskip != 'p')
		{
			throw shapeException("Invalid shape Format: Expected start of a new point");
		}
		is.ignore(sizeof("1=[")-1);
		
		// parse point
		for (int i = 0; i<4; i++)
		{
			is >> this->pts[i][c];
		}
		
		// discard the rest of the point ("]'")
		is.ignore(sizeof("]'")-1);
	}
	
	// Done! Parse the last ')'. It's part of the format.
	is >> cskip;
	
	if (cskip != ')')
	{
		throw shapeException("Invalid shape Format: Expected End Parenthesis");
	}	
}

Shape* Triangle::clone() const
{
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