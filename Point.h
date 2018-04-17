#ifndef POINT_H
#define POINT_H

#include "Shape.h"

class Point : Shape {
	
public:
	
	// a shape will have at least one point, and should be utilized 
	// @params (x,y,z) coordinates of the shape
	// @param color RGB representation of the color of the shape, each color is a byte.
	Point(double x, double y, double z, int color);
	
	// This won't do anything, because the Point object contains POD.
	// The default destructing will suffice.
	virtual ~Point();
	
	Point(const Shape &s);
	Point& operator=(const Point& rhs);
	
	// This sets the GraphicsContext color to the shape's color 
	// and draws a point at the point object's coordinates using the passed GraphicsContext pointer
	virtual void draw(GraphicsContext* gs) const;
	
	// Default implementation of this will print the color and location, p1.
	// However, it should be overriden by derived classes to display data concrete to those classes
	// @param os The output stream to insert into to
	// @return the output stream reference that was passed, after writing to it for chaining.
	virtual std::ostream& out(std::ostream & os) const;
	
	// Default implementation will be able to read color and location, p1, from istream.
	// This should be overriden by derived classes to parse any additional data.
	// @param is The input stream to parse from
	// @return the input stream reference that was 
	virtual std::istream& in(std::istream & is) const;

	// the closest we get to a "virtual copy constructor"
	// This will return a new'd copy of the current shape
	// It's the responsibility of the caller to delete!
	virtual Shape* clone() const;
};