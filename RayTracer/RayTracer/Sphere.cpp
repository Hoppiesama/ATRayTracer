#include "Sphere.h"

double Sphere::intersect(const Ray & r) const
{
	// returns distance, 0 if nohit 
	Vector3 op = position - r.GetOrigin(); // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0 

	double t;
	double epsilon = 1e-4;
	double b = op.dot(r.GetDirection());

	double determinant = b * b - op.dot(op) + radius * radius;

	//hasn't hit
	if(determinant < 0)
	{
		return 0;
	}
	else
	{
		determinant = sqrt(determinant);
	}
	return (t = b - determinant) > epsilon ? t : ((t = b + determinant) > epsilon ? t : 0);
}
