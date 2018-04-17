// @author Mohammed Alzakariya
// @file Shape.h
// This file provides defintions for the abstract Shape class which is derived by several other shapes

#ifndef SHAPE_H
#define SHAPE_H

#include "iostream"
#include "matrix.h"
#include "x11context.h"

class Shape {
	
protected:
	// RGB representation of the color of the shape
	int color;
	// 4x1 matrix; set to the transpose of [X Y Z 1.0]. (1.0 for now)
	matrix p1;
	
public:
	// a shape will have at least one point, and should be utilized 
	// @params (x,y,z) coordinates of the shape
	// @param color RGB representation of the color of the shape, each color is a byte.
	Shape(double x, double y, double z, int color);
	
	// Copy consructor for shape
	// Will copy the color and single point
	Shape(const Shape &s);
	
	// virtual destructor must be present and defined, even if it does nothing
	virtual ~Shape();
	
	// Assigns color and coordinates of the rhs shape to this shape
	Shape& operator=(const Shape& rhs);
	
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

#endif