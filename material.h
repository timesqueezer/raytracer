#ifndef MATERIALH
#define MATERIALH

#include "ray.h"
#include "random.h"


vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0 * vec3(get_random(), get_random(), get_random()) - vec3(1,1,1);
    } while (p.squared_length() >= 1.0);
    return p;
}

vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v,n) * n;
}

class Material {
    public:
        virtual bool scatter(const Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
    public:
        Lambertian(const vec3& a) : albedo(a) {}
        virtual bool scatter(const Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered) const {
            vec3 target = rec.p + rec.normal + random_in_unit_sphere();
            scattered = Ray(rec.p, target - rec.p);
            attenuation = albedo;
            return true;
        }

        vec3 albedo;
};

class Metal : public Material {
    public:
        Metal(const vec3& a, float f) : albedo(a) {
            fuzz = f < 1 ? f : 1;
        }

        virtual bool scatter(const Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered) const {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = Ray(rec.p, reflected + fuzz*random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

        vec3 albedo;
        float fuzz;
};

#endif
