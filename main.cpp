#include "x11context.h"

#include "Image.h"
#include "matrix.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>


// prompts user to press Enter, and pauses until they do
static void waitEnter();

// Tests overall functionaly of the Point class
static void testPoint(GraphicsContext *gc);
// Tests overall functionality of the Line class
static void testLine(GraphicsContext *gc);
// Tests overall functional of the Triangle class
static void testTriangle(GraphicsContext *gc);
// Tests overall functionality of the Circle class
static void testCircle(GraphicsContext *gc);
// Tests overall functionality of the Rectangle class
static void testRectangle(GraphicsContext *gc);

// Tests overall functionality of the image class
static void testImage(GraphicsContext *gc);
// Generates a file full of randoms hapes which is used for testing the Image class
// for the ability to both parse a random number of images, and also drawing them...
// @returns filename
static std::string generateRandomImageFile(GraphicsContext* gc);
static void testImage_DrawFile(GraphicsContext *gc, std::string filename);

int main(void)
{
	GraphicsContext *gc = new X11Context(800,600,GraphicsContext::BLACK);
 
	// Test Shapes
	std::cout << "Testing Shapes..." << std::endl;
	testPoint(gc);
	testLine(gc);
	testTriangle(gc);
	testCircle(gc);
	testRectangle(gc);
	waitEnter();
	
	// Test the Image class
	gc->clear();
	testImage(gc);
	waitEnter();
	
	// Now Draw content from a file!
	gc->clear();
	std::string filename = generateRandomImageFile(gc);
	testImage_DrawFile(gc, filename);
	
    // Exit when pressing enter
	std::cout << "Testing Complete!" << std::endl;
	waitEnter();
	
	delete gc;
 
	return 0;
}



// waits until enter is pressed in the terminal
static void waitEnter()
{
	std::cout << "Press Enter to continue..." << std::endl;
	std::string line;
	std::getline(std::cin, line);
}

