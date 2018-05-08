#ifndef MYDRAWING_H
#define MYDRAWING_H
#include "drawbase.h"
#include "Image.h"
#include <vector>

// forward reference
class GraphicsContext;
class ViewContext;

// a helper class to bundle a message with any thrown exceptions.
// To use, simply 'throw drawingException("A descriptive message about
// the problem")'.  This will throw the exception object by value.
// Recommendation is to catch by reference (to prevent slicing).
class drawingException:public std::runtime_error
{
	public:
		drawingException(std::string message):
		      std::runtime_error((std::string("Drawing Exception: ") + 
		               message).c_str()) {}
};


class MyDrawing: public DrawingBase {
public:
	// Constructor -- Initializes default state, and configures ViewContext
	// matrices to default
	MyDrawing(GraphicsContext *gc);

	// Destructor, frees image and view context
	virtual ~MyDrawing();
	
	// erases the previous drawing and draws a new one
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
	
	// This will undraw previous hypothetical shape that is drawn using
	// the registered hypothetical line. It will then draw the new hypothetical shape
	// In case of polygon and triangle, this is done one segment at a time.
	virtual void mouseMove(GraphicsContext *gc, int x, int y);
	
	// Handles continuously applying transformations
	// exact steps are defined in the implementation file
	// Commands are handled according to the KeyProtocol enum class
	virtual void keyDown(GraphicsContext* gc, unsigned int keycode);
	
	// no need to account for how long a key is held down,
	// but once a key is released, if an action is associated with it,
	// it is handled in this method
	// @throws drawingException if a key is pressed while dragging
	// Commands are handled according to the KeyProtocol enum class
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
	
	// an enum for the different drawing modes
	// Focus is the mode to specify origin of a transformation
	enum class DrawingMode {
		Line, Triangle, Rectangle,
		Circle, Polygon, Focus
	};
	
	// This defines all keys associated with all actions within this program
	enum class KeyProtocol {
		/* Transformation Commands */
		// Reset image and all transformations
		reset = '`',
		// scale down transforms the model around a point
		zoomout = '-',
		// scale up transforms the model around a point
		zoomin = '=',
		// Rotates the model 1 step in the ccw direction
		ccw = ',',
		// Rotates the model 1 step in the cw direction
		cw = '.',
		// I had to put absolute values for the keys, but those really are
		// just the normal up right down left keys! ^ > v <
		// Translate the model +step in y
		up = 65362,
		// Translate the model -step in y
		down = 65364,
		// Translate the model +step in x
		right = 65363,
		// Translate the model -step in x
		left = 65361,
		
		/* Segment Control Commands */
		// Advances to rubberband the next segment. Only Polygon/Triangle modes
		nextSeg = 'Z',
		
		/* Drawing Mode Commands */
		// Switch to Line Drawing Mode
		lineMode = 'l',
		// Switch to Triangle Drawing Mode
		triMode = 't',
		// Switch to Rectangle Drawing Mode
		rectMode = 'r',
		// Switch to Circle Drawing Mode
		circleMode = 'c',
		// Switch to Polygon Drawing Mode
		polyMode = 'p',
		// Switch to focus mode: specifies tranformation origin for zoom/rotate
		focusMode = 'f',
		
		/* Saving Commands */
		// Loads saved image (if any)
		load = 'i',
		// Saves current image
		save = 'o',
		
		/* Coloring Commands */
		// Set color to Black
		black = '1',
		// Set color to Blue
		blue = '2',
		// Set color to Green
		green = '3',
		// Set color to Red
		red = '4',
		// Set color to Cyan
		cyan = '5',
		// Set color to Magenta
		magenta = '6',
		// Set color to Yellow
		yellow = '7',
		// Set color to Gray
		gray = '8',
		// Set color to White
		white = '9',
	};
	
	// The drawing mode to determine the hypothetical shape to be drawn
	DrawingMode drawingMode;
	
	// the color of the shapes to be drawn. This state can be changed
	// with the keyboard events
	int color;
	
	// signal to be set when the user rubber bands the next segment
	bool incrementSegments;
		
	// Image containing all drawn shapes
	Image *image;
	
	// The viewcontext used to seperate the model from the device's view
	ViewContext *vc;
		
	
	// Handles all drawing mode switching 
	// if the keycode doesn't match any expected keys, it simply returns
	void handleModeCommands(GraphicsContext* gc, KeyProtocol key);
	
	// Handles all saving/loading commands
	// if the keycode doesn't match any expected keys, it simply returns
	void handleFileCommands(GraphicsContext* gc, KeyProtocol key);
	
	// Handles all color changing commands
	// if the keycode doesn't match any expected keys, it simply returns
	void handleColorCommands(KeyProtocol key);
	
	// Handles transformations. This manipulates the viewcontext
	// Also redraws the image when called after handling the transformation
	// this will also store the previous key to make sure that transformations
	// are only configured once when necessary. This should speed this function
	// considerably. 
	void handleTransformCommands(GraphicsContext* gc, KeyProtocol key);
	
	// Helper method that handles checking the drawing mode,
	// and drawing the appropriate shape while dragging.
	// Same operation would undraw those shapes, if in in XOR mode.
	// All of the shapes can be drawn using one hypothetical line segment that
	// represents a box in which they are contained
	// if store: the constructed shape will be added to the image
	// TODO: except for polygon mode
	void drawShape(GraphicsContext *gc, bool store);
	
	// Helper method that handles drawing a line within
	// a contained box defined by the hypothetical segment
	// x0,y0 -> x1,y1
	// It draws a line using the startpoint and endpoint
	// using the Line shape class
	// if store: the constructed shape will be added to the image
	void drawLine(GraphicsContext * gc, bool store);
	
	
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
