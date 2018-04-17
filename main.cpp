#include "x11context.h"
#include "Point.h"
#include <unistd.h>
#include <iostream>

static void waitEnter();


int main(void)
{
	GraphicsContext* gc = new X11Context(800,600,GraphicsContext::BLACK);
 
	Shape *s = new Point(255, 255, 0, GraphicsContext::WHITE);
	
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

