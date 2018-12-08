#pragma once
#include "Material.h"
#include "Ray.h"

enum ObjType
{
	Sph,
	Mod,
};

class Object
{

public:
	Object() = default;
	virtual ~Object() = default;

	Vector3 position;

	Material material;

	ObjType type = Sph;

	virtual double intersect(const Ray &r) const = 0;

	virtual double GetLowestXVert() = 0;
	virtual double GetHighestXVert() = 0;

	virtual double GetLowestYVert() = 0;
	virtual double GetHighestYVert() = 0;

	virtual double GetLowestZVert() = 0;
	virtual double GetHighestZVert() = 0;

private:

};

//Vector3 radiance(const Ray &r, int depth, unsigned short *xSubi, std::vector<Object*> objects, BoundingVolumeHierarchy* bvh)
//{
//	double t;                   // distance to intersection 
//	int id = 0;					// id of intersected object 
//
//	if(!intersect(r, t, id, objects))
//	{
//		return Vector3(0.0, 0.0, 0.0); // if miss, return black 
//	}
//
//	//const Sphere &hitObj = spheres[id];        // the hit object 
//	Object* hitObj = objects.at(id);
//
//	Vector3 hitPoint = r.GetOrigin() + r.GetDirection()*t;
//
//	Vector3 normal;
//
//	if(hitObj->type == Sph)
//	{
//		normal = (hitPoint - hitObj->position).normalize();
//	}
//	else if(hitObj->type == Mod)
//	{
//		//Triangle* temp = dynamic_cast<Triangle>(*hitObj);
//		Triangle* panim = dynamic_cast<Triangle*>(hitObj);
//
//		Vector3 v0v1 = panim->vertB.pos - panim->vertA.pos;
//		Vector3 v0v2 = panim->vertC.pos - panim->vertA.pos;
//		normal = v0v1.cross(v0v2).normalize(); // N 
//	}
//
//	//Vector3 normal = (hitPoint - hitObj->position).normalize();
//	Vector3 nl = normal.dot(r.GetDirection()) < 0 ? normal : normal * -1;
//	Vector3 objectColour = hitObj->colour;
//
//	//TODO - remove this, it's super simple to test BVH
//	//return hitObj->colour;
//
//
//	double probability;
//	// Get the highest colour value to determine likelihood of continuing in russian roulette.
//	if(objectColour.x > objectColour.y && objectColour.x > objectColour.z)
//	{
//		probability = objectColour.x;
//	}
//	else if(objectColour.y > objectColour.z)
//	{
//		probability = objectColour.y;
//	}
//	else
//	{
//		probability = objectColour.z;
//	}
//
//	//After 5 radiance recursions, uses random 
//	//chance to end based on highest value in object colour
//	if(++depth > 5)
//	{
//		//in the case that p = 0 (no colour value), it can't possibly trigger and emission is returned instead.
//		if(erand48(xSubi) < probability)
//		{
//			objectColour = objectColour * (1 / probability);
//		}
//		else
//		{
//			return hitObj->emission; //R.R. 
//		}
//	}
//
//	if(hitObj->refl == DIFF)
//	{
//		// Ideal DIFFUSE reflection 
//		double r1 = 2 * M_PI*erand48(xSubi);
//		double r2 = erand48(xSubi);
//		double r2s = sqrt(r2);
//
//		Vector3 w = nl;
//		Vector3 u;
//
//		//I THINK this is for determining whether a surface deflects light or not
//		if(fabs(w.x) > 0.1)
//		{
//			u = Vector3(0, 1, 0).cross(w);
//		}
//		else
//		{
//			u = Vector3(1, 0, 0).cross(w);
//		}
//		u.normalize();
//
//		Vector3 v = w.cross(u); // % = cross
//
//								// TOM!!! - is this an implementation of the monte carlo thing?
//								//New direction randomised slightly due to diffuse colour.
//								// u * cos(randomNo betwen 0 and 2Pi) + v * sin(same randomNo between 0 and 2Pi) + w * sqrt(1 - randomNo between 0.0 & 1.0) . normalized
//		Vector3 newDirection = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).normalize();
//
//		Ray ray(hitPoint, newDirection);
//
//		objects.clear();
//
//		bvh->intersectTree(ray, bvh->thisNode.get(), objects);
//
//		return hitObj->emission + objectColour.multiplyBy(radiance(ray, depth, xSubi, objects, bvh));
//	}
//	else if(hitObj->refl == SPEC)            // Ideal SPECULAR reflection 
//	{
//
//		Vector3 newDirection = r.GetDirection() - normal * 2 * normal.dot(r.GetDirection());
//
//		Ray ray(hitPoint, newDirection);
//
//		objects.clear();
//
//		bvh->intersectTree(ray, bvh->thisNode.get(), objects);
//
//		//r.GetDirection() - normal * 2 * normal.dot(r.GetDirection()) is a mirrored angle from the normal, e.g. angle of incidence and angle of reflection from the normal are the same.
//		//based on the idea shown here https://youtu.be/ytRrjf9OPHg?t=1031 ...
//		return hitObj->emission + objectColour.multiplyBy(radiance(ray, depth, xSubi, objects, bvh));
//	}
//
//	//dielectric REFRACTION 
//	Ray reflRay(hitPoint, r.GetDirection() - normal * 2 * normal.dot(r.GetDirection()));
//
//	// Check if Ray is from the outside going in? 
//	bool into = normal.dot(nl) > 0;
//
//	double airRefractionIndex = 1;
//
//	//TODO - replace this with access to hit object's material "Index of Refraction"
//	double glassRefractionIndex = 1.5;
//
//
//	double nnt;
//
//	//If going in, we calculate the change in angle due to the refraction. If coming out, it's inverted back.
//	if(into)
//	{
//		nnt = airRefractionIndex / glassRefractionIndex; // == 1/1.5 == 0.666666
//	}
//	else
//	{
//		nnt = glassRefractionIndex / airRefractionIndex;  // == 1.5/1 == 1.5
//	}
//
//
//
//	double rayDirectionDotNormal = r.GetDirection().dot(nl);
//
//
//	//Calculate angle of ray relative to surface.
//	double cosine2Theta = 1 - nnt * nnt * (1 - rayDirectionDotNormal * rayDirectionDotNormal);
//
//	/*When the angle of incident is greater than some value called the critical angle,
//	then 100 % of the light incident on the surface is reflected.In another words,
//	when the angle of incident is greater than the critical angle, there isn't any refraction at all.
//	This only happens though when the light ray passes from one medium to another medium with a lower index of refraction,
//	such as in the case of a water-air, diamond-water or glass-water interaction. This phenomenon is called ...
//
//	"total internal reflection."	*/
//	if(cosine2Theta < 0)
//	{
//		objects.clear();
//		bvh->intersectTree(reflRay, bvh->thisNode.get(), objects);
//		return hitObj->emission + objectColour.multiplyBy(radiance(reflRay, depth, xSubi, objects, bvh));
//	}
//
//	Vector3 tdir;
//	if(into)
//	{
//		//fresnel term?
//		tdir = r.GetDirection()*nnt - normal * (1 * (rayDirectionDotNormal*nnt + sqrt(cosine2Theta)));
//	}
//	else
//	{
//		//fresnel term?
//		tdir = r.GetDirection()*nnt - normal * (-1 * (rayDirectionDotNormal*nnt + sqrt(cosine2Theta)));
//	}
//	tdir.normalize();
//
//	double a = glassRefractionIndex - airRefractionIndex; // == 1.5 - 1 == 0.5
//	double b = glassRefractionIndex + airRefractionIndex; // == 1.5 + 1 = 2.5
//	double reflectanceAtNormalIncidence = (a * a) / (b*b); // == 0.5^2 / (b^2)
//
//	double c;	// c = 1 - cos(theta)
//	if(into)
//	{
//		c = 1 - (-rayDirectionDotNormal);
//	}
//	else
//	{
//		c = tdir.dot(normal);
//	}
//
//	double fresnelReflectance = reflectanceAtNormalIncidence + (1 - reflectanceAtNormalIncidence)*c*c*c*c*c;
//	double Tr = 1 - fresnelReflectance;
//	double probabilityOfReflecting = .25 + .5*fresnelReflectance;
//	double probabilityOfRefracting = fresnelReflectance / probabilityOfReflecting;
//	double TP = Tr / (1 - probabilityOfReflecting);
//
//
//
//	if(depth > 2)
//	{
//		if(erand48(xSubi) < probabilityOfReflecting)
//		{
//			objects.clear();
//			bvh->intersectTree(reflRay, bvh->thisNode.get(), objects);
//
//			return hitObj->emission + objectColour.multiplyBy(radiance(reflRay, depth, xSubi, objects, bvh)*probabilityOfRefracting);
//		}
//		else
//		{
//			Ray ray(hitPoint, tdir);
//			objects.clear();
//			bvh->intersectTree(ray, bvh->thisNode.get(), objects);
//
//			return hitObj->emission + objectColour.multiplyBy(radiance(ray, depth, xSubi, objects, bvh)*TP);
//		}
//	}
//	else
//	{
//		Ray ray2(hitPoint, tdir);
//
//		objects.clear();
//		bvh->intersectTree(reflRay, bvh->thisNode.get(), objects);
//		bvh->intersectTree(ray2, bvh->thisNode.get(), objects);
//		return hitObj->emission + objectColour.multiplyBy((radiance(reflRay, depth, xSubi, objects, bvh) * fresnelReflectance + radiance(ray2, depth, xSubi, objects, bvh)*Tr));
//	}
//}
