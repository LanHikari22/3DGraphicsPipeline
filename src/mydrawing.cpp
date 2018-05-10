#include "mydrawing.h"
#include "gcontext.h"
#include "matrix.h"
#include "Image.h"
#include <fstream>
#include <iostream>

// definitions for default constants
#define ROT_STEP	10
#define TRAN_STEP 20
#define SCALE 2
#define DEFAULT_COLOR GraphicsContext::CYAN

MyDrawing::MyDrawing(GraphicsContext *gc) {
	// create the view contexts
	vc = new ViewContext(gc->getWindowHeight(), gc->getWindowWidth());
	// config rotation, and scale
	vc->configRotation(ROT_STEP);
	vc->configZoom(SCALE);

	dragging = incrementSegments = false;
	color = DEFAULT_COLOR;
	numSegments = 1;
	// Default mode will be line
	drawingMode = MyDrawing::DrawingMode::Line;
	// image to redraw everytime an exposure happens
	image = new Image();
	return;
}

MyDrawing::~MyDrawing() {
	delete image;
	delete vc;
}

void MyDrawing::paint(GraphicsContext *gc) {
	gc->clear();
	// redraw the image
	gc->setColor(color);
	image->draw(gc, vc);
	return;
}

void MyDrawing::mouseButtonDown(GraphicsContext *gc, unsigned int button, int x,
		int y) {
	// mouse button pushed down
	// - clear context
	// - set origin of new hypothetical line
	// - set XOR mode for rubber-banding
	// - draw shape in XOR mode.  Note, at this point, the hypothetical line is
	//   degenerate (0 length), but need to do it for consistency

	// focus mode only reconfigures transformation matrices once captured
	// on key up
	if (drawingMode == MyDrawing::DrawingMode::Focus) {
		return;
	}

	// Originally, the hypothetical line(s) is/are degenerate
	// Every shape has AT LEAST two points (one segment)
	this->x.clear();
	this->y.clear();
	numSegments = 1;
	for (int i = 0; i < 2; i++) {
		this->x.push_back(x);
		this->y.push_back(y);
	}

	// For drawing and undrawing the shapes
	gc->setMode(GraphicsContext::MODE_XOR);

	// Using the hypothetical segment, draw the shape
	const bool store = true;
	drawShape(gc, !store);

	// if we move while holding down this button, we're dragging
	dragging = true;
	return;
}

