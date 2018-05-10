// @author Mohammed Alzakariya
// @file Point.cpp
// Implementation cpp file for the Rectangle concrete class and operator<</>> overloads

#include "Rectangle.h"
#include <string>

Rectangle::Rectangle(const matrix &pts, int color)
	: Shape(pts[0][0], pts[1][0], pts[2][0], color, 4)
{ 
	
	// Make sure that the 4 points connect to form a rectangle
	// clockwise or counterclockwise.
	for (int c=0; c<4; c++)
	{
		// Traverse through the polygon, make sure everytime that
		// only one dimension changes
		int nextC = (c+1) % 4;
		double dx = pts[0][c] - pts[0][nextC];
		double dy = pts[1][c] - pts[1][nextC];
		double dz = pts[2][c] - pts[2][nextC];
		double sum = dx + dy + dz;
		if (sum != dx && sum != dy && sum != dz)
		{
			// more than one variable changes... NOT A RECTANGLE!
			throw shapeException("Invalid vertices in Matrix passed for Rectangle");
		}
	}
	
	// assign p2/p3/p4. matrix is assumed 3x3 (or bigger), otherwise this fails
	for (int c=0; c<4; c++)
	{
		this->pts[0][c] = pts[0][c];
		this->pts[1][c] = pts[1][c];
		this->pts[2][c] = pts[2][c];
		this->pts[3][c] = 1; // This is defaulted to 1 for translation
	}
}

Rectangle::Rectangle(const matrix &pts, double w, double h, int color)
	: Shape(pts[0][0]-w/2, pts[1][0]-h/2, pts[2][0], color, 4)
{
	// Assign p2
	this->pts[0][1] = pts[0][0]+w/2;
	this->pts[1][1] = pts[1][0]-h/2;
	// Assign p3
	this->pts[0][2] = pts[0][0]+w/2;
	this->pts[1][2] = pts[1][0]+h/2;
	// Assign p4
	this->pts[0][3] = pts[0][0]-w/2;
	this->pts[1][3] = pts[1][0]+h/2;

	// Assign z-component to the origin's (no depth change)
	// And set 4th component to 0 for now
	for (int c=1; c<4; c++)
	{
		this->pts[2][c] = pts[2][0];
		this->pts[3][c] = 1;
	}
}


Rectangle::Rectangle(const Rectangle &s)
	: Shape(s)
{ }

Rectangle::~Rectangle()
{
	// does nothing, but must be defined
}

Rectangle& Rectangle::operator=(const Rectangle& rhs)
{
	// Shape data
	assignShapeData(rhs);
	return *this;
}

void Rectangle::draw(GraphicsContext *gc, ViewContext *vc) const
{
	// Make sure all z components is zero. 3D is not supported yet...
	bool zeroZ = true;
	for (int c=0; c<4; c++)
	{
		zeroZ &= pts[2][c] == 0;
	}
	if (not zeroZ)
	{
		throw shapeException("3D Drawing Not implemented yet");
	}
	
	// set the color to the shape's
	gc->setColor(this->color);
	
	// convert to device coordinates
	matrix devPts = vc->modelToDevice(this->pts);
	
	// utilize the line drawing algorithm in GraphicsContext
	// connect all four vertices together!
	for (int c=0; c<4; c++)
	{
		int nextC = (c+1) % 4;
		gc->drawLine(devPts[0][c], devPts[1][c], 
				devPts[0][nextC], devPts[1][nextC]);
	}
}

void Rectangle::out(std::ostream & os) const
{	
	// output shape specifier
	os << "r(";
	
	// output shape-specific data
	Shape::out(os);
	
	// compute the string for a new Line, spaceLevel accounts for if previous level 
	// was tabbed
	std::string LineTab(sizeof("s(color=0xFFFFFF ")-1 + this->spaceLevel, ' ');

	// output vertices
	for (int c=1; c<4; c++)
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

void Rectangle::in(std::istream & is)
{
	// parse the shape-specific data
	Shape::in(is);
	
	char cskip = '\0';
	// Parse vertices
	for (int c=1; c<4; c++)
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

Shape* Rectangle::clone() const
{
	Rectangle *o = new Rectangle(*this);
	return o;
}

std::ostream& operator<<(std::ostream &os, const Rectangle &o)
{
	o.out(os);
	return os;
}

std::istream& operator>>(std::istream &is, Rectangle &o)
{
	o.in(is);
	return is;
}