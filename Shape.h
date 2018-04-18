// @author Mohammed Alzakariya
// @file Shape.h
// This file provides declarations for the abstract Shape class which is derived by several other shapes
// It also contains global overloads of operator<< and operator>> for the class

#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include "matrix.h"
#include "x11context.h"

class Shape {
	
protected:
	// RGB representation of the color of the shape
	int color;
	// points matrix: 4xn matrix; set to the transpose of [X Y Z 1.0]. (TODO: 1.0 for now)
	// In the Shape abstract class it's 4x1, but it makes sense to extend it to n columns
	// for n points in the shape.
	matrix pts;
	// number of spaces to pad output lines after a new line.
	// 			This is line 1.
	// [-------]This is line 2.
	unsigned int spaceLevel;
	
public:
	// a shape will have at least one point: the origin 
	// @params (x,y,z) coordinates of the shape
	// @param color RGB representation of the color of the shape, each color is a byte.
	Shape(double x, double y, double z, int color, int numPoints);
	
	// Copy consructor for shape
	// Will copy the color and single point
	Shape(const Shape &s);
	
	// virtual destructor must be present and defined, even if it does nothing
	virtual ~Shape();
	
	// Assigns color and coordinates of the rhs shape to this shape
	Shape& operator=(const Shape& rhs);
	
	virtual void draw(GraphicsContext* gs) const = 0;
	
	// the amoount of space padding to put in a second line when outputting to a stream
	void setSpaceLevel(unsigned int spaceLevel);
	
	// Default implementation of this will print the color and location, pts.
	// However, it should be overriden by derived classes to display data concrete to those classes
	// Output Format: "color=<RGB_int> p1= [<x1> <y1> <z1> <a1>]'"
	// @param os The output stream to insert into to
	virtual void out(std::ostream & os) const;
	
	// Default implementation will be able to read color and location, pts, from istream into object.
	// This should be overriden by derived classes to parse any additional data.
	// Input Format: "color=<RGB_int> p1= [<x1> <y1> <z1> <a1>]'"
	// @param is The input stream to parse from
	virtual void in(std::istream & is);
	
	// the closest we get to a "virtual copy constructor"
	// This will return a new'd copy of the current shape
	// It's the responsibility of the caller to delete!
	virtual Shape* clone() const = 0;
	
};

// global overloading of the stream insertion operator for the Shape class
// utilizes Shape::out to generate the output
// @param os output stream to the left of the << operator
// @param s  Shape object to insert the output of into os
// @return os to allow for chaining 
std::ostream& operator<<(std::ostream &os, const Shape &s);

// global overloading of the stream extraction operator for the Shape class
// utilizes Shape::in to generate the input
// @param is input stream to the left of the >> operator
// @param s  Shape object to parse into
// @return is to allow for chaining
std::istream& operator>>(std::istream &is, Shape &s);


#endif