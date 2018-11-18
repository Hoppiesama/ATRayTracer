#include "Sphere.h"

double Sphere::intersect(const Ray & r) const
{
	// returns distance, 0 if nohit 
	Vector3 op = position - r.GetOrigin(); // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0 

	double t;
	double eps = 1e-4;
	double b = op.dot(r.GetDirection());
	double det = b * b - op.dot(op) + rad * rad;

	if(det < 0)
	{
		return 0;
	}
	else
	{
		det = sqrt(det);
	}
	return (t = b - det) > eps ? t : ((t = b + det) > eps ? t : 0);
}
