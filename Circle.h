// @Author Mohammed Alzakariya
// This is a header file for the Circle subclass of the Shape abstract class
// It also contains global overloads of operator<< and operator>> for the class

#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"

class Circle : public Shape {
	
public:
	
	// A Circle contains two points: an origin and a point on the circle.
	// @param pts 3x2 matrix. Contains columns for the points specified above
	// @param color RGB representation of the color of the shape, each color is a byte.	
	// @throws matrixException if the matrix is not 3x2 (or bigger)
	Circle(const matrix &pts, int color);
	
	// Copy constructor for the Circle class
	// It builds on top of the copy constructor of the shape class
	Circle(const Circle &s);

	// This won't do anything, because the object contains Plain old data (POD).
	// The default destructing will suffice.
	virtual ~Circle();

	// assignment operator. Makes sure to build on the Shape's assignment
	// operator.
	Circle& operator=(const Circle& rhs);
	
	// This sets the GraphicsContext color to the shape's color 
	// and draws a Circle using the origin and a point on the circle
	// using the passed GraphicsContext pointer
	// @throws shapeException if z-component is non-zero. 3D drawing not supported yet.
	virtual void draw(GraphicsContext* gs) const;

	// This implementation extends on the output of the Shape class by specifying 
	// the shape type,
	// and simply closing the parenthesis to signify the end of the output report.
	// Output Format: 
	// "c(color=<RGB_int> p1=[<x1> <y1> <z1>]' 
	//                    p2=[<x2> <y2> <z2>]')"
	// @param os The output stream to insert into to
	virtual void out(std::ostream & os) const;
	
	// Default implementation will be able to read color and location, pts, 
	// from istream into object.
	// This should be overriden by derived classes to parse any additional data.
	// Input Format:
	// "(color=<RGB_int> p1=[<x1> <y1> <z1>]' 
	//                   p2=[<x2> <y2> <z2>]')"
	// Having a shape specifier character before '(' is optional.
	// @param is The input stream to parse from
	// @throws shapeException if an invalid format to parse is passed
	virtual void in(std::istream & is);

	// the closest we get to a "virtual copy constructor"
	// This will return a new'd copy of the current shape
	// It's the responsibility of the caller to delete!
	virtual Shape* clone() const;
	
};

// global overloading of the stream insertion operator for the class
// utilizes Circle::out to generate the output
// @param os output stream to the left of the << operator
// @param o  Circle shape object to insert the output of into os
// @return os to allow for chaining 
std::ostream& operator<<(std::ostream &os, const Circle &o);

// global overloading of the stream extraction operator for the class
// utilizes Circle::in to generate the input
// @param is input stream to the left of the >> operator
// @param o  Circle shape object to parse into
// @return is to allow for chaining
std::istream& operator>>(std::istream &is, Circle &o);


#endif
