// @Author Mohammed Alzakariya
// The ViewContext defines computes device coordinates given a model's
// As well as allowing various transformations on the view

#ifndef VIEW_CONTEXT_H
#define VIEW_CONTEXT_H

#include "matrix.h"

// a helper class to bundle a message with any thrown exceptions.
// To use, simply 'throw viewContextException("A descriptive message about
// the problem")'.  This will throw the exception object by value.
// Recommendation is to catch by reference (to prevent slicing).
class viewContextException:public std::runtime_error
{
	public:
		viewContextException(std::string message):
		      std::runtime_error((std::string("ViewContext Exception: ") + 
		               message).c_str()) {}
};

class ViewContext {
public:
	// default constructor -- sets the composite matrix and its
	// inverse to its reset state.
	// It configures the translation, rotation, and zoom matrices to the
	// identity matrix.
	// The reset state of the composite matrix allows for converion
	// between the cartesian system of the model, and the device.
	// @param modelHeight this is needed for the cartesian conversion
	// @param modelWidth width dimension of the model to get total dimensions
	ViewContext(double modelHeight, double modelWidth);
		
	// Complete reset! Return to init state
	// Resets all configured transformeations, and registered accumulation 
	// of zooming/scaling and translating
	// composite matrix simply translates from model coordinates to device
	// without any additional transformations
	void reset();
	
	// Computes a  point as it would appear on the device, given its value
	// in the model.
	// @param x	the x component of the model point
	// @param y the y component of the model point
	// @param z the z component of the model point (Default 0, 2D)
	// @return a 4x1 matrix repsenting the device coordinates
	matrix modelToDevice(double x, double y, double z=0) const;
	
	// Computes the device coordinates of all column vectors in the
	// matrix parameter.
	// @param numPoints the number of columns in the matrix
	// @points a matrix with 4 rows (x y z 1) and numPoints columns
	//		   represents the number of model vertices to convert
	// @returns a 4xnumPoints matrix representing the device points
	// @throws matrixException if numPoints is incorrect or points rows < 4
	matrix modelToDevice(const matrix& points) const;
	
	// Computes a  point as it would appear in the model, given its value
	// in the device.
	// @param x	the x component of the device point
	// @param y the y component of the device point
	// @param z the z component of the device point (Default 0, 2D)
	// @return a 4x1 matrix repsenting the model coordinates
	matrix deviceToModel(double x, double y, double z=0) const;
	
	// Computes the model coordinates of all column vectors in the
	// matrix parameter.
	// @points a matrix with 4 rows (x y z 1) and numPoints columns
	//		   represents the number of device vertices to convert
	// @returns a 4xnumPoints matrix representing the model points
	// @throws matrixException if numPoints is incorrect or points rows < 4
	matrix deviceToModel(const matrix& points) const;
	
	
	// Translates the view by the configured transformation matrix
	// This Transforms the composite matrix and recomputes its inverse
	void translate();
	
	// Rotates the view using the configured rotation matrix
	// This Transforms the composite matrix and recomputes its inverse
	void rotate(bool cw=true);
	
	// Scales the matrix up or down using the configured zoom matrix
	// This Transforms the composite matrix and recomputes its inverse
	void zoom(bool in=true);

	// This will update the composite by applying the amount of
	// net translation, then net rotation, then net zoom
	void updateComposite();
	
	// This resets the composite matrix to its original state
	// As well as the inverse matrix. Which is a transformation from
	// a cartesian system with the origin in the middle, y up to
	// where the origin is on the top left corner, y down
	// The image is rotated 180 degrees, and shifted down modelHeight units.
	void resetComposite();
	
	// recomputes and sets the translation matrix and its inverse
	// given the parameters dx and dy, which represent 
	// the translation in the x and y directions
	// the matrix is then set in memory for effecient use when performing
	// a translation
	// @param dx The amount of transformation in the x direction
	// @param dy The amount of transformation in the y direction
	void configTranslation(double dx, double dy, double dz=0);
	
	// 2D only -- recomputes and sets the rotation matrix and its inverse
	// for effecient usage when applied
	// @param angle (deg) from x=0 towards y+
	// @params (x,y,z) MODEL point of rotation. Default: origin
	void configRotation(double angle, double x=0, double y=0, double z=0);
	
	// recomputes and sets zoom matrix and its inverse
	// so it can be quickly applied when the zoom method is called.
	// @param multiplier the amount to scale. Must be positive.
	// @params (x,y,z) the MODEL center to zoom with respect to
	// @throws viewContextException if multiplier is less or equal to zero
	void configZoom(double multiplier, double x=0, double y=0, double z=0);
	
private:	
	
	// Internal configuration of model to view
	// computes matrices for conversion  from x,y,z to L,M,N
	// viewing coordinates. This changes as the viewing point 
	// changes via orbit
	// this updates the ModelToView and ViewToModel matrices
	// @params (p0x, p0y, p0z) coords of view point
	void config_vTm(double p0x, double p0y, double p0z);
	
	// Internal configuration of view to view plane
	// determines the projection matrix based on the focal point location
	// in the z coordinate of the view coordinate, zf
	// copmutes and updates the view to view plane matrix
	// matrices
	// @param zf focal point distance from p0. Must be positive: behind view plane
	// @throws viewContextException if zf <= 0
	void config_pTv(const double zf);
		
	// Computes the mapping from the view plane coordinates to the
	// device coordinates. This involves the same rules for the 2D composite
	// transformations, in addition to normalizing the 4th component,
	// and scaling and transforming so that the view plane is fully visible
	void config_dTp();
	
	// internal function that computes a translation matrix
	// @params (dx,dy,dz) amounts of translation in all components
	matrix computeTranslation(double dx, double dy, double dz=0);
	
	// internal function that computes a rotation matrix
	// @params (x,y,z) represent the center of rotation
	// @param angle angle of rotation in radians
	matrix computeRotation(double angle, double x=0, double y=0, double z=0);
	
	// internal function that computes a scale matrix
	// @param multiplier the amount to scale. must be non-zero positive
	// @params (x,y,z) center of zooming
	// @throws viewContextException if multiplier is non-positive
	matrix computeZoom(double multiplier, double x=0, double y=0, double z=0);
	
	// composite matrix used to transform a point
	// from model coordinates to device coordinates
	// its inverse is also needed for the inverse (in case of 2D)
	matrix composite, compositeInv;
	
	// matrices for forming the complete 3D composite matrix!
	// in order to go from model to device, we have to traverse
	// the view (camera) coordinates, the view plane, and then the device!
	matrix vTm, pTv, dTp;
	
	// the translation matrix and its inverse. 
	// When the image is translated, they are applied
	// to the netTranslation matrix and its inverse
	matrix translation, translationInv;
		
	// The rotation matrix and its inverse. 
	// When the image on the screen is rotated,
	// the netRotation matrix and its inverse are transformed by it
	matrix rotation, rotationInv;
		
	// Scale matrix and its inverse. For zooming into or out of an image 
	matrix scale, scaleInv;
	
	// the total amount of translation to be applied to the composite matrix
	matrix netTranslation, netTranslationInv;
	
	// represents the total amount of rotation applied to the composite matrix
	// stored as a matrix instead of a double because of the possiblity
	// of rotating around a focus point
	matrix netRotation, netRotationInv;	
	
	// represents the net amount of zooming applied to the composite matrix
	// stored as a matrix instead of a double because of the possibility
	// of zooming into a focus point
	matrix netScale, netScaleInv;
		
	// dimensions of the device, repersenting the maximum x,y coordinates in it
	const double deviceWidth, deviceHeight;
};

#endif