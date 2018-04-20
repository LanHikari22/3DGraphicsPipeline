// @author Mohammed Alzakariya
// @file Point.cpp
// Implementation cpp file for the Polygon concrete class and operator<</>> overloads

#include "Polygon.h"
#include <string>

Polygon::Polygon(const matrix &pts, int color)
	: Shape(pts[0][0], pts[1][0], pts[2][0], color, Polygon::INITIAL_CAPACITY), 
	  numColumns(1), columnCapacity(Polygon::INITIAL_CAPACITY)
{ }

Polygon::Polygon(unsigned int numColumns, const matrix &pts, int color)
	: Shape(pts[0][0], pts[1][0], pts[2][0], color, numColumns*Polygon::CAPACITY_MULTIPLIER), 
	  numColumns(numColumns), columnCapacity(numColumns*Polygon::CAPACITY_MULTIPLIER)
{	
	// assign numColumns points. matrix is assumed 3xnumColumns (or bigger), otherwise this fails
	for (unsigned int c=0; c<numColumns; c++)
	{
		this->pts[0][c] = pts[0][c];
		this->pts[1][c] = pts[1][c];
		this->pts[2][c] = pts[2][c];
		this->pts[3][c] = 0; // This is defaulted to zero for now
	}
}

Polygon::Polygon(const Polygon &s)
	: Shape(s), numColumns(s.numColumns), columnCapacity(s.columnCapacity)
{ }

Polygon::~Polygon()
{
	// does nothing, but must be defined
}

void Polygon::updateColumnCapacity()
{
	// First, make sure we have a big enough matrix to add a column
	if (numColumns >= columnCapacity)
	{
		matrix newMatrix(4, columnCapacity*Polygon::CAPACITY_MULTIPLIER);
		columnCapacity *= Polygon::CAPACITY_MULTIPLIER; // Ay! Bigger Capacity for more adds!
		// copy old content
		for (int r=0; r<4; r++)
		{
			for (unsigned int c=0; c<numColumns; c++)
			{
				newMatrix[r][c] = pts[r][c];
			}
		}
		// Yay! This is our new points matrix!
		pts = newMatrix;		
	}
}

void Polygon::addVertex(double x, double y, double z)
{
	// First, make sure we have a big enough matrix to add a column
	updateColumnCapacity();
	
	// We have space, add one more column in!
	pts[0][numColumns] = x;
	pts[1][numColumns] = y;
	pts[2][numColumns] = z;

	// Now we have one more column in effect!
	numColumns++;
}

Polygon& Polygon::operator=(const Polygon& rhs)
{
	// Shape data
	assignShapeData(rhs);
	
	this->numColumns = rhs.numColumns;
	this->columnCapacity = rhs.columnCapacity;
	
	return *this;
}

void Polygon::draw(GraphicsContext* gs) const
{
	// Make sure all z components is zero. 3D is not supported yet...
	bool zeroZ = true;
	for (unsigned int c=0; c<numColumns; c++)
	{
		zeroZ &= pts[2][c] == 0;
	}
	if (not zeroZ)
	{
		throw shapeException("3D Drawing Not implemented yet");
	}

	// polygon needs to at least be a triangle!
	if (numColumns < 3)
	{
		throw shapeException("Less than 3 points: Polygon needs to at least be a triangle");

	}
	
	// set the color to the shape's
	gs->setColor(this->color);
	
	// utilize the line drawing algorithm in GraphicsContext
	// This is fun! connect all vertices together!
	for (unsigned int c=0; c<numColumns; c++)
	{
		int nextC = (c+1) % numColumns;
		gs->drawLine(pts[0][c], pts[1][c], pts[0][nextC], pts[1][nextC]);
	}
	
}

void Polygon::out(std::ostream & os) const
{	
	// output shape specifier
	os << "g(";
	
	// output shape-specific data
	Shape::out(os);
	
	
	// compute the string for a new Line, spaceLevel accounts for if previous level was tabbed
	std::string LineTab(sizeof("s(color=0xFFFFFF ")-1 + this->spaceLevel, ' ');
	
	// output vertices
	for (unsigned int c=1; c<numColumns; c++)
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

void Polygon::in(std::istream & is)
{
	// we're parsing new data
	pts = matrix(4,Polygon::INITIAL_CAPACITY);
	columnCapacity = INITIAL_CAPACITY;

	
	// parse the shape-specific data
	Shape::in(is);
	
	numColumns = 1;
	
	char cskip = '\0';
	// Parse vertices
	while (is >> cskip)
	{
		if (cskip == ')')
		{
			// End of polygon vertex list! Quit!
			break;
		}
		if (cskip != 'p')
		{
			throw shapeException("Invalid shape Format: Expected start of a new point");
		}
		is.ignore(sizeof("1=[")-1);

		// Make sure to have enough space for next point to parse
		updateColumnCapacity();

		// parse point
		for (int i = 0; i<4; i++)
		{
			is >> this->pts[i][numColumns];
		}
		
		// discard the rest of the point ("]'")
		is.ignore(sizeof("]'")-1);
		
		// We parsed one more column!
		numColumns++;
	}	
	
}

Shape* Polygon::clone() const
{
	Polygon *o = new Polygon(*this);
	return o;
}

std::ostream& operator<<(std::ostream &os, const Polygon &o)
{
	o.out(os);
	return os;
}

std::istream& operator>>(std::istream &is, Polygon &o)
{
	o.in(is);
	return is;
}