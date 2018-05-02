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
	ViewContext(double modelHeight);
		
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
	void rotate();
	
	// Scales the matrix up or down using the configured zoom matrix
	// This Transforms the composite matrix and recomputes its inverse
	void zoom();

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
	// @param angle from x=0 towards y+
	// @params (x,y,z) point of rotation. Default: origin
	void configRotation(double angle, double x=0, double y=0, double z=0);
	
	// recomputes and sets zoom matrix and its inverse
	// so it can be quickly applied when the zoom method is called.
	// @param multiplier the amount to scale. Must be positive.
	// @params (x,y,z) the origin to zoom with respect to
	// @throws viewContextException if multiplier is less or equal to zero
	void configZoom(double multiplier, double x=0, double y=0, double z=0);
	
private:	
	// composite matrix used to transform a point
	// from model coordinates to device coordinates
	// initially set to transform from center origin, y up
	// to center upperleft corner, y down.
	matrix composite;
	
	// inverse of the composite matrix. Used to transform a point
	// from device coordinates to model coordinates
	// initially set to transform from center upperleft corner, y down
	// to center origin, y up
	// This must be recomputed whenever the composite matrix is recomputed
	matrix compositeInv;
	
	// A translation matrix that pans the image in the x or y direction
	// the matrix is left multiplied with the composite matrix when
	// the image is panned
	matrix translation;
	
	// inverse of the translation matrix. Used for computing the overall
	// inverse composite matrix
	matrix translationInv;
	
	// The rotation matrix. When the image on the screen is rotated,
	// the composite matrix is transformed by it
	matrix rotation;
	
	// inverse of the rotation matrix. Used for computing the overall
	// inverse composite matrix
	matrix rotationInv;
	
	// Scale matrix. Zooms into or out of an image by scaling the view up.
	matrix scale;
	
	// inverse of the scale matrix. Used for computing the overall
	// inverse composite matrix
	matrix scaleInv;
	
	// The height of the model, repersenting the maximum y coordinate in it
	const double modelHeight;
		
};

#endif