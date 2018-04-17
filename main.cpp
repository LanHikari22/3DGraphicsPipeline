#include "x11context.h"
#include <unistd.h>
#include <iostream>
#include <cmath>	// for trig functions

// function forwarding
void testDrawLine(GraphicsContext *gc);
void testDrawCircle(GraphicsContext *gc); 
void testDrawLine_CircleSegments(GraphicsContext *gc, int x0, int y0, unsigned int radius);


int main(void)
{
	GraphicsContext* gc = new X11Context(800,600,GraphicsContext::BLACK);
 
 
	// draw some lines
	std::cout << "testing basic... Square and Circle" << std::endl;
	gc->setColor(GraphicsContext::GREEN);
	gc->setPixel(10,10);
	gc->setPixel(30,30);
	gc->drawLine(100,100,100,500);
	gc->setColor(GraphicsContext::RED);
	gc->drawLine(100,500,500,500);
	gc->setColor(GraphicsContext::BLUE);
	gc->drawLine(500,500,500,100);
	gc->setColor(GraphicsContext::YELLOW);
	gc->drawLine(500,100,100,100);
	gc->setColor(GraphicsContext::MAGENTA);
	gc->drawCircle(300,300,200);
	sleep(1);
	gc->clear();
	
	testDrawLine(gc);
	testDrawCircle(gc);
	
	// Draw something cool!

  /* initialize random seed: */
  srand (time(NULL));

  // Loop until cont is 22!
  int cont = rand() % 23;
  while (cont != 22)
  {
	  int isCircle = rand() % 2; // 1 or 0...
	  int x0 = rand() % gc->getWindowWidth();
	  int y0 = rand() % gc->getWindowHeight();
	  if (isCircle)
	  {
		  int r = rand() % 500;
		  gc->setColor(GraphicsContext::MAGENTA);
		  gc->drawCircle(x0,y0,r);
	  }
	  else
	  {
		  int x1 = rand() % gc->getWindowWidth();
		  int y1 = rand() % gc->getWindowHeight();
		  gc->setColor(GraphicsContext::CYAN);
		  gc->drawLine(x0,y0,x1,y1);

	  }

	  // Loop until cont is 22!
	  cont = rand() % 23;

  }
    
	sleep(2);
	delete gc;
 
	return 0;
}

// this is actually the naive circle impelementation
// it makes for a very solid test case. It tests for many slopes
// and it also tests for all quadrants
void testDrawLine_CircleSegments(GraphicsContext *gc, int x0, int y0, unsigned int radius)
{
	// This is a naive implementation that draws many line
	// segments.  Also uses floating point math for poor performance

	// also, large circles will be "jagged"
	
	// start at 0 degrees
	int oldx = radius;
	int oldy = 0;

	// go from 1 to 360 degrees
	for (int segment =1; segment<=360; segment += 1)
	{
		int newx = std::cos(segment*M_PI/180)*radius;
		int newy = std::sin(segment*M_PI/180)*radius;

		// hit four quadrants
		gc->drawLine(x0+oldx,y0+oldy,x0+newx,y0+newy);
		
		oldx = newx;
		oldy = newy;
		
	}
	
	return;	
}

void testDrawLine(GraphicsContext *gc)
{
	std::cout << "Testing GraphicsContext::DrawLine()..." << std::endl;
	
	std::cout << "\tTesting initial Circle algorithm - Segments of many slopes... " << std::endl;
	gc->setColor(GraphicsContext::BLUE);
	testDrawLine_CircleSegments(gc, 300,300,150);
	sleep(1);
	
	
	std::cout << "\tTesting that segment endpoints are both lit... ";
	gc->setColor(GraphicsContext::CYAN);
	gc->drawLine(0,0,30,30);
	if (gc->getPixel(0,0) == GraphicsContext::CYAN && gc->getPixel(30,30) == GraphicsContext::CYAN)
	{
		std::cout << "OK!" << std::endl;
	}
	else
	{
		std::cout << "FAILED!" << std::endl;
	}
	sleep(1);
			
	std::cout << "\tTesting Segments with coordinates out of range... ";
	gc->setColor(GraphicsContext::WHITE);
	gc->drawLine(0,0,-999,-9999);
	gc->drawLine(-200,300,500,700);
	gc->drawLine(-1000,-1000,1000,1000);
	std::cout << "OK. not weird!" << std::endl;
	sleep(1);
	
	std::cout << "\tTesting No Segment (0,0)->(0,0)... ";
	gc->setColor(GraphicsContext::RED);
	gc->drawLine(0,0,0,0);
	if (gc->getPixel(0,0) == GraphicsContext::RED)
	{
		std::cout << "OK! Draws Pixel!" << std::endl;
	}
	else
	{
		std::cout << "FAILED! NO DRAWING" << std::endl;
	}
	sleep(1);
	
	std::cout << "Tesing Range. Drawing an X on the whole screen..." << std::endl;
	gc->drawLine(0,0,gc->getWindowWidth(), gc->getWindowHeight());
	gc->drawLine(0, gc->getWindowHeight(), gc->getWindowWidth(), 0);
	sleep(1);
	
	gc->clear();
	
}
void testDrawCircle(GraphicsContext *gc)
{
	std::cout << "Testing GraphicsContext::DrawCircle()..." << std::endl;
	
	std::cout << "\tTest filled circle in the middle..." << std::endl;
	gc->setColor(GraphicsContext::GREEN);
	for (unsigned int i=0; i<200; i++)
	{
		gc->drawCircle(gc->getWindowWidth()>>1, gc->getWindowHeight()>>1, i);
	}
	sleep(1);
	
	std::cout << "Nothing bad should happen out of range..." << std::endl;
	gc->drawCircle(gc->getWindowWidth()<<2, gc->getWindowHeight()<<2, 100);

	std::cout << "Nothing bad should happen with big radius..." << std::endl;
	gc->drawCircle(gc->getWindowWidth()>>2, gc->getWindowHeight()>>2, 10000);

	
	std::cout << "\tTesting 0 Radius. Should light up pixel... ";
	gc->setColor(GraphicsContext::GRAY);
	gc->drawCircle(500,500, 0);
	if (gc->getPixel(500,500) == GraphicsContext::GRAY)
	{
		std::cout << "OK!" << std::endl;
	}
	else
	{
		std::cout << "FAILED!" << std::endl;
	}
	sleep(1);
	
	std::cout << "\tTesting 4 Corner Circles. Should partially display..." << std::endl;
	gc->setColor(GraphicsContext::RED);
	gc->drawCircle(0,0,200);
	gc->drawCircle(gc->getWindowWidth(), 0, 200);
	gc->drawCircle(0, gc->getWindowHeight(), 200);
	gc->drawCircle(gc->getWindowWidth(), gc->getWindowHeight(), 200);
	
	std::cout << "\tTesting 4 bigger Corner Circles. Should still look circle..." << std::endl;
		gc->setColor(GraphicsContext::GRAY);
		gc->drawCircle(0, 0, 500);
		gc->drawCircle(gc->getWindowWidth(), 0, 500);
		gc->drawCircle(0, gc->getWindowHeight(), 500);
		gc->drawCircle(gc->getWindowWidth(), gc->getWindowHeight(), 500);
	sleep(2);
	
	gc->clear();
}

