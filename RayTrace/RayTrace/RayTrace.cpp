// RayTrace.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "vector"

#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "rtweekend.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "moving_sphere.h"
#include <thread>
#include <fstream>
#include "Timer.h"

using namespace std;

double hit_sphere(const point3& center, double radius, const ray& r);
color ray_color(const ray& r);
color ray_color(const ray& r, const hittable& world, int depth);

void lab8();

hittable_list two_perlin_spheres()
{
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    return objects;
}


static hittable_list two_spheres() {
    hittable_list objects;
    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

//    objects.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
    objects.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    return objects;
}


static hittable_list random_scene() {
    hittable_list world;

    // �ر���ʣ�ɢ�����ʣ���ɫ
//    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    auto ground_material = make_shared<lambertian>(make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9,0.9,0.9)));

    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int i = -11; i < 11; i++) {
        for (int j = -11; j < 11; j++) {

            auto choose_mat = random_double();
            
            point3 center(i + 0.9 * random_double(), 0.2, j + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                // ���������ʸ�����С��0.8��������ɢ����ʣ���80%�ĸ��� 
                if (choose_mat < 0.8) {

                    //diff
                    auto albedo = vec3::random() * vec3::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0, 0.5), 0);
                    world.add(make_shared<moving_sphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));
                }
                // ���������ʸ�����С��0.95���������������ʣ���15%�ĸ��� 
                else if (choose_mat < 0.95) {
                    //metal
                    auto albedo = vec3::random();
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                // ʣ�µ�5%�ĸ��ʴ��������� 
                else {
                    //glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }

        }
    }
    // ��������
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
    //world.add(make_shared<sphere>(point3(0, 1, 0), -0.9, material1));

    // ɢ�����
    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    // ��������
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

int main()
{
    lab8();
}


static void myThread(int start, int end, int image_height, int image_width, int samples_per_pixel, int max_depth, camera &cam, hittable_list &world, vector<color> &res) {
    for (int j = start - 1; j >= end; --j) {
        std::cerr << "\rScanlines nowing: " << j << ' ' << std::endl;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            res.push_back(pixel_color);
        }
    }
}



void lab8() {



    //Image

    const auto aspect_ratio = 16.0 / 9.0;
    const auto image_width = 1600;
    const auto image_height = static_cast<int>(image_width / aspect_ratio);
    const auto samples_per_pixel = 50;
    const int max_depth = 50;

    //World
//    auto world = random_scene();
    //��������
    auto world = two_perlin_spheres();

    //Camera;
    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10;
    //auto dist_to_focus = (lookfrom - lookat).length();
//    auto aperture = 0.1;
    auto aperture = 0.0;
    //camera cam(point3(-2, 2, 1), point3(0, 0, -1), vec3(0, 1, 0), 90, aspect_ratio);
    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);


    ofstream outfile;
    outfile.open("myImg.ppm", ios::out);
    cout << getTime() << endl;
    // Render
    outfile << "P3\n" << image_width << " " << image_height << "\n255\n";


    int threads_num = 10;
    int per_thread = (image_height - 1) / threads_num + 1;
    vector<thread> threads(threads_num);
    vector<vector<color>> results(threads_num, vector<color>());
    int ih = image_height;

    for (int i = 0; i < threads_num; i++) {
        threads[i] = std::thread(myThread,ih, std::max(0, ih - per_thread), image_height, image_width, samples_per_pixel, max_depth, ref(cam), ref(world), ref(results[i]));
        ih -= per_thread;
    }

    for (auto& t : threads) {
        t.join();
    }

    for (auto& res : results) {
        for (auto& pixel_color : res) {
            write_color(outfile, pixel_color, samples_per_pixel);
        }
    }
    outfile.close();
    
    std::cerr << "\nDone.\n";

}




inline double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return -1.0;
    }
    else {
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}

color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
    }
    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}


//<8
//color ray_color(const ray& r, const hittable& world) {
//    hit_record rec;
//    if (world.hit(r, 0, infinity, rec)) {
//        return 0.5 * (rec.normal + color(1, 1, 1));
//    }
//
//    vec3 unit_direction = unit_vector(r.direction());
//    auto t = 0.5 * (unit_direction.y() + 1.0);
//    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
//
//}

//8

//color ray_color(const ray& r, const hittable& world, int depth) {
//    if (depth <= 0) return color(0, 0, 0);
//    hit_record rec;
//    if (world.hit(r, 0.001, infinity, rec)) {
//        point3 target = rec.p + rec.normal + vec3::random_unit_vector();
//        return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
//    }
//
//    vec3 unit_direction = unit_vector(r.direction());
//    auto t = 0.5 * (unit_direction.y() + 1.0);
//    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
//
//}


//��������
color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0, 0, 0);

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return color(0, 0, 0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}








// ���г���: Ctrl + F5 ����� >����ʼִ��(������)���˵�
// ���Գ���: F5 ����� >����ʼ���ԡ��˵�

// ����ʹ�ü���: 
//   1. ʹ�ý��������Դ�������������/�����ļ�
//   2. ʹ���Ŷ���Դ�������������ӵ�Դ�������
//   3. ʹ��������ڲ鿴���������������Ϣ
//   4. ʹ�ô����б��ڲ鿴����
//   5. ת������Ŀ��>���������Դ����µĴ����ļ�����ת������Ŀ��>�����������Խ����д����ļ���ӵ���Ŀ
//   6. ��������Ҫ�ٴδ򿪴���Ŀ����ת�����ļ���>���򿪡�>����Ŀ����ѡ�� .sln �ļ�
