// @Author Mohammed Alzakariya
// The ViewContext defines computes device coordinates given a model's
// As well as allowing various transformations on the view

#include "ViewContext.h"
#include <cmath>

ViewContext::ViewContext(double deviceHeight, double deviceWidth)
	: composite(4,4), compositeInv(4,4), vTm(4,4), pTv(4,4), dTp(4,4),
	  translation(4,4), translationInv(4,4), rotation(4,4), rotationInv(4,4), 
	  scale(4,4), scaleInv(4,4), netTranslation(4,4), netTranslationInv(4,4),
	  netRotation(4,4), netRotationInv(4,4), netScale(4,4), netScaleInv(4,4),
	  deviceWidth(deviceWidth), deviceHeight(deviceHeight)
{
	reset();
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
	
	// normalize 4th component TODO: has to be done here?
	point = 1/point[3][0] * point;

	
	return point;
}
	
matrix ViewContext::modelToDevice(const matrix& points) const
{
	matrix devPts = composite * points;
	// normalize 4th component TODO: has to be done here?
	for(int p=0; p<devPts.getCols(); p++)
	{
		for(int r=0; r<4; r++)
		{
			devPts[r][p] = 1/devPts[3][p] * devPts[r][p];
		}
	}
	
	return devPts;
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

void ViewContext::reset()
{
	// reset all accumulations
	netTranslation = netTranslationInv = netRotation = netRotationInv
			= netScale = netScaleInv = matrix::identity(4);
	// reset rotate(), translate(), and zoom() to have no effect
	configTranslation(0, 0, 0);
	configRotation(0);
	configZoom(1);
	
	// reset the composite matrix so that it transforms from model to
	// device, with no other transformations
	resetComposite();
}

void ViewContext::translate()
{
	// translate in the model coordinates
	netTranslation = netTranslation * translation;
	netTranslationInv = translationInv * netTranslationInv;
	
	updateComposite();
}
	
void ViewContext::rotate(bool cw)
{
	// rotate in the model coordinates
	if (cw)
	{
		netRotation = netRotation * rotation;
		netRotationInv = rotationInv * netRotationInv;
	} else
	{
		netRotation = netRotation * rotationInv;
		netRotationInv = rotation * netRotationInv;
	}
	
	updateComposite();
}

void ViewContext::zoom(bool in) 
{	
	// accumulate to netZoom
	if (in) 
	{
		netScale = netScale * scale;
		netScaleInv = scaleInv * netScaleInv;
	} else {
		netScale = netScale * scaleInv;
		netScaleInv = scale * netScaleInv;
	}
	
	updateComposite();
}

void ViewContext::updateComposite()
{
	resetComposite();
	// apply zoom, rotation, than translation
	composite = composite * netTranslation * netRotation * netScale;
	compositeInv = netScaleInv * netRotationInv * 
			netTranslationInv * compositeInv;
}

void ViewContext::resetComposite()
{	
	// Device = dTp*S*pTv*vTm*O*Model
	config_vTm(0,0,200);
	config_pTv(25);
	config_dTp();
	composite = dTp*pTv*vTm;

	
	/* old resetComposite TODO: remove?
	// reflect y
	matrix reflectY = matrix::identity(4);
	reflectY[1][1] = -1;
	composite = reflectY;
	compositeInv = reflectY;
	
	// image is inverted, and above the screen, translate down and right
	// DEVICE [reflectY][translate] MODEL (translate right-mults reflectY)
	composite = computeTranslation(deviceWidth/2, deviceHeight/2, 0) * composite;
	compositeInv = compositeInv * computeTranslation(-deviceWidth/2, -deviceHeight/2, 0);
	*/
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

// Internal configuration of model to view
// computes matrices for conversion  from x,y,z to L,M,N
// viewing coordinates. This changes as the viewing point 
// changes via orbit
// this updates the ModelToView and ViewToModel matrices
// @params (p0x, p0y, p0z) coords of view point
void ViewContext::config_vTm(double p0x, double p0y, double p0z)
{
	// converting from x,y,z coords to L,M,N coords... N = Pr -> p0
	matrix N(3,1);
	N[0][0] = p0x;
	N[1][0] = p0y;
	N[2][0] = p0z;
	// L = N x V, where V = y
	matrix L(3,1);
	L[0][0] = N[2][0]; // VyNz - NyVz
	L[1][0] = 0; // VzNx - NzVx, Vz and Vx always 0
	L[2][0] = -N[0][0];
	// M = N x L
	matrix M(3,1);
	M[0][0] = N[1][0]*L[2][0] - N[2][0]*L[1][0];
	M[1][0] = N[2][0]*L[0][0] - N[0][0]*L[2][0];
	M[2][0] = N[0][0]*L[1][0] - N[1][0]*L[0][0];
	// compute lengths and reduce to unit vectors!
	double l = std::sqrt(L[0][0]*L[0][0] + L[1][0]*L[1][0] + L[2][0]*L[2][0]);
	double m = std::sqrt(M[0][0]*M[0][0] + M[1][0]*M[1][0] + M[2][0]*M[2][0]);
	double n = std::sqrt(N[0][0]*N[0][0] + N[1][0]*N[1][0] + N[2][0]*N[2][0]);
	L = 1/l*L;
	M = 1/m*M;
	N = 1/n*N;
	// figure out x, y and z translations TODO: change back to 0 0 -n
	double xTran = L[0][0]*p0x + L[1][0]*p0y + L[2][0]*p0z;
	double yTran = M[0][0]*p0x + M[1][0]*p0y + M[2][0]*p0z;
	double zTran = N[0][0]*p0x + N[1][0]*p0y + N[2][0]*p0z;	
	// now update vTm! -n*P0 ends up being -n in the direction of N!
	vTm[0][0]=L[0][0]; vTm[0][1]=L[1][0]; vTm[0][2]=L[2][0]; vTm[0][3]=-xTran;
	vTm[1][0]=M[0][0]; vTm[1][1]=M[1][0]; vTm[1][2]=M[2][0]; vTm[1][3]=-yTran;
	vTm[2][0]=N[0][0]; vTm[2][1]=N[1][0]; vTm[2][2]=N[2][0]; vTm[2][3]=-zTran;
	vTm[3][0]=0; 	   vTm[3][1]=0; 	  vTm[3][2]=0; 	     vTm[3][3]=1;

}

// Internal configuration of view to view plane
// determines the projection matrix based on the focal point location
// in the z coordinate of the view coordinate, zf
// copmutes and updates the view to view plane matrix
// matrices
// @param zf focal point distance from p0. Must be positive: behind view plane
// @throws viewContextException if zf <= 0
void ViewContext::config_pTv(const double zf)
{
	if (zf <= 0)
	{
		throw viewContextException("focal point must be behind the view plane!");
	}
	
	pTv = matrix::identity(4);
	// no z after projection
	pTv[2][2] = 0;
	// perspective projection
	pTv[3][2] = -1/zf;
}
	
// Computes the mapping from the view plane coordinates to the
// device coordinates. This involves the same rules for the 2D composite
// transformations, in addition to normalizing the 4th component,
// and scaling and transforming so that the view plane is fully visible
void ViewContext::config_dTp()
{
	// TODO: implement????
	// old resetComposite...
	// reflect y
	dTp = matrix::identity(4);
	dTp[1][1] = -1;
		
	// image is inverted, and above the screen, translate down and right
	// DEVICE [reflectY][translate] MODEL (translate right-mults reflectY)
	dTp = computeTranslation(deviceWidth/2, deviceHeight/2, 0) * dTp;	
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