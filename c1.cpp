#include <vector>
#include <sstream>
#include <iostream>
#include <float.h>
#include <omp.h>

#include "random.h"
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "hitable_list.h"
#include "material.h"
#include "camera.h"


vec3 color(const Ray& r, Hitable *world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        Ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1);

        } else {
            return vec3(0, 0, 0);

        }

    } else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.3, 0.5, 1.0);

    }
}

int main() {
    int nx = 1200;
    int ny = 600;
    int ns = 100;

    std::cerr << get_random() << std::endl;
    std::cerr << get_random() << std::endl;
    std::cerr << get_random() << std::endl;

    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    omp_set_num_threads(6);

    int num_threads = omp_get_num_threads();

    uint32_t* image_data = (uint32_t*) malloc(sizeof(uint32_t) * nx * ny * 3);

    Hitable* list[4];
    list[0] = new Sphere(vec3(0, 0, -1), 0.5, new Lambertian(vec3(0.8, 0.3, 0.3)));
    list[1] = new Sphere(vec3(0, -100.5, -1), 100, new Lambertian(vec3(0.1, 0.3, 0.2)));
    list[2] = new Sphere(vec3(1, 0, -1), 0.5, new Metal(vec3(0.8, 0.4, 0.7), 0.3));
    list[3] = new Sphere(vec3(-1, 0, -1), 0.5, new Metal(vec3(1.0, 1.0, 1.0), 0.9));
    Hitable* world = new HitableList(list, 4);

    Camera cam;

    int i, j, k, ir, ig, ib, num, s;
    float u, v;
    vec3 col, p;
    Ray r;

    #pragma omp parallel for private(j, i, k, col, u, v, r, p, ir, ig, ib, num, s) shared(image_data, world, cam)
    for (k = 1; k <= ny; k++) {
        j = ny - k;
        num = omp_get_thread_num();
        // seed_random(num*1234);
        // std::cerr << num << std::endl;

        for (i = 0; i < nx; i++) {
            col = vec3(0, 0, 0);

            for (s = 0; s < ns; s++) {
                u = float(i + get_random()) / float(nx);
                v = float(j + get_random()) / float(ny);

                r = cam.get_ray(u, v);
                p = r.point_at_t(2.0);

                col += color(r, world, 0);
            }

            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            ir = uint32_t(255.99*col[0]);
            ig = uint32_t(255.99*col[1]);
            ib = uint32_t(255.99*col[2]);

            image_data[(j*nx*3) + (i*3)] = ir;
            image_data[(j*nx*3) + (i*3) + 1] = ig;
            image_data[(j*nx*3) + (i*3) + 2] = ib;

            // std::cout << j << " " << i << " " << (j*nx) + (i*3) << std::endl;

        }

    }

    for (k = 1; k <= ny; k++) {
        j = ny - k;
        for (i = 0; i < nx; i++) {
            std::cout << image_data[(j*nx*3) + (i*3)] << " " << image_data[(j*nx*3) + (i*3) + 1] << " " << image_data[(j*nx*3) + (i*3) + 2] << "\n";
        }
    }

    free_random();
    free(image_data);
}
