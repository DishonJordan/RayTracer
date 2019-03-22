#include "mesh.h"
#include <fstream>
#include <string>
#include <limits>

// Consider a triangle to intersect a ray if the ray intersects the plane of the
// triangle with barycentric weights in [-weight_tolerance, 1+weight_tolerance]
static const double weight_tolerance = 1e-4;

// Read in a mesh from an obj file.  Populates the bounding box and registers
// one part per triangle (by setting number_parts).
void Mesh::Read_Obj(const char* file)
{
    std::ifstream fin(file);
    if(!fin)
    {
        exit(EXIT_FAILURE);
    }
    std::string line;
    ivec3 e;
    vec3 v;
    box.Make_Empty();
    while(fin)
    {
        getline(fin,line);

        if(sscanf(line.c_str(), "v %lg %lg %lg", &v[0], &v[1], &v[2]) == 3)
        {
            vertices.push_back(v);
            box.Include_Point(v);
        }

        if(sscanf(line.c_str(), "f %d %d %d", &e[0], &e[1], &e[2]) == 3)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
        }
    }
    number_parts=triangles.size();
}

// Check for an intersection against the ray.  See the base class for details.
Hit Mesh::Intersection(const Ray& ray, int part) const
{

	Hit result;
	result.object = NULL;

	if (part >= 0) {
		double temp_dist;

		if (Intersect_Triangle(ray, part, temp_dist)) {
			result.object = this;
			result.dist = temp_dist;
			result.part = part;
		}
	}
	else {
		float min_t = std::numeric_limits<float>::max();

		double temp_dist;
		for (int i = 0; i < triangles.size(); i++) {
			if (Intersect_Triangle(ray, i, temp_dist)) {
				if (temp_dist < min_t) {
					result.object = this;
					result.dist = temp_dist;
					result.part = i;
					min_t = temp_dist;
				}
				if (debug_pixel) {
					std::cout << "Intersected with part " << i << " dist:" << temp_dist <<std::endl;
				}
			
			}
		
		}
	
	
	}
	   	  
	if (debug_pixel) {
		std::cout << "dist: " << result.dist << std::endl;
	}

 
    return result;
}

// Compute the normal direction for the triangle with index part.
vec3 Mesh::Normal(const vec3& point, int part) const
{
    assert(part>=0);
	vec3 normal;
	ivec3 current_triangle = triangles[part];
	vec3 vertex_a = vertices[current_triangle[0]];
	vec3 vertex_b = vertices[current_triangle[1]];
	vec3 vertex_c = vertices[current_triangle[2]];

	vec3 ac = vertex_c - vertex_a;
	vec3 ab = vertex_b - vertex_a;

	normal = cross(ab, ac).normalized();

    return normal;
}

// This is a helper routine whose purpose is to simplify the implementation
// of the Intersection routine.  It should test for an intersection between
// the ray and the triangle with index tri.  If an intersection exists,
// record the distance and return true.  Otherwise, return false.
// This intersection should be computed by determining the intersection of
// the ray and the plane of the triangle.  From this, determine (1) where
// along the ray the intersection point occurs (dist) and (2) the barycentric
// coordinates within the triangle where the intersection occurs.  The
// triangle intersects the ray if dist>small_t and the barycentric weights are
// larger than -weight_tolerance.  The use of small_t avoid the self-shadowing
// bug, and the use of weight_tolerance prevents rays from passing in between
// two triangles.
bool Mesh::Intersect_Triangle(const Ray& ray, int tri, double& dist) const
{
	bool result = false;
	ivec3 current_triangle = triangles[tri];
	vec3 vertex_a = vertices[current_triangle[0]];
	vec3 vertex_b = vertices[current_triangle[1]];
	vec3 vertex_c = vertices[current_triangle[2]];

	vec3 u = ray.direction.normalized();
	vec3 v = vertex_b - vertex_a;
	vec3 w = vertex_c - vertex_a;
	vec3 y = ray.endpoint - vertex_a;

	float denom = dot(cross(u, v), w);

	float t = -(dot(cross(v, w),y)) / (dot(cross(v, w),u));
	
	if (t > small_t) {

		float beta = dot(cross(w, u), y) / denom;
		float gamma = dot(cross(u, v), y) / denom;

		if (beta > weight_tolerance && gamma > weight_tolerance && (1 - gamma - beta) > weight_tolerance) {

			result = true;
			dist = t;
		}
	}
    return result;
}

// Compute the bounding box.  Return the bounding box of only the triangle whose
// index is part.
Box Mesh::Bounding_Box(int part) const
{
    Box b;
    TODO;
    return b;
}
