#include "sphere.h"
#include "ray.h"
#include "algorithm"


// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
	Hit result;
	vec3 ray_to_sphere = ray.endpoint - center;

	float a = dot(ray.direction,ray.direction);
	float b = 2 * dot(ray.direction, ray_to_sphere);
	float c = dot(ray_to_sphere, ray_to_sphere) - (radius * radius);

	float discriminant = (b*b) - 4 * a * c;


	if (discriminant > 0) {

		float t_1 = -b + sqrt(discriminant);
		float t_2 = -b - sqrt(discriminant);

		if (t_1 >= 0 && t_2 >= 0) {

			float t = std::min(t_1, t_2);
			result.object = this;
			result.dist = t;
			
		}
		else if (t_1 >= 0 && t_2 < 0) {

			result.object = this;
			result.dist = t_1;

		}
		else if (t_2 >= 0 && t_1 < 0) {

			result.object = this;
			result.dist = t_2;

		}
		else {
			result.object = NULL;
		}
		
	}
	else
	{
		result.object = NULL;
	}

    return result;
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
    // compute the normal direction
	normal = (point - center).normalized();
    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}
