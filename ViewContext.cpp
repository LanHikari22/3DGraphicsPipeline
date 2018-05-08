// @Author Mohammed Alzakariya
// The ViewContext defines computes device coordinates given a model's
// As well as allowing various transformations on the view

#include "ViewContext.h"
#include <cmath>

ViewContext::ViewContext(double modelHeight, double modelWidth)
	: composite(4,4), compositeInv(4,4), translation(4,4), translationInv(4,4),
	  rotation(4,4), rotationInv(4,4), scale(4,4), scaleInv(4,4),
	  netTranslation(0), netRotation(0), netZoom(0),
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
	
matrix ViewContext::computeTranslation(double dx, double dy, double dz=0)
{
	// reset translation and translationInv to identity
	tranMatrix = matrix::identity(4);
	
	// Set the translation matrix
	tranMatrix[0][3] = dx;
	tranMatrix[1][3] = dy;
	tranMatrix[2][3] = dz;	
	
	return tranMatrix;
}

matrix ViewContext::computeRotation(double angle, double x=0, double y=0, double z=0)
{
	// convert degrees to radians
	angle = angle/180 * M_PI;
	
	// reset rotation and rotationInv to identity
	matrix rotation = matrix::identity(4);
	
	
	// Translate to origin
	matrix translation = computeTranslation(-x, -y, -z);	
	rotation = translation * rotation;
	
	// composite the rotation matrix
	matrix rot = matrix::identity(4);
	matrix rotInv = matrix::identity(4);
	
	rot[0][0] = std::cos(angle);
	rot[0][1] = -std::sin(angle);
	rot[1][0] = std::sin(angle);
	rot[1][1] = std::cos(angle);
	
	rotation = rot * rotation;
	
	// Translate back to the point
	translation = computeTranslation(x, y, z);
	rotation = translation * rotation;

	return rotation;
}

matrix ViewContext::computeZoom(double multiplier, double x=0, double y=0, double z=0);


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
	// convert degrees to radians
	angle = angle/180 * M_PI;
	
	// reset rotation and rotationInv to identity
	rotation = matrix::identity(4);
	rotationInv = matrix::identity(4);
	
	// Remember Translation state in case a translation is needed
	matrix prevTranslation = translation;
	matrix prevTranslationInv = translationInv;
	
	// Translate to origin
	configTranslation(-x, -y, -z);
	rotation = translation * rotation;
	rotationInv = rotationInv * translationInv;
	
	// composite the rotation matrix
	matrix rot = matrix::identity(4);
	matrix rotInv = matrix::identity(4);
	
	rot[0][0] = std::cos(angle);
	rot[0][1] = -std::sin(angle);
	rot[1][0] = std::sin(angle);
	rot[1][1] = std::cos(angle);
	
	rotation = rot * rotation;
	
	// composite the rotationInv matrix
	rotInv[0][0] = std::cos(-angle);
	rotInv[0][1] = -std::sin(-angle);
	rotInv[1][0] = std::sin(-angle);
	rotInv[1][1] = std::cos(-angle);

	rotationInv = rotationInv * rotInv;
	
	// Translate back to the point		
	rotation = translationInv * rotation;
	rotationInv = rotationInv * translation;
		
	// revert translation to its previous state
	translation = prevTranslation;
	translationInv = prevTranslationInv;

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
	configTranslation(-x, -y, -z);
	scale = translation;
	scaleInv = translationInv;
	
	// composite into the scale matrix the actual scaling
	matrix scaleMatrix = matrix::identity(4);
	matrix scaleMatrixInv = matrix::identity(4);
	for (int i=0; i<3; i++)
	{
		scaleMatrix[i][i] *= multiplier;
		scaleMatrixInv[i][i] *= 1/multiplier;
	}
	scale = scaleMatrix * scale;
	scaleInv = scaleInv * scaleMatrixInv;
	
	// Translate back to the point's center	
	scale = translationInv * scale;
	scaleInv = scaleInv * translation;
	
	// revert translation to its previous state
	translation = prevTranslation;
	translationInv = prevTranslationInv;
}