static void testImage(GraphicsContext *gc)
{
	Image *img0 = new Image();
	
	std::cout << "Testing Image..." << std::endl;
	
	// Set space level for prompt text for nice printing when shapes printed are not
	// at the start of the line
	const unsigned int SL = sizeof("    Expected output: ")-1;
	img0->setSpaceLevel(SL);
	
	// Make up a few shapes
	const int CP = GraphicsContext::RED;
	matrix mp(3,1);
	mp[0][0] = 444;
	mp[1][0] = 222;
	mp[2][0] = 0;
	Shape *sp = new Point(mp[0][0], mp[1][0], mp[2][0], CP);
	
	const int CL = GraphicsContext::BLUE;
	matrix ml(3,2);
	ml[0][0] = 400;	ml[0][1] = 33;
	ml[1][0] = 100;	ml[1][1] = 22;
	ml[2][0] = 0;	ml[2][0] = 0;
	Shape *sl = new Line(ml, CL);
	
	const int CT = GraphicsContext::WHITE;
	matrix mt = matrix(3,3);
	mt[0][0] = 100;	mt[0][1] = 250;	mt[0][2] = 0;
	mt[1][0] = 500;	mt[1][1] = 500;	mt[1][2] = 0;
	mt[2][0] = 0;	mt[2][0] = 0;	mt[2][2] = 0;
	Shape *st = new Triangle(mt, CT);
	
	const int CC = GraphicsContext::MAGENTA;
	matrix mc(3,2);
	mc[0][0] = 0;	mc[0][1] = 256;
	mc[1][0] = 0;	mc[1][1] = 400;
	mc[2][0] = 0;	mc[2][0] = 0;
	Shape *sc = new Circle(mc, CC);
	
	const int CR = GraphicsContext::YELLOW;
	matrix mr(3,1);
	mr[0][0] = gc->getWindowWidth()/2;
	mr[1][0] = gc->getWindowHeight()/2;
	mr[2][0] = 0;
	const int rw = mr[0][0];
	const int rh = mr[1][0];
	Shape *sr = new Rectangle(mr, rw, rh, CR);
	
	// First, add point shape twice, should display it twice
	img0->add(sp);
	img0->add(sp);
	std::cout << "  Testing Add, Basic..." << std::endl;
	std::cout << "    Expected Output: " <<  *sp << std::endl;
	std::cout << "                     " << *sp << std::endl;
	std::cout << "    Actual Output:   " << *img0 << std::endl;
	
	// Erase, then try to print...
	std::cout << "  Testing Erase..." << std::endl;
	img0->erase();
	std::cout << "    Displaying sp... Should still be there" << std::endl;
	std::cout << "    Output sp:       " << *sp << std::endl;
	std::cout << "    Displaying img0... Shouldn't have anything" << std::endl;
	std::cout << "    Output *img0:    " << *img0 << std::endl;
	
	// Copy Constructor
	std::cout << "  Testing Copy Constructor..." << std::endl;
	img0->add(sp);
	Image img1 = *img0;
	std::cout << "    Expected Output: " << *img0 << std::endl;
	std::cout << "    Actual Output:   " <<  img1 << std::endl;
	
	// check deep-copy
	std::cout << "  Testing for deep copy..." << std::endl;
	std::cout << "  Deleting sp... Shouldn't affect img0" << std::endl;
	std::cout << "    Expected Output: " << *img0 << std::endl;
	delete sp;
	std::cout << "    Actual Output:   " << *img0 << std::endl;
	
	// Operator=
	img1.add(sl);
	*img0 = img1;
	std::cout << "  Testing Operator=..." << std::endl;
	std::cout << "    Expected Output: " << img1 << std::endl;
	std::cout << "    Actual Output:   " << *img0 << std::endl;
	
	// Draw: All three shapes together, test overall deep copy capabilities
	std::cout << "  Testing Draw... ";
	img1.add(st);
	img1.add(sc);
	img1.add(sr);
	*img0 = img1;
	img0->draw(gc);
	// Test that all of the vertices of each shape have been drawn
	bool verticesDrawn = true;
	// Test point sp vertices
	verticesDrawn &= gc->getPixel(mp[0][0], mp[1][0]) == CP;
	// Test line sl vertices
	verticesDrawn &= (gc->getPixel(ml[0][0], ml[1][0]) == CL)
				   & (gc->getPixel(ml[0][1], ml[1][1]) == CL);
	// Test triangle st vertices
	verticesDrawn &= (gc->getPixel(mt[0][0], mt[1][0]) == CT)
				   & (gc->getPixel(mt[0][1], mt[1][1]) == CT)
				   & (gc->getPixel(mt[0][2], mt[1][2]) == CT);
	// Test circle sc vertices
	verticesDrawn &= (gc->getPixel(mc[0][1], mc[1][1]) == CC);
	// Test Rectangle st vertices
	verticesDrawn &= (gc->getPixel(mr[0][0]-rw/2, mr[1][0]-rh/2) == CR)
				   & (gc->getPixel(mr[0][0]+rw/2, mr[1][0]-rh/2) == CR)
				   & (gc->getPixel(mr[0][0]+rw/2, mr[1][0]+rh/2) == CR)
	   	   	   	   & (gc->getPixel(mr[0][0]-rw/2, mr[1][0]+rh/2) == CR);
	
	if (verticesDrawn)
	{
		std::cout << "OK!" << std::endl;
	} else {
		std::cout << "FAILED!" << std::endl;
	}
	
	// Operator>>
	std::cout << "  Testing Operator>>..." << std::endl;
	std::cout << "    Copying img0 to stringstream then to imag1..." << std::endl;
	std::stringstream ss;
	Image img2;
	img2.setSpaceLevel(SL);
	ss << *img0;
	ss >> img2;
	std::cout << "    Expected Output: " << *img0 << std::endl;
	std::cout << "    Actual Output:   " << img2 << std::endl;
	
//	delete sp;	// Already deleted!
	delete sl;
	delete st;
	delete sc;
	delete sr;

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
	
	// Test Exceptions
	std::cout << "  Testing Exceptions..." << std::endl;
	
	std::cout << "  Testing Parsing Exception is thrown... ";
	try
	{
		ss = std::stringstream("Random Text. Cannot Parse this, haha!");
		ss >> s3;
		std::cout << "NOT THROWN!" << std::endl;
	} catch (shapeException)
	{
		std::cout << "thrown!" << std::endl;
	}
	
	std::cout << "  Testing Drawing 3D Point...";
	try
	{
		Point s4(0,0,1,GraphicsContext::RED);
		s4.draw(gc);
		std::cout << "NOT THROWN!" << std::endl;
	} catch (shapeException)
	{
		std::cout << "thrown!" << std::endl;
	}
	
}

