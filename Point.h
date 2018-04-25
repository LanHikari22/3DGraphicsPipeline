// @Author Mohammed Alzakariya
// This is a header file for the Point subclass of the Shape abstract class
// It also contains global overloads of operator<< and operator>> for the class

#ifndef POINT_H
#define POINT_H

#include "Shape.h"

class Point : public Shape {
	
public:
	
	// a shape will have at least one point, and should be utilized 
	// @params (x,y,z) coordinates of the shape
	// @param color RGB representation of the color of the shape, each color is a byte.
	Point(double x, double y, double z, int color);
	
	// Copy constructor for the Point class
	// It builds on top of the copy constructor of the shape class
	Point(const Point &s);

	// This won't do anything, because the Point object contains POD.
	// The default destructing will suffice.
	virtual ~Point();

	// assignment operator. Makes sure to build on the Shape's assignment
	// operator.
	Point& operator=(const Point& rhs);
	
	// This sets the GraphicsContext color to the shape's color 
	// and draws a point at the point object's coordinates using the 
	// passed GraphicsContext pointer
	// @throws shapeException if z-component is non-zero. 3D drawing not supported yet.
	virtual void draw(GraphicsContext* gs) const;

	// This implementation extends on the output of the Shape class 
	// by specifying the shape type,
	// and simply closing the parenthesis to signify the end of the output report.
	// Output Format: "p(color=<RGB_int> p1=[<x> <y> <z>]')"
	// @param os The output stream to insert into to
	virtual void out(std::ostream & os) const;
	
	// Default implementation will be able to read color and location, pts, 
	// from istream into object.
	// This should be overriden by derived classes to parse any additional data.
	// Input Format: "(color=<RGB_int> p1=[<x> <y> <z>]'"
	// Having a shape specifier character before '(' is optional.
	// @param is The input stream to parse from
	// @throws shapeException if the format is invalid
	virtual void in(std::istream & is);

	// the closest we get to a "virtual copy constructor"
	// This will return a new'd copy of the current shape
	// It's the responsibility of the caller to delete!
	virtual Shape* clone() const;
	
};

// global overloading of the stream insertion operator for the Point class
// utilizes Point::out to generate the output
// @param os output stream to the left of the << operator
// @param p  Point shape object to insert the output of into os
// @return os to allow for chaining 
std::ostream& operator<<(std::ostream &os, const Point &p);

// global overloading of the stream extraction operator for the Point class
// utilizes Point::in to generate the input
// @param is input stream to the left of the >> operator
// @param p  Point shape object to parse into
// @return is to allow for chaining
std::istream& operator>>(std::istream &is, Point &p);


#endif
