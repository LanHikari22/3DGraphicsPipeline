/* This is an abstract base class representing a generic graphics
 * context.  Most implementation specifics will need to be provided by
 * a concrete implementation.  See header file for specifics. */

#define _USE_MATH_DEFINES	// for M_PI
#include <cmath>	// for trig functions
#include <iostream> // for debugging
#include "gcontext.h"	


/*
 * Destructor - does nothing
 */
GraphicsContext::~GraphicsContext()
{
	// nothing to do
	// here to insure subclasses handle destruction properly
}


/* Bresenham's line algorithm -- No floating point arithmetic.
 * Only integer add/subtract and bit shifting
 * 
 * Parameters:
 * 	x0, y0 - origin of line
 *  x1, y1 - end of line
 * 
 * Returns: void
 */ 
void GraphicsContext::drawLine(int x0, int y0, int x1, int y1)
{
	// find slope
	int dx = x1-x0;
	int dy = y1-y0;
	
	// Figure out whether to increment or decrement x and y (quadrant dependent)
	int xinc = (dx > 0) ? 1 : -1;
	int yinc = (dy > 0) ? 1 : -1;
	
	// We have all we need out of sign information 
	// Set dx/dy to abs to be used to compute d
	dx = (dx < 0) ? -dx : dx;
	dy = (dy < 0) ? -dy : dy;
	
	// determine vertical mode (m>1) vs horizontal mode (m<=1)
	if (dx > dy) /* horizontal mode */
	{	
		// determine factor -- d0
		int d = (dy<<1) - dx;
		int y = y0;
		
		// scan convert!
		for (int x=x0; x!=x1; x+=xinc)
		{
			setPixel(x,y);
			// decide to increment y or not for next iteration
			if (d > 0)
			{
				y += yinc;
				d = d - (dx<<1);
			}
			d = d + (dy<<1);
		}	
	}
	else /* vertical mode */
	{
		// determine factor -- d0
		int d = (dx<<1) - dy;
		int x = x0;
		
		// scan convert!
		for (int y=y0; y!=y1; y+=yinc)
		{
			setPixel(x,y);
			// decide to increment y or not for next iteration
			if (d > 0)
			{
				x += xinc;
				d = d - (dy<<1);
			}
			d = d + (dx<<1);
		}	
	}
	// catch last point!
	setPixel(x1, y1);
	
	return;
}



/* This is a naive implementation that uses floating-point math
 * and "setPixel" which will need to be provided by the concrete
 * implementation.
 * 
 * Parameters:
 * 	x0, y0 - origin/center of circle
 *  radius - radius of circle
 * 
 * Returns: void
 */
void GraphicsContext::drawCircle(int x0, int y0, unsigned int radius)
{
	// If you want to draw a pixel just call setPixel!
	if (radius == 0)
	{
		setPixel(x0,y0);
	}
	else
	{		
		// di = (xi+1)^2 + (yi+0.5)^2 - r^2
		//    = xi^2 + 2xi + yi^2 + 5/4 - r^2
		// d_i+1 = { di + 2*y + 1; if di <= 0
		//			 di + 2*y - 2*x if di > 0 }
		
		// As far as the error is concerned, we can simplify by assuming we
		// are at the origin (x0, y0) = (0,0)
		// init d to d0 = 5/4 + r, which can round down to 1 + r
		int d = 1 + radius;
		
		// scan convert the first 45 deg, until x == y
		// scanning x right, y down from topleft pixel
		for (int x=0, y=radius; x<=y; x++)
		{
			// The circle is highly symmetrical over its 8 octants
			// Symmetrically, 8 points can be computed and drawn at a time
	        setPixel(x0 + x, y0 + y);
	        setPixel(x0 + y, y0 + x);
	        setPixel(x0 - y, y0 + x);
	        setPixel(x0 - x, y0 + y);
	        setPixel(x0 - x, y0 - y);
	        setPixel(x0 - y, y0 - x);
	        setPixel(x0 + y, y0 - x);
	        setPixel(x0 + x, y0 - y);
	        
	        // if d is within the circle, y should increase
	        if (d <= 0)
	        {
	        	y--;
	        	d = d + 2*y + 1;
	        }
	        d = d - 2*x;
		}
	}
	
	return;	
}

void GraphicsContext::endLoop()
{
	run = false;
}