static void testLine(GraphicsContext *gc)
{
	std::cout << "Testing Line..." << std::endl;
	
	std::cout << "  Allocating Line(pts,color) on the heap..." << std::endl;
	matrix pts(3,2);
	pts[0][0] = 255;	pts[0][1] = 33;
	pts[1][0] = 255;	pts[1][1] = 22;
	pts[2][0] = 0;		pts[2][1] = 0;
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

	// Test Exceptions
	std::cout << "  Testing Exceptions..." << std::endl;
	
	std::cout << "  Testing Parsing Exception is thrown... ";
	try
	{
		ss = std::stringstream("Random Text. Cannot Parse this, haha!");
		ss >> s3;
		std::cout << "NOT THROWN!" << std::endl;
	} catch (shapeException)
	{
		std::cout << "thrown!" << std::endl;
	}
	
	std::cout << "  Testing Drawing 3D Point...";
	try
	{
		pts[2][0] = 1; pts[2][1] = 1;
		Line s4(pts,GraphicsContext::RED);
		s4.draw(gc);
		std::cout << "NOT THROWN!" << std::endl;
	} catch (shapeException)
	{
		std::cout << "thrown!" << std::endl;
	}
	
}

static void testTriangle(GraphicsContext *gc)
{
	std::cout << "Testing Triangle..." << std::endl;
	
	std::cout << "  Allocating Triangle(pts,color) on the heap..." << std::endl;
	matrix pts(3,3);
	pts[0][0] = 500;	pts[0][1] = 250;	pts[0][2] = 0;
	pts[1][0] = 500;	pts[1][1] = 250;	pts[1][2] = 0;
	pts[2][0] = 0;		pts[2][1] = 0;		pts[2][2] = 0;
	Shape *s0 = new Triangle(pts, GraphicsContext::RED);
	
	// Set space level for prompt text of "    Actual output:   " for nice printing
	const unsigned int SPACE_LEVEL = sizeof("    Actual output:   ")-1;
	s0->setSpaceLevel(SPACE_LEVEL);
	
	// Testing Operator<<
	std::cout << "  Testing Operator<<..." << std::endl;
	std::cout << "    Expected output: t(color=0xFF0000 p1=[500 500 0 1]'" << std::endl;
	std::cout << "                                      p2=[250 250 0 1]'" << std::endl;
	std::cout << "                                      p3=[0 0 0 1]')" << std::endl;
	std::cout << "    Actual output:   " << *s0 << std::endl;
	
	// Testing Operator>>
	std::cout << "  Testing Operator>>..." << std::endl;
	std::string inStr("l(color=0x00FFFF p1=[0 1 2 3.3]'\n");
	inStr += std::string(SPACE_LEVEL, ' ') + "                 p2=[4 5 6 7.7]'\n";
	inStr += std::string(SPACE_LEVEL, ' ') + "                 p3=[8 9 10 11]')";
	std::stringstream ss(inStr);
	ss >> *s0;
	std::cout << "    Expected output: " << inStr << std::endl;
	std::cout << "    Actual output:   " << *s0 << std::endl;
	
	// Testing Copy Constructor
	std::cout << "  Testing Copy Constructor..." << std::endl;
	Triangle s1(*((Triangle*)s0));
	std::cout << "    Expected output: " << *s0 << std::endl;
	std::cout << "    Actual output:   " << s1 << std::endl;
	
	// Testing Operator=
	std::cout << "  Testing Operator=..." << std::endl;
	pts[0][0] = 200;	pts[0][1] = 222;		pts[0][2] = 500;
	pts[1][0] = 599;	pts[1][1] = 333;		pts[1][2] = 234;
	pts[2][0] = 0;		pts[2][1] = 0;			pts[2][2] = 0;
	Triangle s3(pts,GraphicsContext::CYAN);
	s3.setSpaceLevel(SPACE_LEVEL); // set first line startpad level
	*s0 = s3;
	std::cout << "    Expected output: " << s3 << std::endl;
	std::cout << "    Actual output:   " << *s0 << std::endl;
	
	// Testing Draw
	std::cout << "  Testing Draw...";
	s0->draw(gc);
	bool success = gc->getPixel(pts[0][0], pts[1][0]) == GraphicsContext::CYAN
			&& gc->getPixel(pts[0][1],pts[1][1]) == GraphicsContext::CYAN
			&& gc->getPixel(pts[0][2],pts[1][2]) == GraphicsContext::CYAN;
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
	
	// Test Exceptions
	std::cout << "  Testing Exceptions..." << std::endl;
	
	std::cout << "  Testing Parsing Exception is thrown... ";
	try
	{
		ss = std::stringstream("c(color=Random Text. , p=haha!");
		ss >> s3;
		std::cout << "NOT THROWN!" << std::endl;
	} catch (shapeException)
	{
		std::cout << "thrown!" << std::endl;
	}
	
	std::cout << "  Testing Drawing 3D Point...";
	try
	{
		pts[2][0] = 1; pts[2][1] = 1; pts[2][2] = 1;
		Triangle s4(pts,GraphicsContext::RED);
		s4.draw(gc);
		std::cout << "NOT THROWN!" << std::endl;
	} catch (shapeException)
	{
		std::cout << "thrown!" << std::endl;
	}
}

