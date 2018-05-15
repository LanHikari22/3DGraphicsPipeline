// @author Mohammed Alzakariya
// @file Shape.h
// This file provides declarations for the abstract Shape class 
// which is derived by several other shapes
// It also contains global overloads of operator<< and operator>> for the class

#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include "matrix.h"
#include "x11context.h"
#include "ViewContext.h"

#include <stdexcept>	// for std::runtime_error
 
// a helper class to bundle a message with any thrown exceptions.
// To use, simply 'throw shapeException("A descriptive message about
// the problem")'.  This will throw the exception object by value.
// Recommendation is to catch by reference (to prevent slicing).
class shapeException:public std::runtime_error
{
	public:
		shapeException(std::string message):
		      std::runtime_error((std::string("Shape Exception: ") + 
		               message).c_str()) {}
};

class Shape {
	
protected:
	// RGB representation of the color of the shape
	int color;
	
	// points matrix: 4xn matrix; set to the transpose of [X Y Z 1.0]. 
	// (TODO: 1.0 for now)
	// In the Shape abstract class it's 4x1, but it makes sense to extend 
	// it to n columns
	// for n points in the shape.
	matrix pts;
	
	// Additional amount of space padding to insert after line 1.
	//          This is line 1.
	// [-------]This is line 2. ([----]: Automatic padding)
	// someText:         This is line 1.
	// [xxxxxxx][-------]This is line 2. ([xxxx]: Specified extra padding)
	// [xxxxxxx] would be the specified extra padding: spaceLevel.
	// Tabs can certainly mess this process, so if tabs are used, 
	// the tab to space equivalent is
	// expected to be accounted for.
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
	
	// I can't call the operator= of the base class, but I can call this...
	// This basically assigns all Shape related data, so derived classes wouldn't
	// have to
	void assignShapeData(const Shape& rhs);
	
	// Assigns color and coordinates of the rhs shape to this shape
	// Never call the Shape's operator=, That makes no sense!
	Shape& operator=(const Shape& rhs) = delete;
	
	// a concrete class defines the exact mechanisms of drawing this
	// The ViewContext pointer is used to convert from model to
	// device coordinates.
	virtual void draw(GraphicsContext *gc, ViewContext *vc) const = 0;
	
	// the amoount of space padding to put in a second line when outputting to a 
	// stream
	void setSpaceLevel(unsigned int spaceLevel);
	
	// Default implementation of this will print the color and location, pts.
	// However, it should be overriden by derived classes to display data concrete to 
	// those classes
	// Output Format: "color=<RGB_int> p1= [<x1> <y1> <z1> <a1>]'"
	// @param os The output stream to insert into to
	virtual void out(std::ostream & os) const;
	
	// Default implementation will be able to read color and location, pts, 
	// from istream into object.
	// This should be overriden by derived classes to parse any additional data.
	// Input Format: "(color=<RGB_int> p1= [<x1> <y1> <z1> <a1>]'"
	// If a character comes before the '(' to signify shape type, it is 
	// ignored by this
	// parsing function.
	// @param is The input stream to parse from
	// @throws shapeException for invalid format
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