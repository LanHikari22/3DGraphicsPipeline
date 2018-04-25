#include "mydrawing.h"
#include "gcontext.h"
#include "matrix.h"
#include "Image.h"
#include <fstream>
#include <iostream> // TODO: temporary?

// Constructor
MyDrawing::MyDrawing() {
	dragging = incrementSegments = false;
	color = GraphicsContext::CYAN;
	numSegments = 1;
	// Default mode will be line
	drawingMode = MyDrawing::DrawingMode::Line;
	// image to redraw everytime an exposure happens
	image = new Image();
	// vectors x and y will need at least 2 points
	x.push_back(0); x.push_back(0);
	y.push_back(0); y.push_back(0);
	return;
}

MyDrawing::~MyDrawing() {
	delete image;	
}

void MyDrawing::paint(GraphicsContext *gc) {
	// TODO: what am i supposed to do here??
	// for fun, let’s draw a "fixed" shape in the middle of the screen
	// it will only show up after an exposure
//	int middlex = gc->getWindowWidth() / 2;
//	int middley = gc->getWindowHeight() / 2;
//	gc->setColor(GraphicsContext::MAGENTA);
//	for (int yi = middley - 50; yi <= middley + 50; yi++) {
//		gc->drawLine(middlex - 50, yi, middlex + 50, yi);
//	}
	
	// redraw the image
	gc->setColor(color);
	image->draw(gc);
	return;
}

void MyDrawing::mouseButtonDown(GraphicsContext * gc, unsigned int button,
		int x, int y) {
	// mouse button pushed down
	// - clear context
	// - set origin of new hypothetical line
	// - set XOR mode for rubber-banding
	// - draw shape in XOR mode.  Note, at this point, the hypothetical line is
	//   degenerate (0 length), but need to do it for consistency
	
	// Originally, the hypothetical line(s) is/are degenerate
	this->x.clear();
	this->y.clear();
	numSegments = 1;	
	for (int i=0; i<2; i++) {
		this->x.push_back(x);
		this->y.push_back(y);
	}
	
	// For drawing and undrawing the shapes
	gc->setMode(GraphicsContext::MODE_XOR);
	
	// Using the hypothetical segment, draw the shape
	const bool store = true;
	drawShape(gc, !store);

	std::cout << "called mouse down" << std::endl;
	dragging = true;
	return;
}

void MyDrawing::mouseButtonUp(GraphicsContext * gc, unsigned int button, int x,
		int y) {
	if (dragging) {
		// undraw old shape
		const bool store = true;
		drawShape(gc, !store);
				
		// just in x and y here do not match x and y of last mouse move
		this->x[numSegments] = x;
		this->y[numSegments] = y;
		
		// go back to COPY mode
		gc->setMode(GraphicsContext::MODE_NORMAL);
		// new shape drawn in copy mode: actually store shape into image
		drawShape(gc, store);
		// clear flag
		dragging = false;
	}
	return;
}

void MyDrawing::mouseMove(GraphicsContext * gc, int x, int y) {
	if (dragging) {
		// mouse moved - "undraw" old shape, then re-draw in new position
		// will already be in XOR mode if dragging
		// old shape undrawn
		const bool store = true;
		drawShape(gc, !store);
		
		// When releasing the increment segments key, this activates // TODO: debug?
		if (incrementSegments) {
			numSegments++;
			std::cout << "numSegments: " << numSegments << std::endl;
			this->x.push_back(this->x[numSegments-1]);
			this->y.push_back(this->x[numSegments-1]);
			incrementSegments = false;
		}

		
		// update
		this->x[numSegments] = x;
		this->y[numSegments] = y;

		// new shape drawn
		drawShape(gc, !store);
		
	}
	return;
}


void MyDrawing::keyUp(GraphicsContext* gc, unsigned int keycode)
{
	if (dragging && drawingMode == MyDrawing::DrawingMode::Polygon 
			&& (char)keycode == 'z') {
		incrementSegments = true;
	}
	else if (dragging) {
		throw drawingException("Cannot initiate an action while dragging (unless in polygon mode)");
	}

	std::cout << "keycode: '" << (char)keycode << "'" << std::endl;
	
	handleModeCommands(gc, keycode);
	handleFileCommands(gc, keycode);
	handleColorCommands(gc, keycode);
}


// Handles all drawing mode switching commands according to the 
// specifications in KeyUp()
// if the keycode doesn't match any expected keys, it simply returns
void MyDrawing::handleModeCommands(GraphicsContext* gc, unsigned int keycode) {
	switch ((char)keycode) {
	case 'l':
		drawingMode = MyDrawing::DrawingMode::Line;
		break;
	case 't':
		drawingMode = MyDrawing::DrawingMode::Triangle;
		break;
	case 'r':
		drawingMode = MyDrawing::DrawingMode::Rectangle;
		break;
	case 'c':
		drawingMode = MyDrawing::DrawingMode::Circle;
		break;
	case 'p':
		drawingMode = MyDrawing::DrawingMode::Polygon;
		break;
	default:
		break;
	}
}

void MyDrawing::handleFileCommands(GraphicsContext* gc, unsigned int keycode) {
	switch ((char)keycode) {
	case 'i': {
		// input image from file!
		std::cout << "Loading image from Saved_Image.img" << std::endl;
		std::ifstream ifs("Saved_Image.img");
		// clear, input, redraw
		gc->clear();
		ifs >> *image;
		// clear and redraw
		paint(gc);
		ifs.close();
	}
	break;
	case 'o': {
		// output image into file!
		std::cout << "Saving image to Saved_Image.img" << std::endl;
		std::ofstream ofs("Saved_Image.img");
		ofs << *image;
		ofs.close();
	}
	break;
	default:
		break;
	}
}

void MyDrawing::handleColorCommands(GraphicsContext* gc, unsigned int keycode)
{
	int colors[] = {GraphicsContext::BLACK, GraphicsContext::BLUE, GraphicsContext::GREEN,
	GraphicsContext::RED, GraphicsContext::CYAN, GraphicsContext::MAGENTA,
	GraphicsContext::YELLOW, GraphicsContext::GRAY, GraphicsContext::WHITE};
	int colorIndex = keycode - '0';
	if (colorIndex >= 0 && colorIndex < 9) {
		color = colors[colorIndex];
	}
}

void MyDrawing::drawShape(GraphicsContext * gc, bool store) {
	switch (drawingMode) {
		case MyDrawing::DrawingMode::Line: {
			drawLine(gc, store);
		}
		break;
		case MyDrawing::DrawingMode::Triangle: {
			drawTriangle(gc, store);
		}
		break;
		case MyDrawing::DrawingMode::Rectangle: {
			drawRectangle(gc, store);
		}
		break;
		case MyDrawing::DrawingMode::Circle: {
			drawCircle(gc, store);
		}
		break;
		case MyDrawing::DrawingMode::Polygon: {
			drawPolygon(gc, store);
		}
		break;
		default:
			throw drawingException("Invalid drawing mode specified");
	}
}

void MyDrawing::drawLine(GraphicsContext * gc, bool store) {
	
	// startpoint and endpoint
	matrix m(3,3);
	m[0][0] = x[0];	m[0][1] = x[1];
	m[1][0] = y[0];	m[1][1] = y[1];
	m[2][0] = 0;	m[2][1] = 0;
	
	// Draw the triangle
	Line l(m, this->color);
	l.draw(gc);
	
	if (store) {
		image->add(&l);
	}
}


void MyDrawing::drawTriangle(GraphicsContext * gc, bool store) {
	// Define the triangle to be contained in the hypothetical box
	int w = x[1] - x[0];
	
	// p1 would be on the lower left corner
	// p2 in the upper middle
	// p3 in the lower right corner
	matrix m(3,3);
	m[0][0] = x[0];	m[0][1] = x[0]+w/2;	m[0][2] = x[1];
	m[1][0] = y[1];	m[1][1] = y[0];		m[1][2] = y[1];
	m[2][0] = 0;	m[2][1] = 0;		m[2][2] = 0;
	
	// Draw the triangle
	Triangle t(m, this->color);
	t.draw(gc);
	
	if (store) {
		image->add(&t);
	}
}

void MyDrawing::drawRectangle(GraphicsContext *gc, bool store) {
	// Define the rectangle to be the hypothetical box
	int w = x[1] - x[0];
	int h = y[1] - y[0];
	
	// Its origin will be halfway from p0 and p1
	matrix m(3,1);
	m[0][0] = x[0] + w/2;
	m[1][0] = y[0] + h/2;
	m[2][0] = 0;
	
	// Draw the rectangle
	Rectangle r(m, w, h, color);
	r.draw(gc);
	
	if (store) {
		image->add(&r);
	}
}

void MyDrawing::drawCircle(GraphicsContext * gc, bool store) {
	// width/height of the hypothetical box formed by the segment
	int w = x[1] - x[0];
	int h = y[1] - y[0];
	
	// Its origin will be halfway from p0 and p1
	// column two is a point on the circle; (r,0,0)
	matrix m(3,2);
	m[0][0] = x[0] + w/2;	m[0][1] = x[1];
	m[1][0] = y[0] + h/2; 	m[1][1] = y[1];
	m[2][0] = 0;			m[2][1] = 0;
	
	// Draw the circle
	Circle c(m, color);
	c.draw(gc);
	
	if (store) {
		image->add(&c);
	}
}

void MyDrawing::drawPolygon(GraphicsContext * gc, bool store) {
	
	// connect all the vertices in vect x, y together!
	matrix m(3,numSegments+1);
	for (unsigned int c=0; c<numSegments+1; c++) {
		m[0][c] = x[c];
		m[1][c] = y[c];
		m[2][c] = 0;
	}
	
	std::cout << m << std::endl;
	
	// Try to draw a polygon, if possible...
	try {
		// Draw the Polygon
		Polygon p(numSegments+1, m, color);
		p.draw(gc);
		if (store) {
			image->add(&p);
		}
	} catch (shapeException) {
		// Nope! Degenerate polygon! just draw a line
		drawLine(gc, store);
	}
}