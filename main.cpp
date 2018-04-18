#include "x11context.h"
#include "Point.h"
#include "Line.h"
#include "matrix.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <sstream>

static void waitEnter();
static void testPoint(GraphicsContext *gc);
static void testLine(GraphicsContext *gc);

int main(void)
{
	GraphicsContext *gc = new X11Context(800,600,GraphicsContext::BLACK);
 
	// Test Shapes
	testPoint(gc);
	testLine(gc);
	
    // Exit when pressing enter
	waitEnter();
	
	delete gc;
 
	return 0;
}



// waits until enter is pressed in the terminal
static void waitEnter()
{
	std::string line;
	std::getline(std::cin, line);
}

static void testPoint(GraphicsContext *gc)
{
	std::cout << "Testing Point..." << std::endl;
	
	std::cout << "  Allocating Point(x,y,z,color) on the heap..." << std::endl;
	Shape *s0 = new Point(255, 255, 0, GraphicsContext::WHITE);
	
	std::cout << "  Testing Operator<<..." << std::endl;
	std::cout << "    Expected output: p(color=0xFFFFFF p1=[255 255 0 1]')" << std::endl;
	std::cout << "    Actual output:   " << *s0 << std::endl;
	
	std::cout << "  Testing Operator>>..." << std::endl;
	std::string inStr("p(color=0x00FF00 p1=[0 1.1 2.2 3.3]')");
	std::stringstream ss(inStr);
	ss >> *s0;
	std::cout << "    Expected output: " << inStr << std::endl;
	std::cout << "    Actual output:   " << *s0 << std::endl;
	
	std::cout << "  Testing Copy Constructor..." << std::endl;
	Point s1(*((Point*)s0));
	std::cout << "    Expected output: " << *s0 << std::endl;
	std::cout << "    Actual output:   " << s1 << std::endl;
	
	std::cout << "  Testing Operator=..." << std::endl;
	Point s3(22,222,2222,GraphicsContext::BLACK);
	*s0 = s3;
	std::cout << "    Expected output: " << s3 << std::endl;
	std::cout << "    Actual output:   " << *s0 << std::endl;
	delete s0;
	
	std::cout << "  Testing Draw...";
	s0 = new Point(500, 500, 0, GraphicsContext::WHITE);
	s0->draw(gc);
	bool success = gc->getPixel(500, 500) == GraphicsContext::WHITE;
	if (success)
		std::cout << " OK!" << std::endl;
	else
		std::cout << " FAILED!" << std::endl;
	delete s0;
	
	std::cout << "  Testing Clone..." << std::endl;
	s0 = s1.clone();
	std::cout << "    Expected output: " << s1 << std::endl;
	std::cout << "    Actual output:   " << *s0 << std::endl;
	delete s0;
}

static void testLine(GraphicsContext *gc)
{
	std::cout << "Testing Line..." << std::endl;
	
	std::cout << "  Allocating Line(pts,color) on the heap..." << std::endl;
	matrix pts(3,2);
	pts[0][0] = 255;	pts[0][1] = 33;
	pts[1][0] = 255;	pts[1][1] = 22;
	pts[2][0] = 0;		pts[2][0] = 0;
	Shape *s0 = new Line(pts, GraphicsContext::WHITE);
	
	// Set space level for prompt text of "    Actual output:   " for nice printing
	const unsigned int SPACE_LEVEL = sizeof("    Actual output:   ")-1;
	s0->setSpaceLevel(SPACE_LEVEL);
	
	// Testing Operator<<
	std::cout << "  Testing Operator<<..." << std::endl;
	std::cout << "    Expected output: l(color=0xFFFFFF p1=[255 255 0 1]'" << std::endl;
	std::cout << "                                      p2=[33 22 0 1]')" << std::endl;
	std::cout << "    Actual output:   " << *s0 << std::endl;
	
	// Testing Operator>>
	std::cout << "  Testing Operator>>..." << std::endl;
	std::string inStr("l(color=0x00FF00 p1=[0 1.1 2.2 3.3]'\n");
	inStr += std::string(SPACE_LEVEL, ' ') + "                 p2=[5.5 6.6 7.7 8.8]')";
	std::stringstream ss(inStr);
	ss >> *s0;
	std::cout << "    Expected output: " << inStr << std::endl;
	std::cout << "    Actual output:   " << *s0 << std::endl;
	
	// Testing Copy Constructor
	std::cout << "  Testing Copy Constructor..." << std::endl;
	Line s1(*((Line*)s0));
	std::cout << "    Expected output: " << *s0 << std::endl;
	std::cout << "    Actual output:   " << s1 << std::endl;
	
	// Testing Operator=
	std::cout << "  Testing Operator=..." << std::endl;
	pts[0][0] = 222;	pts[0][1] = 0;
	pts[1][0] = 222;	pts[1][1] = 0;
	pts[2][0] = 0;		pts[2][0] = 0;
	Line s3(pts,GraphicsContext::BLUE);
	s3.setSpaceLevel(SPACE_LEVEL); // set first line startpad level
	*s0 = s3;
	std::cout << "    Expected output: " << s3 << std::endl;
	std::cout << "    Actual output:   " << *s0 << std::endl;
	
	// Testing Draw
	std::cout << "  Testing Draw...";
	s0->draw(gc);
	bool success = gc->getPixel(222, 222) == GraphicsContext::BLUE
			&& gc->getPixel(0,0) == GraphicsContext::BLUE;
	if (success)
		std::cout << " OK!" << std::endl;
	else
		std::cout << " FAILED!" << std::endl;
	delete s0;
	
	// Testing Clone
	std::cout << "  Testing Clone..." << std::endl;
	s0 = s1.clone();
	std::cout << "    Expected output: " << s1 << std::endl;
	std::cout << "    Actual output:   " << *s0 << std::endl;
	delete s0;
}