#include "Sphere.h"

double Sphere::intersect(const Ray & r) const
{
	// returns distance, 0 if nohit 
	Vector3 originToPositionDirection = position - r.GetOrigin(); // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0 

	double t;
	double epsilon = 1e-4;
	double originToPosDirDotRayDir = originToPositionDirection.dot(r.GetDirection());

	double determinant = originToPosDirDotRayDir * originToPosDirDotRayDir - originToPositionDirection.dot(originToPositionDirection) + radius * radius;

	//hasn't hit
	if(determinant < 0)
	{
		return 0;
	}
	else
	{
		determinant = sqrt(determinant);
	}
	//if hit return t, else return 0
	return (t = originToPosDirDotRayDir - determinant) > epsilon ? t : ((t = originToPosDirDotRayDir + determinant) > epsilon ? t : 0);
}
