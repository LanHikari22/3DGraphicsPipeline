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

	color = DEFAULT_COLOR;
	// image to redraw everytime an exposure happens
	image = new Image();
	// Draw axes TODO: mention in doc? make function??
	matrix pts(3,2);
	const double axisLen = 200;
	pts[0][1]= axisLen;
	pts[1][1]= 0;
	pts[2][1]= 0;
	Line *xAxis = new Line(pts, GraphicsContext::RED);
	pts[0][1] = 0;
	pts[1][1] = axisLen;
	pts[2][1] = 0;
	Line *yAxis = new Line(pts, GraphicsContext::GREEN);
	pts[0][1] = 0;
	pts[1][1] = 0;
	pts[2][1] = axisLen;
	Line *zAxis = new Line(pts, GraphicsContext::BLUE);
	
	image->add(xAxis);
	image->add(yAxis);
	image->add(zAxis);
	
	// TODO: testing...
	image->parseStl("word.stl");
	paint(gc);
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

void MyDrawing::keyDown(GraphicsContext* gc, unsigned int keycode) {
	// since transforming changes all device coordinates, the image is repainted
	handleViewCommands(gc, (KeyProtocol) keycode);
}

void MyDrawing::keyUp(GraphicsContext *gc, unsigned int keycode) {
	// big switch statement handler that handles all drawing key actions
	// that includes changing colors of loaded pictures and saving/loading images
	handleImageCommands(gc, (KeyProtocol) keycode);
}

void MyDrawing::handleImageCommands(GraphicsContext *gc, KeyProtocol key) {
	switch (key) {
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
