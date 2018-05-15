// @author Mohammed Alzakariya
// @file Point.cpp
// Implementation cpp file for the Line concrete class and operator<</>> overloads

#include "Line.h"
#include <string>

Line::Line(const matrix &pts, int color)
	: Shape(pts[0][0], pts[1][0], pts[2][0], color, 2)
{ 
	// assign startpoint and endpoint. matrix is assumed 3x2 (or bigger), 
	// otherwise this fails
	for (int r=0; r<3; r++)
	{
		for (int c=0; c<2; c++)
		{
			this->pts[r][c] = pts[r][c];
		}
	}
	// default row 3 is 1.0 for translation
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
	// Shape data
	assignShapeData(rhs);
	
	return *this;
}

void Line::draw(GraphicsContext *gc, ViewContext *vc) const
{
	// set the color to the shape's
	gc->setColor(this->color);
	
	// convert to device coordinates
	matrix devPts = vc->modelToDevice(this->pts);
	
	// utilize line drawing algorithm in GraphicsContext
	gc->drawLine(devPts[0][0], devPts[1][0], devPts[0][1], devPts[1][1]);
}

void Line::out(std::ostream & os) const
{	
	// output shape specifier
	os << "l(";
	
	// output shape-specific data
	Shape::out(os);
	
	// compute the string for a new line, spaceLevel accounts for if previous 
	// level was tabbed
	std::string lineTab(sizeof("s(color=0xFFFFFF ")-1 + this->spaceLevel, ' ');

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
	// Parse  shape-specific data
	Shape::in(is);
	
	// skip until you get to a point
	char cskip = '\0';
	is >> cskip;
	if (cskip != 'p')
	{
		throw shapeException("Invalid shape Format: Expected endpoint after " \
				"start point...");
	}
	is.ignore(sizeof("1=[")-1);
	
	// parse endpoint
	for (int i = 0; i<4; i++)
	{
		is >> this->pts[i][1];
	}

	// discard the rest out of the stream ("]'")
	is.ignore(sizeof("]'")-1);
	
	// Done! Parse the last ')'. It's part of the format.
	is >> cskip;
	
	if (cskip != ')')
	{
		throw shapeException("Invalid shape Format: Expected End Parenthesis");
	}	
}

Shape* Line::clone() const
{
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