#include "x11context.h"
#include "Point.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <sstream>

static void waitEnter();
static void testPoint(GraphicsContext *gc);

int main(void)
{
	GraphicsContext *gc = new X11Context(800,600,GraphicsContext::BLACK);
 
	// Test Shapes
	testPoint(gc);
	
    // Exit when pressing enter
	waitEnter();
	
	delete gc;
 
	return 0;
}

static void testPoint(GraphicsContext *gc)
{
	std::cout << "testing Point..." << std::endl;
	
	std::cout << "\tAllocating Point(x,y,z,color) on the heap..." << std::endl;
	Shape *s0 = new Point(255, 255, 0, GraphicsContext::WHITE);
	
	std::cout << "\tTesting Operator<<..." << std::endl;
	std::cout << "\t\tExpected output: p(color=0xFFFFFF p1=[255 255 0 1]')" << std::endl;
	std::cout << "\t\tActual output:   " << *s0 << std::endl;
	
	std::cout << "\tTesting Operator>>..." << std::endl;
	std::string inStr("p(color=0xFF00 p1=[0 1.1 2.2 3.3]')");
	std::stringstream ss(inStr);
	ss >> *s0;
	std::cout << "\t\tExpected output: " << inStr << std::endl;
	std::cout << "\t\tActual output:   " << *s0 << std::endl;
	
	std::cout << "\tTesting Copy Constructor..." << std::endl;
	Point s1(*((Point*)s0));
	std::cout << "\t\tExpected output: " << *s0 << std::endl;
	std::cout << "\t\tActual output:   " << s1 << std::endl;
	
	std::cout << "\tTesting Operator=..." << std::endl;
	Point s3(22,222,2222,GraphicsContext::BLACK);
	*s0 = s3;
	std::cout << "\t\tExpected output: " << s3 << std::endl;
	std::cout << "\t\tActual output:   " << *s0 << std::endl;
	delete s0;
	
	std::cout << "\tTesting Draw...";
	s0 = new Point(500, 500, 0, GraphicsContext::WHITE);
	s0->draw(gc);
	bool success = gc->getPixel(500, 500) == GraphicsContext::WHITE;
	if (success)
		std::cout << " OK!" << std::endl;
	else
		std::cout << " FAILED!" << std::endl;
	delete s0;
	
	std::cout << "\tTesting Clone..." << std::endl;
	s0 = s1.clone();
	std::cout << "\t\tExpected output: " << s1 << std::endl;
	std::cout << "\t\tActual output:   " << *s0 << std::endl;
	delete s0;

	
}

// waits until enter is pressed in the terminal
static void waitEnter()
{
	std::string line;
	std::getline(std::cin, line);
}