void MyDrawing::mouseButtonUp(GraphicsContext *gc, unsigned int button, int x,
		int y) {

	// in case of focus, we reconfigure the scale and rotation matrices
	if (drawingMode == MyDrawing::DrawingMode::Focus) {
		matrix pts = vc->deviceToModel(x, y);
		std::cout << "New Focus Point: x=" << pts[0][0] << " y=" << pts[1][0]
				<< std::endl;
		vc->configZoom(SCALE, pts[0][0], pts[1][0]);
		vc->configRotation(ROT_STEP, pts[0][0], pts[1][0]);

	} else if (dragging) {
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

void MyDrawing::mouseMove(GraphicsContext *gc, int x, int y) {
	if (dragging) {
		// mouse moved - "undraw" old shape, then re-draw in new position
		// will already be in XOR mode if dragging
		// old shape undrawn
		const bool store = true;
		drawShape(gc, !store);

		// When releasing the increment segments key, we should rubberband a new
		// segment.
		if (incrementSegments) {
			const bool inPolyMode = drawingMode
					== MyDrawing::DrawingMode::Polygon;
			const bool inTriMode = drawingMode
					== MyDrawing::DrawingMode::Triangle;
			// for a triangle, you can only have 3 segments
			if (inPolyMode || (inTriMode && numSegments < 2)) {
				numSegments++;
			}
			incrementSegments = false;
		}

		// update (numSegments = numPoints-1; ie. last point)
		if (numSegments >= this->x.size()) {
			this->x.push_back(x);
			this->y.push_back(y);
		} else {
			this->x[numSegments] = x;
			this->y[numSegments] = y;
		}

		// new shape drawn
		drawShape(gc, !store);

	}
	return;
}

void MyDrawing::keyDown(GraphicsContext* gc, unsigned int keycode) {
	// since transforming changes all device coordinates, the image is repainted
	handleViewCommands(gc, (KeyProtocol) keycode);
}

void MyDrawing::keyUp(GraphicsContext *gc, unsigned int keycode) {
	// When pressing 'z', it advances the segment being rubber banded!
	const bool inPolyMode = drawingMode == MyDrawing::DrawingMode::Polygon;
	const bool inTriMode = drawingMode == MyDrawing::DrawingMode::Triangle;
	if (dragging && (inPolyMode || inTriMode) && (char) keycode == 'z') {
		incrementSegments = true;
	}
	// Otherwise, you're not allowed to 
	else if (dragging) {
		throw drawingException("Cannot initiate an action "
				"while dragging (Not in Polygon/Triangle Mode)");
	}

	// big switch statement handler that handles all drawing key actions
	// that includes changing modes, drawing different shapes, and saving/loading images
	handleImageCommands(gc, (KeyProtocol) keycode);
}

void MyDrawing::handleImageCommands(GraphicsContext *gc, KeyProtocol key) {
	switch (key) {
	/* Mode changing actions */
	case MyDrawing::KeyProtocol::lineMode:
		drawingMode = MyDrawing::DrawingMode::Line;
		break;
	case MyDrawing::KeyProtocol::triMode:
		drawingMode = MyDrawing::DrawingMode::Triangle;
		break;
	case MyDrawing::KeyProtocol::rectMode:
		drawingMode = MyDrawing::DrawingMode::Rectangle;
		break;
	case MyDrawing::KeyProtocol::circleMode:
		drawingMode = MyDrawing::DrawingMode::Circle;
		break;
	case MyDrawing::KeyProtocol::polyMode:
		drawingMode = MyDrawing::DrawingMode::Polygon;
		break;
	case MyDrawing::KeyProtocol::focusMode:
		drawingMode = MyDrawing::DrawingMode::Focus;
		break;
		/* Loading/Saving images */
	case MyDrawing::KeyProtocol::load: {
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
	case MyDrawing::KeyProtocol::save: {
		// output image into file!
		std::cout << "Saving image to Saved_Image.img" << std::endl;
		std::ofstream ofs("Saved_Image.img");
		ofs << *image;
		ofs.close();
	}
		break;
		/* Color Commands */
	case MyDrawing::KeyProtocol::black:
		color = GraphicsContext::BLACK;
		break;
	case MyDrawing::KeyProtocol::blue:
		color = GraphicsContext::BLUE;
		break;
	case MyDrawing::KeyProtocol::green:
		color = GraphicsContext::GREEN;
		break;
	case MyDrawing::KeyProtocol::red:
		color = GraphicsContext::RED;
		break;
	case MyDrawing::KeyProtocol::cyan:
		color = GraphicsContext::CYAN;
		break;
	case MyDrawing::KeyProtocol::magenta:
		color = GraphicsContext::MAGENTA;
		break;
	case MyDrawing::KeyProtocol::yellow:
		color = GraphicsContext::YELLOW;
		break;
	case MyDrawing::KeyProtocol::gray:
		color = GraphicsContext::GRAY;
		break;
	case MyDrawing::KeyProtocol::white:
		color = GraphicsContext::WHITE;
		break;
	default:
		break;
	}

}

void MyDrawing::handleViewCommands(GraphicsContext *gc, KeyProtocol key) {
	// if default case executed, this becomes false
	// so in all other cases, the image is redrawn
	bool performedTransformation = true;

	// remembers the previous key so that it doesn't always reconfigure
	// the translation matrix
	static KeyProtocol prevKey = key;

	bool newKey = prevKey != key;

	// update prevKey for next time
	prevKey = key;

	switch (key) {
	case MyDrawing::KeyProtocol::reset:
		// reset all transformations
		std::cout << "reset view and focus point to model origin" << std::endl;
		vc->reset();
		vc->configRotation(ROT_STEP, 0, 0);
		vc->configZoom(SCALE, 0, 0);
		break;
	case MyDrawing::KeyProtocol::up:
		if (newKey)
			vc->configTranslation(0, TRAN_STEP, 0);
		vc->translate();
		break;
	case MyDrawing::KeyProtocol::right:
		if (newKey)
			vc->configTranslation(TRAN_STEP, 0, 0);
		vc->translate();
		break;
	case MyDrawing::KeyProtocol::left:
		if (newKey)
			vc->configTranslation(-TRAN_STEP, 0, 0);
		vc->translate();
		break;
	case MyDrawing::KeyProtocol::down:
		if (newKey)
			vc->configTranslation(0, -TRAN_STEP, 0);
		vc->translate();
		break;
	case MyDrawing::KeyProtocol::zoomout:
		vc->zoom(false);
		break;
	case MyDrawing::KeyProtocol::zoomin:
		vc->zoom(true);
		break;
	case MyDrawing::KeyProtocol::ccw:
		vc->rotate(true);
		break;
	case MyDrawing::KeyProtocol::cw:
		vc->rotate(false);
		break;
	default:
		performedTransformation = false;
		break;
	}

	// redraw the image since the coords changed
	if (performedTransformation) {
		paint(gc);
	}

}

void MyDrawing::drawShape(GraphicsContext *gc, bool store) {
	switch (drawingMode) {
	case MyDrawing::DrawingMode::Line: {
		drawLine(gc, store);
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
	case MyDrawing::DrawingMode::Triangle:
	case MyDrawing::DrawingMode::Polygon: {
		drawPolygon(gc, store);
	}
		break;
	case MyDrawing::DrawingMode::Focus: /// should never reach here
	default:
		throw drawingException("Invalid drawing mode specified");
	}
}

void MyDrawing::drawLine(GraphicsContext *gc, bool store) {

	// startpoint and endpoint
	matrix m(4, 3);
	m[0][0] = x[0];
	m[0][1] = x[1];
	m[1][0] = y[0];
	m[1][1] = y[1];
	m[2][0] = 0;
	m[2][1] = 0;
	m[3][0] = 1;
	m[3][1] = 1;

	// convert to model coordinates
	m = vc->deviceToModel(m);

	// Draw the triangle
	Line l(m, this->color);
	l.draw(gc, vc);

	if (store) {
		image->add(&l);
	}
}

void MyDrawing::drawRectangle(GraphicsContext *gc, bool store) {
	// Define the rectangle to be the hypothetical box
	double w = x[1] - x[0];
	double h = y[1] - y[0];

	// Its origin will be halfway from p0 and p1
	matrix m(4, 1);
	m[0][0] = x[0] + w / 2;
	m[1][0] = y[0] + h / 2;
	m[2][0] = 0;
	m[3][0] = 1;

	// recompute w,h
	matrix p0 = vc->deviceToModel(x[0], y[0], 0);
	matrix p1 = vc->deviceToModel(x[1], y[1], 0);
	w = p1[0][0] - p0[0][0];
	h = p1[1][0] - p0[1][0];

	// convert to model coordinates
	m = vc->deviceToModel(m);

	// Draw the rectangle
	Rectangle r(m, w, h, color);
	r.draw(gc, vc);

	if (store) {
		image->add(&r);
	}
}

void MyDrawing::drawCircle(GraphicsContext *gc, bool store) {
	// width/height of the hypothetical box formed by the segment
	int w = x[1] - x[0];
	int h = y[1] - y[0];

	// Its origin will be halfway from p0 and p1
	// column two is a point on the circle; (r,0,0)
	matrix m(4, 2);
	m[0][0] = x[0] + w / 2;
	m[0][1] = x[1];
	m[1][0] = y[0] + h / 2;
	m[1][1] = y[1];
	m[2][0] = 0;
	m[2][1] = 0;
	m[3][0] = 1;
	m[3][1] = 1;

	// Convert to model coordinates
	m = vc->deviceToModel(m);

	// Draw the circle
	Circle c(m, color);
	c.draw(gc, vc);

	if (store) {
		image->add(&c);
	}
}

void MyDrawing::drawPolygon(GraphicsContext *gc, bool store) {

	// connect all the vertices in vect x, y together!
	matrix m(4, numSegments + 1);
	for (unsigned int c = 0; c < numSegments + 1; c++) {
		m[0][c] = x[c];
		m[1][c] = y[c];
		m[2][c] = 0;
		m[3][c] = 1.0;
	}

	// Convert to model coordinates
	m = vc->deviceToModel(m);

	// Try to draw a polygon, if possible...
	try {
		// Draw the Polygon
		Polygon p(numSegments + 1, m, color);
		p.draw(gc, vc);
		if (store) {
			image->add(&p);
		}
	} catch (shapeException) {
		// Nope! Degenerate polygon! just draw a line
		drawLine(gc, store);
	}
}
