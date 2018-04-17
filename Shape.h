#ifndef SHAPE_H
#define SHAPE_H

#include "iostream"
#include "matrix.h"
#include "GraphicsContext.h"

class Shape {
	
protected:
	// RGB representation of the color of the shape
	int color;
	// RGB
	matrix p1;
	
public:
	// a shape will have at least one point, and should be utilized 
	// @params (x,y,z) coordinates of the shape
	// @param color RGB representation of the color of the shape, each color is a byte.
	Shape(double x, double y, double z, int color);
	Shape(const Shape &s);
	virtual ~Shape();
	
	// Assigns color and coordinates of the rhs shape to this shape
	Shape& operator=(const Shape& rhs) = 0;
	
	virtual void draw(GraphicsContext* gs) const = 0;
	
	// Default implementation of this will print the color and location, p1.
	// However, it should be overriden by derived classes to display data concrete to those classes
	// @param os The output stream to insert into to
	// @return the output stream reference that was passed, after writing to it for chaining.
	virtual std::ostream& out(std::ostream & os) const;
	
	// Default implementation will be able to read color and location, p1, from istream.
	// This should be overriden by derived classes to parse any additional data.
	// @param is The input stream to parse from
	// @return the input stream reference that was TODO: should it? 
	virtual std::istream& in(std::istream & is) const;
	
	// the closest we get to a "virtual copy constructor"
	// This will return a new'd copy of the current shape
	// It's the responsibility of the caller to delete!
	virtual Shape* clone() const = 0;
};
