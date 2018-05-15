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
	
	// Handles continuously applying transformations
	// exact steps are defined in the implementation file
	// Commands are handled according to the KeyProtocol enum class
	virtual void keyDown(GraphicsContext* gc, unsigned int keycode);
	
	// no need to account for how long a key is held down,
	// but once a key is released, if an action is associated with it,
	// it is handled in this method
	// Commands are handled according to the KeyProtocol enum class
	virtual void keyUp(GraphicsContext* gc, unsigned int keycode);
	
private:
	
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
		
	// the color of the shapes to be drawn. This state can be changed
	// with the keyboard events
	int color;
			
	// Image containing all drawn shapes
	Image *image;
	
	// The viewcontext used to seperate the model from the device's view
	ViewContext *vc;
			
	// Handles image-changing commands
	// Handles saving/loading, and color changing commands
	// if the keycode doesn't match any expected keys, it simply returns
	void handleImageCommands(GraphicsContext *gc, KeyProtocol key);
	
	// Handles actions that concern the view of the image, not the image
	// itself Handles transformations. This manipulates the viewcontext
	// Also redraws the image when called after handling the transformation
	// this will also store the previous key to make sure that transformations
	// are only configured once when necessary. This should speed this function
	// considerably. 
	void handleViewCommands(GraphicsContext *gc, KeyProtocol key);		
};
#endif
