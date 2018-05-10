// @Author Mohammed Alzakariya
// This is a header file for the Triangle subclass of the Shape abstract class
// It also contains global overloads of operator<< and operator>> for the class

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.h"

class Triangle : public Shape {
	
public:
	
	// A Triangle contains three vertices.
	// @param pts 3x3 matrix consisting of 3 column vectors of [x y z]'
	// @param color RGB representation of the color of the shape, each color is a byte.
	//		  the color is by default not presented in STL files, so it has a default.
	// @throws matrixException if the matrix is not 3x3 (or bigger)
	Triangle(const matrix &pts, int color=GraphicsContext::CYAN);
	
	// Copy constructor for the Triangle class
	// It builds on top of the copy constructor of the shape class
	Triangle(const Triangle &s);

	// This won't do anything, because the object contains Plain old data (POD).
	// The default destructing will suffice.
	virtual ~Triangle();

	// assignment operator. Makes sure to build on the Shape's assignment
	// operator.
	Triangle& operator=(const Triangle& rhs);
	
	// This sets the GraphicsContext color to the shape's color 
	// and draws the triangle by drawing 3 segments using the GraphicsContext pointer
	// and the ViewContext pointer
	// @throws shapeException if any z-component is non-zero. 3D Drawing not supported 
	// yet
	virtual void draw(GraphicsContext *gc, ViewContext *vc) const;

	// This implementation extends on the output of the Shape class by specifying 
	// the shape type,
	// and simply closing the parenthesis to signify the end of the output report.
	// Output Format: 
	// "p(color=<RGB_int> p1=[<x1> <y1> <z1>]' 
	//                    p2=[<x2> <y2> <z2>]'
	//					  p3=[<x3> <y3> <z3>]')"
	// @param os The output stream to insert into to
	virtual void out(std::ostream & os) const;
	
	// Default implementation will be able to read color and location, pts, 
	// from istream into object.
	// This should be overriden by derived classes to parse any additional data.
	// Input Format:
	// "(color=<RGB_int> p1=[<x1> <y1> <z1>]' 
	//                    p2=[<x2> <y2> <z2>]'
	//					  p3=[<x3> <y3> <z3>]')"
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
// utilizes Triangle::out to generate the output
// @param os output stream to the left of the << operator
// @param o  Triangle shape object to insert the output of into os
// @return os to allow for chaining 
std::ostream& operator<<(std::ostream &os, const Triangle &o);

// global overloading of the stream extraction operator for the class
// utilizes Triangle::in to generate the input
// @param is input stream to the left of the >> operator
// @param o  Triangle shape object to parse into
// @return is to allow for chaining
std::istream& operator>>(std::istream &is, Triangle &o);


#endif