static void testCircle(GraphicsContext *gc)
{
	std::cout << "Testing Circle..." << std::endl;
	
	std::cout << "  Allocating Circle(pts,color) on the heap..." << std::endl;
	matrix pts(3,2);
	pts[0][0] = 255;	pts[0][1] = 33;
	pts[1][0] = 255;	pts[1][1] = 22;
	pts[2][0] = 0;		pts[2][1] = 0;
	Shape *s0 = new Circle(pts, GraphicsContext::MAGENTA);
	
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
	std::string inStr("c(color=0x00FF00 p1=[0 1.1 2.2 3.3]'\n");
	inStr += std::string(SPACE_LEVEL, ' ') + "                 p2=[5.5 6.6 7.7 8.8]')";
	std::stringstream ss(inStr);
	ss >> *s0;
	std::cout << "    Expected output: " << inStr << std::endl;
	std::cout << "    Actual output:   " << *s0 << std::endl;
	
	// Testing Copy Constructor
	std::cout << "  Testing Copy Constructor..." << std::endl;
	Circle s1(*((Circle*)s0));
	std::cout << "    Expected output: " << *s0 << std::endl;
	std::cout << "    Actual output:   " << s1 << std::endl;
	
	// Testing Operator=
	std::cout << "  Testing Operator=..." << std::endl;
	const int C = GraphicsContext::BLUE;
	pts[0][0] = 0;	pts[0][1] = 256;
	pts[1][0] = 0;	pts[1][1] = 0;
	pts[2][0] = 0;	pts[2][1] = 0;
	Circle s3(pts,GraphicsContext::BLUE);
	s3.setSpaceLevel(SPACE_LEVEL); // set first line startpad level
	*s0 = s3;
	std::cout << "    Expected output: " << s3 << std::endl;
	std::cout << "    Actual output:   " << *s0 << std::endl;
	
	// Testing Draw
	std::cout << "  Testing Draw...";
	s0->draw(gc);
	bool success = gc->getPixel(pts[0][1], pts[1][1]) == C;
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

	// Test Exceptions
	std::cout << "  Testing Exceptions..." << std::endl;
	
	std::cout << "  Testing Parsing Exception is thrown... ";
	try
	{
		ss = std::stringstream("Random Text. Cannot Parse this, haha!]')");
		ss >> s3;
		std::cout << "NOT THROWN!" << std::endl;
	} catch (shapeException)
	{
		std::cout << "thrown!" << std::endl;
	}
	
	std::cout << "  Testing Drawing 3D Point...";
	try
	{
		pts[2][0] = 1; pts[2][1] = 1;
		Circle s4(pts,GraphicsContext::RED);
		s4.draw(gc);
		std::cout << "NOT THROWN!" << std::endl;
	} catch (shapeException)
	{
		std::cout << "thrown!" << std::endl;
	}
	
}

