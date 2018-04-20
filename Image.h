// @Author Mohammed Alzakariya
// This is the header file for the Image class

#ifndef IMAGE_H
#define IMAGE_H

#include "Shape.h"
#include "Point.h"
#include "Line.h"
#include "Triangle.h"
#include "Circle.h"
#include "Rectangle.h"
#include "x11context.h"
#include <vector>

class Image {
	// container for shape pointers: anything that extends Shape can be here
	std::vector<Shape*> shapes;
	// Additional amount of space padding to insert to each shape. Helps printing good output
	unsigned int spaceLevel;

public:
	// no-argument constructor. Creates an empty Image with no shapes in it
	Image();
	
	// Copy constructor -- deep-copies contents of given image
	// @param i	Reference to Image to be deep-copied
	Image(const Image &i);
	
	// Destructor, for deleting all dynamically allocated Shapes
	~Image();
	
	// Assignment Operator -- Deep-copies content from rhs
	// All shape objects pointed to in the shapes container are destroyed first
	// @param rhs	Reference to Image to be deep-copied in assignment
	// @returns Reference to this Image for chaining
	Image& operator=(const Image &rhs);
	
	// Adds a Shape to the container by deep-copying the contents given
	// This gives the responsibility of freeing the dynamically allocated data to the
	// container alone
	// @param s	Shape pointer; Deep-copied content will be allocated in the heap
	void add(const Shape *s);
	
	// Invokes the draw() method of all shape objects within the shapes container
	// @param gs Pointer to GraphicsContext used for drawing
	void draw(GraphicsContext *gs);
	
	// Configures output for Extra space padding to generate output
	// that does not begin at the start of a line
	void setSpaceLevel(unsigned int spaceLevel);
	
	// Outputs all shapes to an ostream
	std::ostream& out(std::ostream &os) const;
	
	// Reads a set of shapes from istream
	std::istream& in(std::istream &is);
	
	// removes all shapes from memory, and deallocates dynamic objects in the Image
	// this destructor logic can be used in the destructor, but also the assignment operator
	void erase();
};

// global overloading of the stream insertion operator for the class
// utilizes Image::out to generate the output
// @param os output stream to the left of the << operator
// @param o  Image shape object to insert the output of into os
// @return os to allow for chaining 
std::ostream& operator<<(std::ostream &os, const Image &o);

// global overloading of the stream extraction operator for the class
// utilizes Image::in to generate the input
// @param is input stream to the left of the >> operator
// @param o  Image shape object to parse into
// @return is to allow for chaining
std::istream& operator>>(std::istream &is, Image &o);


#endif