// @Author Mohammed Alzakariya
// This is a header file for the Polygon subclass of the Shape abstract class
// It also contains global overloads of operator<< and operator>> for the class

#ifndef POLYGON_H
#define POLYGON_H


#include "Shape.h"

class Polygon : public Shape {
	
	// Everytime capacity must change, it multiplies by this amount
	static const int CAPACITY_MULTIPLIER = 2;
	// First number of vertices that can create a polygon not covered by other classes
	static const int INITIAL_CAPACITY = 5;
	
	// Actual number of columns. Needed since the number of points is now arbitrary
	unsigned int numColumns;
	
	// number of columns within matrix, since now matrix should be growing
	// dynamically... This will grow CAPACITY_MULTIPLIER times the current capacity 
	// everytime it's reached
	// this will initialize to INITIAL_CAPACITY
	unsigned int columnCapacity;
	
	// This updates the matrix with a new capacity if it's full
	// USed to handle capacity logic
	void updateColumnCapacity();
	
public:
	
	// A Polygon contains an arbitrary number of vertices
	// @param pts p1; 3x1 matrix consisting of 1 column vector of [x y z]'
	// @param color RGB representation of the color of the shape, each color is a byte.	
	// @throws matrixException if the matrix is not 3x1 (or bigger)
	Polygon(const matrix &pts, int color);
		
	// A Polygon contains an arbitrary number of vertices.
	// This constructor allows passing an an arbitrary number of columns in a matrix
	// to instantiate an arbitrary number of vertices
	// @param pts 3xn matrix consisting of numColumns column vector of [x y z]'
	// @param numColumns Number of points to instantiate on init.
	// @param color RGB representation of the color of the shape, each color is a byte.	
	// @throws matrixException if the matrix is not 3xnumColumns (or bigger)
	Polygon(unsigned int numColumns, const matrix &pts, int color);

	// Copy constructor for the Polygon class
	// It builds on top of the copy constructor of the shape class
	Polygon(const Polygon &s);

	// This won't do anything, because the object contains Plain old data (POD).
	// The default destructing will suffice.
	virtual ~Polygon();

	// Adds a vertex to the polygon. This increments numColumns
	// It also creates a bigger matrix if its capacity is reached
	void addVertex(double x, double y, double z);
	
	// assignment operator. Makes sure to build on the Shape's assignment
	// operator.
	Polygon& operator=(const Polygon& rhs);
	
	// This sets the GraphicsContext color to the shape's color 
	// and draws the Polygon by drawing n segments using the GraphicsContext pointer
	// @throws shapeException if any z-component is non-zero. 3D Drawing not supported yet
	// @throws shapeException if numColumns < 3. A polygon needs to AT LEAST be a triangle
	virtual void draw(GraphicsContext* gs) const;

	// This implementation extends on the output of the Shape class by specifying the shape type,
	// and simply closing the parenthesis to signify the end of the output report.
	// Output Format: 
	// "r(color=<RGB_int> p1=[<x1> <y1> <z1>]' 
	//                    p2=[<x2> <y2> <z2>]'
	//					  p3=[<x3> <y3> <z3>]'
	//					  p4=[<x4> <y4> <z4>]')"
	// @param os The output stream to insert into to
	virtual void out(std::ostream & os) const;
	
	// Default implementation will be able to read color and location, pts, from istream into object.
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
// utilizes Polygon::out to generate the output
// @param os output stream to the left of the << operator
// @param o  Polygon shape object to insert the output of into os
// @return os to allow for chaining 
std::ostream& operator<<(std::ostream &os, const Polygon &o);

// global overloading of the stream extraction operator for the class
// utilizes Polygon::in to generate the input
// @param is input stream to the left of the >> operator
// @param o  Polygon shape object to parse into
// @return is to allow for chaining
std::istream& operator>>(std::istream &is, Polygon &o);


#endif