static void testRectangle(GraphicsContext *gc)
{
	std::cout << "Testing Rectangle..." << std::endl;
	
	std::cout << "  Allocating Rectangle(pts,color) on the heap..." << std::endl;
	matrix pts(3,4);
	pts[0][0] = 250;	pts[0][1] = 500;	pts[0][2] = 500;
	pts[1][0] = 250;	pts[1][1] = 250;	pts[1][2] = 500;
	pts[2][0] = 0;		pts[2][1] = 0;		pts[2][2] = 0;
	pts[0][3] = 250;
	pts[1][3] = 500;
	pts[2][3] = 0;
	Shape *s0 = new Rectangle(pts, GraphicsContext::YELLOW);
	
	// Set space level for prompt text of "    Actual output:   " for nice printing
	const unsigned int SPACE_LEVEL = sizeof("    Actual output:   ")-1;
	s0->setSpaceLevel(SPACE_LEVEL);
	
	// Testing Operator<<
	std::cout << "  Testing Operator<<..." << std::endl;
	std::cout << "    Expected output: r(color=0xFF0000 p1=[250 250 0 1]'" << std::endl;
	std::cout << "                                      p2=[500 250 0 1]'" << std::endl;
	std::cout << "                                      p3=[500 500 0 1]'" << std::endl;
	std::cout << "                                      p4=[250 500 0 1]')" << std::endl;
	std::cout << "    Actual output:   " << *s0 << std::endl;
	
	// Testing Operator>>
	std::cout << "  Testing Operator>>..." << std::endl;
	std::string spacePad = std::string(SPACE_LEVEL, ' ');
	std::string inStr("l(color=0x00FFFF p1=[50 50 0 1]'\n");
	inStr += spacePad  + "                 p2=[100 50 0 1]'\n";
	inStr += spacePad  + "                 p3=[100 100 0 1]'\n";
	inStr += spacePad  + "                 p4=[50 100 0 1]')";
	std::stringstream ss(inStr);
	ss >> *s0;
	std::cout << "    Expected output: " << inStr << std::endl;
	std::cout << "    Actual output:   " << *s0 << std::endl;
	
	// Testing Copy Constructor
	std::cout << "  Testing Copy Constructor..." << std::endl;
	Rectangle s1(*((Rectangle*)s0));
	std::cout << "    Expected output: " << *s0 << std::endl;
	std::cout << "    Actual output:   " << s1 << std::endl;
	
	// Testing Operator=
	std::cout << "  Testing Operator=..." << std::endl;
	double w3 = 111.1;
	double h3= 222.2;
	Rectangle s3(pts, w3, h3,GraphicsContext::GREEN);
	s3.setSpaceLevel(SPACE_LEVEL); // set first line startpad level
	*s0 = s3;
	std::cout << "    Expected output: " << s3 << std::endl;
	std::cout << "    Actual output:   " << *s0 << std::endl;
	
	// Testing Draw
	std::cout << "  Testing Draw...";
	Rectangle s4(pts, GraphicsContext::GREEN);
	s4.draw(gc);
	bool success = gc->getPixel(pts[0][0], pts[1][0]) == GraphicsContext::GREEN
			    && gc->getPixel(pts[0][1], pts[1][1]) == GraphicsContext::GREEN
			    && gc->getPixel(pts[0][2], pts[1][2]) == GraphicsContext::GREEN
			    && gc->getPixel(pts[0][3], pts[1][3]) == GraphicsContext::GREEN;
	if (success)
		std::cout << " OK!" << std::endl;
	else
		std::cout << " FAILED!" << std::endl;
	
	// Testing Clone
	delete s0;
	std::cout << "  Testing Clone..." << std::endl;
	s0 = s1.clone();
	std::cout << "    Expected output: " << s1 << std::endl;
	std::cout << "    Actual output:   " << *s0 << std::endl;
	delete s0;
	
	// Test Exceptions
	std::cout << "  Testing Exceptions..." << std::endl;
	
	std::cout << "  Testing Parsing Exception is thrown... ";
	try
	{
		ss = std::stringstream("p(color=0x000000 p1=[500 500 500 500]')");
		ss >> s3;
		std::cout << "NOT THROWN!" << std::endl;
	} catch (shapeException)
	{
		std::cout << "thrown!" << std::endl;
	}
	
	std::cout << "  Testing Drawing 3D Point... ";
	try
	{
		pts[2][0] = 1; pts[2][1] = 1; pts[2][2] = 1;
		Rectangle s4(pts,GraphicsContext::RED);
		s4.draw(gc);
		std::cout << "NOT THROWN!" << std::endl;
	} catch (shapeException)
	{
		std::cout << "thrown!" << std::endl;
	}
	
	std::cout << "  Testing Passing non-rectangle vertices... ";
	try
	{
		pts[0][0]++;
		Rectangle s4(pts,GraphicsContext::RED);
		std::cout << "NOT THROWN!" << std::endl;

	} catch (shapeException)
	{
		std::cout << "thrown!" << std::endl;
	}
}


