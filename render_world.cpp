#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"

extern bool disable_hierarchy;

Render_World::Render_World()
	:background_shader(0), ambient_intensity(0), enable_shadows(true),
	recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
	delete background_shader;
	for (size_t i = 0; i < objects.size(); i++) delete objects[i];
	for (size_t i = 0; i < lights.size(); i++) delete lights[i];
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t
Hit Render_World::Closest_Intersection(const Ray& ray)
{

	float min_t = std::numeric_limits<float>::max();

	Hit closest_hit;
	closest_hit.object = NULL;

	for (unsigned i = 0; i < objects.size(); i++) {

		Hit temp_hit = objects[i]->Intersection(ray, -1);

		if (temp_hit.object != NULL) {

			if (temp_hit.dist < min_t && temp_hit.dist > small_t) {
				closest_hit = temp_hit;
				min_t = temp_hit.dist;
			}
		}
	}
	if (debug_pixel) {
		std::cout << "closest intersection: " << closest_hit.dist << std::endl;
	}

	return closest_hit;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
	// set up the initial view ray here
	vec3 direction = (camera.World_Position(pixel_index) - camera.position).normalized();
	Ray ray;
	ray.endpoint = camera.position;
	ray.direction = direction;

	vec3 color = Cast_Ray(ray, 1);
	if (debug_pixel) {
		std::cout << "color: " << color <<std::endl;
	}
	camera.Set_Pixel(pixel_index, Pixel_Color(color));
}

void Render_World::Render()
{
	if (!disable_hierarchy)
		Initialize_Hierarchy();

	for (int j = 0; j < camera.number_pixels[1]; j++)
		for (int i = 0; i < camera.number_pixels[0]; i++)
			Render_Pixel(ivec2(i, j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray, int recursion_depth)
{
	vec3 color;

	Hit hit = Closest_Intersection(ray);
	if (recursion_depth <= recursion_depth_limit) {
		if (hit.object != NULL) {

			if (debug_pixel) {
				std::cout << "Shade Surface Location: " << ray.Point(hit.dist) <<", normal " << hit.object->Normal(ray.Point(hit.dist), 0) <<std::endl;
			}
			color = hit.object->material_shader->
				Shade_Surface(ray, ray.Point(hit.dist), hit.object->Normal(ray.Point(hit.dist), hit.part), recursion_depth);

		}
		else {
			
			color = background_shader->
				Shade_Surface(ray, vec3(0, 0, 0), vec3(0, 0, 0), recursion_depth);
		}
	}
	else {
		color = background_shader->
			Shade_Surface(ray, vec3(0, 0, 0), vec3(0, 0, 0), recursion_depth);
	}
	if (debug_pixel) {
		std::cout << "color in cast ray: " << color << std::endl;
	}

	return color;
}

void Render_World::Initialize_Hierarchy()
{
	TODO;
	// Fill in hierarchy.entries; there should be one entry for
	// each part of each object.

	hierarchy.Reorder_Entries();
	hierarchy.Build_Tree();
}
