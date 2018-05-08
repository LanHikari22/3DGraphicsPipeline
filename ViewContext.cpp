// @Author Mohammed Alzakariya
// The ViewContext defines computes device coordinates given a model's
// As well as allowing various transformations on the view

#include "ViewContext.h"
#include <cmath>

ViewContext::ViewContext(double modelHeight, double modelWidth)
	: composite(4,4), compositeInv(4,4), translation(4,4), translationInv(4,4),
	  rotation(4,4), rotationInv(4,4), scale(4,4), scaleInv(4,4),
	  netTranslation(0), netRotation(4,4), netZoom(4,4),
	  modelHeight(modelHeight), modelWidth(modelWidth)
{
	configTranslation(0, 0, 0);
	configRotation(0);
	configZoom(1);
	resetComposite();
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
	// translate in the model coordinates
	composite = composite * translation;
	compositeInv = translationInv * compositeInv;
}
	
void ViewContext::rotate(bool cw)
{
	// rotate in the model coordinates
	if (cw)
	{
		composite = composite * rotation;
		compositeInv = rotationInv * compositeInv;
	} else
	{
		composite = composite * rotationInv;
		compositeInv = rotation * compositeInv;
	}
}

void ViewContext::zoom(bool in) 
{
	// scale in the model coordinates
	if (in) 
	{
		composite = composite * scale;
		compositeInv = scaleInv * compositeInv;
	} else {
		composite = composite * scaleInv;
		compositeInv = scale * compositeInv;
	}
}

void ViewContext::updateComposite()
{
	// apply translation, then rotation, then zoom
	
}


void ViewContext::resetComposite()
{	
	// initialize composite and compositeInv to identity
	composite = matrix::identity(4);
	compositeInv = matrix::identity(4);
	
	// remember previous transformatio matrices to revert back
	matrix prevTranslation = translation;
	matrix prevTranslationInv = translationInv;
	
	// reflect y
	matrix reflectY = matrix::identity(4);
	reflectY[1][1] = -1;
	composite = composite * reflectY;
	compositeInv = reflectY * compositeInv;
	
	// image is inverted, and above the screen, translate down and right

	configTranslation(modelWidth/2, -modelHeight/2, 0);
	translate();
	translation = prevTranslation;
	translationInv = prevTranslationInv;	
}
	
matrix ViewContext::computeTranslation(double dx, double dy, double dz)
{
	// reset translation and translationInv to identity
	matrix tranMatrix = matrix::identity(4);
	
	// Set the translation matrix
	tranMatrix[0][3] = dx;
	tranMatrix[1][3] = dy;
	tranMatrix[2][3] = dz;	
	
	return tranMatrix;
}

matrix ViewContext::computeRotation(double angle, double x, double y, double z)
{
	// convert degrees to radians
	angle = angle/180 * M_PI;
		
	// Translate to origin
	matrix rotation = computeTranslation(-x, -y, -z);
	
	// composite the rotation matrix
	matrix tempRot = matrix::identity(4);
	
	tempRot[0][0] = std::cos(angle);
	tempRot[0][1] = -std::sin(angle);
	tempRot[1][0] = std::sin(angle);
	tempRot[1][1] = std::cos(angle);
	
	rotation = tempRot * rotation;
	
	// Translate back to the center point
	rotation = computeTranslation(x, y, z) * rotation;

	return rotation;
}

matrix ViewContext::computeZoom(double multiplier, double x, double y, double z)
{
	// if the multiplier is non-positive, throw an exception
	if (multiplier <= 0)
	{
		throw viewContextException("scale multiplier be a positive double");
	}
			
	// Translate to origin
	matrix scale = computeTranslation(-x,-y,-z);
	
	// composite into the scale matrix the actual scaling
	matrix scaleMatrix = matrix::identity(4);
	for (int i=0; i<3; i++)
	{
		scaleMatrix[i][i] *= multiplier;
	}
	scale = scaleMatrix * scale;
	
	// Translate back to the point's center	
	scale = computeTranslation(x,y,z) * scale;
	
	return scale;
}


void ViewContext::configTranslation(double dx, double dy, double dz)
{
	translation = computeTranslation(dx, dy, dz);
	translationInv = computeTranslation(-dx, -dy, -dz);	
}
	
void ViewContext::configRotation(double angle, double x, double y, double z)
{
	rotation = computeRotation(angle, x, y, z);
	rotationInv = computeRotation(-angle, x, y, z);
}
	
void ViewContext::configZoom(double multiplier, double x, double y, double z)
{
	scale = computeZoom(multiplier, x, y, z);
	scaleInv = computeZoom(1/multiplier, x, y, z);
}