static std::string generateRandomImageFile(GraphicsContext *gc)
{
	/* initialize random seed */
	srand (time(NULL));

	const char SHAPE_SPECIFIERS[] = {'p', 'l', 't', 'c', 'r'};
	
	const std::string FILENAME("Random.img");
	
	std::ofstream ofs(FILENAME);
	int numShapes = rand() % 1000;
	Image *randomImg = new Image();
	
	for (int i=0; i<numShapes; i++)
	{
		
		// this matrix will contain all the data needed for any type of shape!
		int numColumns = 4 + rand()%7; // anywhere from 4 to 10 columns, for polygon shape
		matrix m(3, numColumns);
		
		for (int c=0; c<numColumns; c++)
		{
			m[0][c] = rand() % gc->getWindowWidth();
			m[1][c] = rand() % gc->getWindowHeight();
			m[2][c] = 0;
		}

		switch (SHAPE_SPECIFIERS[rand() % sizeof(SHAPE_SPECIFIERS)])
		{
			case 'p':
			{
				Point p(m[0][0], m[1][0], m[2][0], rand() % 0xFFFFFF); 
				randomImg->add(&p);
			}
			break;
			case 'l':
			{
				Line l(m, rand() % 0xFFFFFF);
				randomImg->add(&l);
			}
			break;
			case 't':
			{
				Triangle t(m, rand() % 0xFFFFFF);
				randomImg->add(&t);
			}
			break;
			case 'c':
			{
				Circle c(m, rand() % 0xFFFFFF);
				randomImg->add(&c);
			}
			break;
			case 'r':
			{
				// those are copmuted width/height relative
				int w = (int)m[0][1];
				int h = (int)m[1][2];
				
				Rectangle r(m, w, h, rand() % 0xFFFFFF);
				randomImg->add(&r);
			}
			break;
		}
	}
	
	ofs << *randomImg;
	
	delete randomImg;
	ofs.close();
	return FILENAME;
}

static void testImage_DrawFile(GraphicsContext *gc, std::string filename)
{
	std::ifstream ifs(filename.c_str());
	Image img;
	ifs >> img;
	ifs.close();
	
	img.draw(gc);
	
}