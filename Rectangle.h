// @Author Mohammed Alzakariya
// This is a header file for the Rectangle subclass of the Shape abstract class
// It also contains global overloads of operator<< and operator>> for the class

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape.h"

class Rectangle : public Shape {
	
public:
	
	// A Rectangle contains four ORDERD vertices that must line-connect to form
	// a rectangle.
	// @param pts 3x4 matrix consisting of 4 column vectors of [x y z]'
	// @param color RGB representation of the color of the shape, each color is a byte.	
	// @throws matrixException if the matrix is not 3x4 (or bigger)
	// @throws shapeException if 4 points passed do not represent a rectangle
	// when connected in order p0 -> p1 -> p2 -> p3
	Rectangle(const matrix &pts, int color);
	
	// A rectangle represented as a 3x1 matrix, a column vector of the origin
	// and the height and width of the rectangle
	// The origin is the center of the rectangle.
	// @param pts 3x1 matrix consisting of 1 column vectors of [x y z]'
	// @param color RGB representation of the color of the shape, each color is a byte.	
	// @throws matrixException if the matrix is not 3x1 (or bigger)
	Rectangle(const matrix &pts, double w, double h, int color);
	
	// Copy constructor for the Rectangle class
	// It builds on top of the copy constructor of the shape class
	Rectangle(const Rectangle &s);

	// This won't do anything, because the object contains Plain old data (POD).
	// The default destructing will suffice.
	virtual ~Rectangle();

	// assignment operator. Makes sure to build on the Shape's assignment
	// operator.
	Rectangle& operator=(const Rectangle& rhs);
	
	// This sets the GraphicsContext color to the shape's color 
	// and draws the Rectangle by drawing 4 segments using the GraphicsContext pointer
	// and ViewContext pointer
	// @throws shapeException if any z-component is non-zero. 
	// 3D Drawing not supported yet
	virtual void draw(GraphicsContext *gc, ViewContext *vc) const;

	// This implementation extends on the output of the Shape class by specifying 
	// the shape type,
	// and simply closing the parenthesis to signify the end of the output report.
	// Output Format: 
	// "r(color=<RGB_int> p1=[<x1> <y1> <z1>]' 
	//                    p2=[<x2> <y2> <z2>]'
	//					  p3=[<x3> <y3> <z3>]'
	//					  p4=[<x4> <y4> <z4>]')"
	// @param os The output stream to insert into to
	virtual void out(std::ostream & os) const;
	
	// Default implementation will be able to read color and location, pts, 
	// from istream into object.
	// This should be overriden by derived classes to parse any additional data.
	// Input Format:
	// "(color=<RGB_int> p1=[<x1> <y1> <z1>]' 
	//                    p2=[<x2> <y2> <z2>]'
	//					  p3=[<x3> <y3> <z3>]'
	//					  p4=[<x4> <y4> <z4>]')"
	// Having the shape specifier at the start is optional.
	// @throws shapeException if the format is invalid to parse
	// @param is The input stream to parse from
	virtual void in(std::istream & is);

	// the closest we get to a "virtual copy constructor"
	// This will return a new'd copy of the current shape
	// It's the responsibility of the caller to delete!
	virtual Shape* clone() const;
	
};

// global overloading of the stream insertion operator for the class
// utilizes Rectangle::out to generate the output
// @param os output stream to the left of the << operator
// @param o  Rectangle shape object to insert the output of into os
// @return os to allow for chaining 
std::ostream& operator<<(std::ostream &os, const Rectangle &o);

// global overloading of the stream extraction operator for the class
// utilizes Rectangle::in to generate the input
// @param is input stream to the left of the >> operator
// @param o  Rectangle shape object to parse into
// @return is to allow for chaining
std::istream& operator>>(std::istream &is, Rectangle &o);


#endif
