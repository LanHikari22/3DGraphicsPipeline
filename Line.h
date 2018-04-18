// @Author Mohammed Alzakariya
// This is a header file for the Line subclass of the Shape abstract class
// It also contains global overloads of operator<< and operator>> for the class

#ifndef LINE_H
#define LINE_H

#include "Shape.h"

class Line : public Shape {
	
public:
	
	// a shape will have at least one point, and should be utilized 
	// @param p1 ref of Point encapsulating the (x y z) coordinates of the startpoint
	// @param p2 ref of Point encapsulating the (x y z) coordinates of the endpoint
	// @param color RGB representation of the color of the shape, each color is a byte.	
	Line(const Point &p1, const Point &p2, int color);
	
	// Copy constructor for the Point class
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
	virtual void draw(GraphicsContext* gs) const;

	// This implementation extends on the output of the Shape class by specifying the shape type,
	// and simply closing the parenthesis to signify the end of the output report.
	// Output Format: 
	// "p(color=<RGB_int> p1=[<x1> <y1> <z1>]' 
	//    p2=[<x2> <y2> <z2>]')"
	// @param os The output stream to insert into to
	virtual void out(std::ostream & os) const;
	
	// Default implementation will be able to read color and location, pts, from istream into object.
	// This should be overriden by derived classes to parse any additional data.
	// Input Format:
	// "p(color=<RGB_int> p1=[<x1> <y1> <z1>]' 
	//    p2=[<x2> <y2> <z2>]')"
	// @param is The input stream to parse from
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
