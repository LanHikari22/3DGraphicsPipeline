#ifndef MYDRAWING_H
#define MYDRAWING_H
#include "drawbase.h"
#include "Image.h"
#include <vector>

// forward reference
class GraphicsContext;

// a helper class to bundle a message with any thrown exceptions.
// To use, simply 'throw drawingException("A descriptive message about
// the problem")'.  This will throw the exception object by value.
// Recommendation is to catch by reference (to prevent slicing).
class drawingException:public std::runtime_error
{
	public:
		drawingException(std::string message):
		      std::runtime_error((std::string("Shape Exception: ") + 
		               message).c_str()) {}
};


class MyDrawing: public DrawingBase {
public:
	// Constructor
	MyDrawing();

	// Destructor, frees state on the heap
	virtual ~MyDrawing();
	
	// we will override just these
	virtual void paint(GraphicsContext *gc);
	
	// This will make sure to set dragging, and XOR mode
	// It will set the coordinates of the hypothetical line to be drawn
	// to be degenerate at first, and it will draw a shape that fits that line
	virtual void mouseButtonDown(GraphicsContext *gc, unsigned int button,
			int x, int y);
	
	// This will make sure to clear dragging, and return to Copy mode
	// it will draw the shape, and add to the image.
	virtual void mouseButtonUp(GraphicsContext *gc, unsigned int button, int x,
			int y);
	
	// This will undraw previous hypotheticla shape that is drawn using
	// the registered hypothetical line. (as a hypothetical box, with the shape
	// contained within it). It will then draw the new hypothetical shape
	virtual void mouseMove(GraphicsContext *gc, int x, int y);
	
	// no need to account for how long a key is held down,
	// but once a key is released, if an action is associated with it,
	// it is handled in this method
	// Commands:
	// Segment Control Commands:
	// Key: Z	Action: Advances to rubberband the next segment
	// Drawing Mode Commands
	// Key: l	Action: Switch to Line Drawing Mode
	// Key: t	Action: Switch to Triangle Drawing Mode
	// Key: r	Action: Switch to Rectangle Drawing Mode
	// Key: c	Action: Switch to Circle Drawing Mode
	// Key: p	Action: Switch to Polygon Drawing Mode
	// Saving Commands
	// Key: i	Action: Loads saved image (if any)
	// Key: o	Action: Saves current image
	// Coloring Commands
	// Key: 0	Action: Set color to Black
	// Key: 1	Action: Set color to Blue
	// Key: 2	Action: Set color to Green
	// Key: 3	Action: Set color to Red
	// Key: 4	Action: Set color to Cyan
	// Key: 5	Action: Set color to Magenta
	// Key: 6	Action: Set color to Yellow
	// Key: 7	Action: Set color to Gray
	// Key: 8	Action: Set color to White
	// @throws drawingException if a key is pressed while dragging
	virtual void keyUp(GraphicsContext* gc, unsigned int keycode);
	
private:

	// With rubberbanding, regardless of the shape being drawn,
	// only one segment needs to be remembered
	std::vector<int> x;
	std::vector<int> y;
	
	// The number of hypothetical segments drawn.
	// With a triangle, there are two. Polygons? arbitrary
	unsigned int numSegments;
	
	// flag to know if we are dragging
	bool dragging;
	
	// an enum fo rthe different drawing modes
	enum class DrawingMode{
		Line, Triangle, Rectangle,
		Circle, Polygon,
	};
	
	// The drawing mode to determine the hypothetical shape to be drawn
	DrawingMode drawingMode;
	
	// the color of the shapes to be drawn. This state can be changed
	// with the keyboard events
	int color;
	
	// signal to be set when the user rubber bands the next segment TODO: needed?
	bool incrementSegments;
	
	
	// Image containing all drawn shapes
	Image *image;
	
	// filename to save to/load from // TODO: how?
//	static const char * const FILENAME = "Saved_Image.img";
	
	
	// Handles all drawing mode switching commands according to the 
	// specifications in KeyUp()
	// if the keycode doesn't match any expected keys, it simply returns
	void handleModeCommands(GraphicsContext* gc, unsigned int keycode);
	
	// Handles all saving/loading commands according to the specifications
	// in keyUp()
	// if the keycode doesn't match any expected keys, it simply returns
	void handleFileCommands(GraphicsContext* gc, unsigned int keycode);
	
	// Handles all color changing commands according to the specifications
	// in keyUp()
	// if the keycode doesn't match any expected keys, it simply returns
	void handleColorCommands(GraphicsContext* gc, unsigned int keycode);
	
	// Helper method that handles checking the drawing mode,
	// and drawing the appropriate shape while dragging.
	// Same operation would undraw those shapes, if in in XOR mode.
	// All of the shapes can be drawn using one hypothetical line segment that
	// represents a box in which they are contained
	// if store: the constructed shape will be added to the image
	// TODO: except for polygon mode
	void drawShape(GraphicsContext *gc, bool store);
	
	// Helper method that handles drawing a triangle within
	// a contained box defined by the hypothetical segment
	// x0,y0 -> x1,y1
	// It draws a line using the startpoint and endpoint
	// using the Line shape class
	// if store: the constructed shape will be added to the image
	void drawLine(GraphicsContext * gc, bool store);
	
	// Helper method that handles drawing a triangle within
	// a contained box defined by the hypothetical segment
	// x0,y0 -> x1,y1
	// It draws a triangle with the base of box's width
	// and the height of the box's height
	// if store: the constructed shape will be added to the image
	void drawTriangle(GraphicsContext *gc, bool store);
	
	// Helper method that handles drawing a rectangle within
	// a containex box defined by the hypothetical segment
	// x0,y0 -> x1,y1
	// It simple draws the box as a rectangle extending from the
	// first point to the second point!
	// if store: the constructed shape will be added to the image
	void drawRectangle(GraphicsContext *gc, bool store);
	
	// Helper method that handles drawing a circle within
	// a containex box defined by the hypothetical segment
	// x0,y0 -> x1,y1
	// start point of the segment will point to the origin
	// while end point will be a point on the circle
	// if store: the constructed shape will be added to the image
	void drawCircle(GraphicsContext *gc, bool store);

	// Helper method that handles drawing a polygon 
	// using an arbitrary number of rubberbanded 
	// segments
	// x0,y0 -> x1,y1 -> ... -> xn,yn -> x0,y0
	// if store: the constructed shape will be added to the image
	void drawPolygon(GraphicsContext *gc, bool store);
	
		
};
#endif
