// @Author Mohammed Alzakariya
// This is a header file for the Line subclass of the Shape abstract class
// It also contains global overloads of operator<< and operator>> for the class

#ifndef LINE_H
#define LINE_H

#include "Shape.h"

class Line : public Shape {
	
public:
	
	// A line contains two points: a startpoint and endpoint.
	// @param pts 3x2 matrix consisting of a startpoint column and an endpoint 
	// column of [x y z]'
	// @param color RGB representation of the color of the shape, each color is a byte.	
	// @throws matrixException if the matrix is not 3x2 (or bigger)
	Line(const matrix &pts, int color);
	
	// Copy constructor for the Line class
	// It builds on top of the copy constructor of the shape class
	Line(const Line &s);

	// This won't do anything, because the object contains Plain old data (POD).
	// The default destructing will suffice.
	virtual ~Line();

	// assignment operator. Makes sure to build on the Shape's assignment
	// operator.
	Line& operator=(const Line& rhs);
	
	// This sets the GraphicsContext color to the shape's color 
	// and draws a line from p1 to p2 using the passed GraphicsContext pointer
	// and ViewContext pointer
	// @throws shapeException if z-component is non-zero. 3D drawing not supported yet.
	virtual void draw(GraphicsContext *gc, ViewContext *vc) const;

	// This implementation extends on the output of the Shape class by specifying 
	// the shape type,
	// and simply closing the parenthesis to signify the end of the output report.
	// Output Format: 
	// "p(color=<RGB_int> p1=[<x1> <y1> <z1>]' 
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
// utilizes Line::out to generate the output
// @param os output stream to the left of the << operator
// @param o  Line shape object to insert the output of into os
// @return os to allow for chaining 
std::ostream& operator<<(std::ostream &os, const Line &o);

// global overloading of the stream extraction operator for the class
// utilizes Line::in to generate the input
// @param is input stream to the left of the >> operator
// @param o  Line shape object to parse into
// @return is to allow for chaining
std::istream& operator>>(std::istream &is, Line &o);


#endif
