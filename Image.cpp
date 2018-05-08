// @Author Mohammed Alzakariya
// Implementation file for the Image class

#include "Image.h"

Image::Image()
	: spaceLevel(0)
{ }

Image::Image(const Image &i)
	: spaceLevel(i.spaceLevel)
{	
	// deep-Copy each Shape pointer found in the image's shapes
	std::vector<Shape*>::const_iterator it;
	for (it = i.shapes.begin(); it != i.shapes.end(); it++)
	{
		this->add(*it);
	}
}

Image::~Image()
{
	erase();
}

Image& Image::operator=(const Image &rhs)
{	
	// First, destroy all shapes in the container
	this->erase();
	
	// Now, deep-copy all shapes in rhs
	std::vector<Shape*>::const_iterator it;
	for (it = rhs.shapes.begin(); it != rhs.shapes.end(); it++)
	{
		this->add(*it);
	}
	
	// also copy the spaceLevel of the image
	this->spaceLevel = rhs.spaceLevel;
	
	return *this;
	
}

void Image::add(const Shape *s)
{
	// Clone shape and add the clone to the container
	shapes.push_back(s->clone());
}

void Image::draw(GraphicsContext *gc, ViewContext *vc)
{
	// draw all shapes!
	std::vector<Shape*>::const_iterator it;
	for (it = shapes.begin(); it != shapes.end(); it++)
	{
		(*it)->draw(gc, vc);
	}
}

void Image::setSpaceLevel(unsigned int spaceLevel)
{
	this->spaceLevel = spaceLevel;
}

std::ostream& Image::out(std::ostream &os) const
{
	std::vector<Shape*>::const_iterator it;
	
	// print spacelevel*' ' after every shape so that
	// the first line of next shape can be printed on the same level
	const std::string NEW_LINE_PAD(this->spaceLevel, ' ');
	
	for (unsigned int i=0; i<shapes.size(); i++)
	{
		// set the space level for each shape in case the output is appended to 
		// extra text
		shapes[i]->setSpaceLevel(this->spaceLevel);

		os << *shapes[i];
		
		// if not the last shape, set up the line pad for the next
		// so it prints on the same padding as the previous line...
		if (i != shapes.size() - 1)
		{
			os << std::endl << NEW_LINE_PAD;
		}
	}
	
	return os;
}

std::istream& Image::in(std::istream &is)
{
	// clear image, since new data is being input
	this->erase();
	
	// parse in a character to determine type
	char shapeSpecifier = '\0';
	
	// Parse Shape streams until non-shape data is detected or End of Stream
	bool detectedNonShape = false;
	while (!detectedNonShape && is >> shapeSpecifier)
	{
		switch (shapeSpecifier)
		{
			case 'p': 
			{
				Point p(1,2,3,0xFFFFFF);
				is >> p;
				this->add(&p);
			}
			break;
			case 'l':
			{
				matrix m_l(4,2);
				Line l(m_l, 0);
				is >> l;
				this->add(&l);
			}
			break;
			case 't':
			{
				matrix m_t(4,3);
				Triangle t(m_t, 0);
				is >> t;
				this->add(&t);
			}
			break;
			case 'c':
			{
				matrix m_c(4,2);
				Circle c(m_c, 0);
				is >> c;
				this->add(&c);
			}
			break;
			case 'r':
			{
				matrix m_r(4,4);
				Rectangle r(m_r, 0);
				is >> r;
				this->add(&r);
			}
			break;
			case 'g':
			{
				matrix m_g(4,1);
				Polygon g(m_g, 0);
				is >> g;
				this->add(&g);
			}
			break;
			default:
				detectedNonShape = true;
		}
	}
	
	return is;
}

void Image::erase()
{
	// TODO: test
	
	// Loop through all shapes! delete shapes and remove from container!
	std::vector<Shape*>::iterator it;
	for (it = shapes.begin(); it != shapes.end(); it++)
	{
		delete *it;
	}
	shapes.clear();
}

std::ostream& operator<<(std::ostream &os, const Image &o)
{
	o.out(os);
	return os;
}

std::istream& operator>>(std::istream &is, Image &o)
{
	o.in(is);
	return is;
}