// @Author Mohammed Alzakariya
// The ViewContext defines computes device coordinates given a model's
// As well as allowing various transformations on the view

#include "ViewContext.h"
#include <cmath>


ViewContext::ViewContext(double modelHeight)
	: composite(4,4), compositeInv(4,4), translation(4,4), translationInv(4,4),
	  rotation(4,4), rotationInv(4,4), scale(4,4), scaleInv(4,4),
	  modelHeight(modelHeight)
{
	resetComposite();
	configTranslation(0, 0, 0);
	configRotation(0);
	configZoom(1);	
}
		
matrix ViewContext::modelToDevice(double x, double y, double z) const 
{
	// make a 4x1 matrix out of the model point
	matrix point(4,1);
	point[0][0] = x;
	point[1][0] = y;
	point[2][0] = z;
	point[3][0] = 1;
	
	// transform it into a device point
	point = composite * point;
	
	return point;
}
	
matrix ViewContext::modelToDevice(const matrix& points) const
{
	return composite * points;
}
	
matrix ViewContext::deviceToModel(double x, double y, double z) const
{
	// make a 4x1 matrix out of the device point
	matrix point(4,1);
	point[0][0] = x;
	point[1][0] = y;
	point[2][0] = z;
	point[3][0] = 1;
	
	// transform it into a model point
	point = compositeInv * point;
	
	return point;
}

matrix ViewContext::deviceToModel(const matrix& points) const
{
	return compositeInv * points;
}
	
void ViewContext::translate()
{
	// perform the transformation
	composite = translation * composite;
	compositeInv = compositeInv * translationInv;
}
	
void ViewContext::rotate()
{
	// perform the transformation
	composite = rotation * composite;
	compositeInv = compositeInv * rotationInv;
}

void ViewContext::zoom() 
{
	// perform the transformation
	composite = translation * scale;
	compositeInv = compositeInv * scaleInv;
}

void ViewContext::resetComposite()
{	
	
	// remember previous transformatio matrices to revert back
	matrix prevTranslation = translation;
	matrix prevTranslationInv = prevTranslationInv;
	matrix prevRotation = prevRotation;
	matrix prevRotationInv = prevRotationInv;
	
	// 2D rotate composite by 180 deg
	configRotation(180);
	rotate();
	rotation = prevRotation;
	rotationInv = prevRotationInv;
	
	// image is inverted, and above the screen, translate down
	configTranslation(0, -modelHeight, 0);
	translate();
	translation = prevTranslation;
	translationInv = prevTranslationInv;	
}
	
void ViewContext::configTranslation(double dx, double dy, double dz)
{
	// reset translation and translationInv to identity
	translation = matrix::identity(4);
	translationInv = matrix::identity(4);
	
	// Set the translation matrix
	translation[0][3] = dx;
	translation[1][3] = dy;
	translation[2][3] = dz;
	
	// Set the inverse translation matrix
	translationInv[0][3] = -dx;
	translationInv[1][3] = -dy;
	translationInv[2][3] = -dz;
}
	
void ViewContext::configRotation(double angle, double x, double y, double z)
{
	// reset rotation and rotationInv to identity
	rotation = matrix::identity(4);
	rotationInv = matrix::identity(4);
	
	// Remember Translation state in case a translation is needed
	matrix prevTranslation = translation;
	matrix prevTranslationInv = translationInv;
	
	// Translate to origin
	if (x!=0 && y!= 0 && z!=0)
	{
		configTranslation(-x, -y, -z);
		rotation = translation * rotation;
		rotationInv = rotationInv * translationInv;
	}
	
	// set the rotation matrix
	rotation[0][0] = std::cos(angle);
	rotation[0][1] = -std::sin(angle);
	rotation[1][0] = std::sin(angle);
	rotation[1][1] = std::cos(angle);
	
	// set the rotationInv matrix
	rotationInv[0][0] = std::cos(angle);
	rotationInv[0][1] = -std::sin(angle);
	rotationInv[1][0] = std::sin(angle);
	rotationInv[1][1] = std::cos(angle);

	// Translate back to the origin
	if (x!=0 && y!=0 && z!=0)
	{
		rotation = translationInv * rotation;
		rotationInv = rotationInv * translation;
		
		// revert translation to its previous state
		translation = prevTranslation;
		translationInv = prevTranslationInv;
	}
}
	
void ViewContext::configZoom(double multiplier, double x, double y, double z)
{
	// if the multiplier is non-positive, throw an exception
	if (multiplier <= 0)
	{
		throw viewContextException("scale multiplier be a positive double");
	}
	
	// reset scale and scaleInv matrices to identity
	scale = matrix::identity(4);
	scaleInv = matrix::identity(4);
	
	// Remember Translation state in case a translation is needed
	matrix prevTranslation = translation;
	matrix prevTranslationInv = translationInv;
	
	// Translate to origin
	if (x!=0 && y!= 0 && z!=0)
	{
		configTranslation(-x, -y, -z);
		scale = translation * scale;
		scaleInv = scaleInv * translationInv;
	}
	
	// set the scale and scaleInv matrices
	for (int i=0; i<3; i++)
	{
		scale[i][i] = multiplier;
		scaleInv[i][i] = 1/multiplier;
	}
	
	// Translate back to the origin
	if (x!=0 && y!=0 && z!=0)
	{
		scale = translationInv * scale;
		scaleInv = scaleInv * translation;
		
		// revert translation to its previous state
		translation = prevTranslation;
		translationInv = prevTranslationInv;
	}
}
	