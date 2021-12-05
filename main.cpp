#include "Utilities.h"
#include "Color.h"
#include "Sphere.h"
#include "Hittable_List.h"
#include "Camera.h"
#include "Material.h"

#include <iostream>
#include <fstream>


using namespace std;


Color ray_color(const Ray& r, const Hittable& world, int depth){
    Hit_record rec;


    if (depth<=0)
    return Color(0,0,0);

        if (world.hit(r, 0.001, infinity, rec)) {
        Ray scattered;
        Color attenuation;
        if (rec.matPtr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return Color(0,0,0);
        }

    if(world.hit(r,0.001,infinity,rec)){
       Point3 target = rec.p + random_in_hemisphere(rec.normal);
       return 0.5*ray_color(Ray(rec.p, target-rec.p), world, depth-1);
    }
    

    Vec3 unit_direction=unit_vector(r.direction());
    auto t=0.5*(unit_direction.y()+1.0);
    return (1.0-t)*Color(1.0, 1.0, 1.0)+t*Color(0.5,0.7,1.0);
}

int main ()
{

    //output image dimensions

const auto aspect_ratio=16.0/9.0;
const int fx=400;
const int fy=static_cast<int>(fx/aspect_ratio);
const int samples_per_pixel=100;
const int max_depth=50;

//Envo
    auto R=cos(pi/4);

    Hittable_List world;

   auto material_ground = make_shared<Lambertian>(Color(0.9, 0.9, 0.9));
   auto material_ground1 = make_shared<Dielectric>(1.3);
    auto mat0 = make_shared<Lambertian>(Color(1.0, 0.1, 0.6));
    auto mat1 = make_shared<Lambertian>(Color(0.2, 0.3, 0.8));
    auto metal1   = make_shared<Metal>(Color(0.3, 0.3, 0.7), 0.6);
    auto refractron  = make_shared<Dielectric>(1.5);
    auto metal2  = make_shared<Metal>(Color(0.1, 0.4, 0.2), 0.4);

  world.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
  world.add(make_shared<Sphere>(Point3(0.0, -100, -1.0), 30.0, material_ground1));
  world.add(make_shared<Sphere>(Point3(-R, 0,-1), R, mat1));
  world.add(make_shared<Sphere>(Point3(R, 0,-4), R, metal1));
  world.add(make_shared<Sphere>(Point3(R, 0,0.5), R, mat0));
  world.add(make_shared<Sphere>(Point3(R, 0,-2.5), R, metal2));
  world.add(make_shared<Sphere>(Point3(-R, 0,-4.5), -R, refractron));
   world.add(make_shared<Sphere>(Point3(-R, 0.5,0), -R, refractron));
   world.add(make_shared<Sphere>(Point3(-R, 0,-8), R, mat1));
  world.add(make_shared<Sphere>(Point3(R, 0,-7.5), R, metal1));
  world.add(make_shared<Sphere>(Point3(-R, 0,-8), R, mat1));
  world.add(make_shared<Sphere>(Point3(R, 0,-7.5), R, metal1));
  world.add(make_shared<Sphere>(Point3(R, 0,-7), R, mat0));
  world.add(make_shared<Sphere>(Point3(R, 0,-6.5), R, metal2));
  world.add(make_shared<Sphere>(Point3(-R, 0,-4.5), -R, refractron));
   world.add(make_shared<Sphere>(Point3(-R, 0.5,6), -R, refractron));
    //cam

Camera cam (Point3(-2,2,1), Point3(0,0,-1), Vec3(0,1,0), 90, aspect_ratio);

// output
ofstream output;
output.open("output.ppm");
output<<"P3\n"<<fx<<" "<<fy<<"\n255\n";
for (int j=fy-1; j>=0;j--)
{
    std::cerr<<"\rCreating environment... Lines remaining: "<< j<< ' '<<std::flush;
    

    for (int i=0; i<fx;i++)
    {
Color pixel_color(0,0,0);
for (int s=0; s<samples_per_pixel;++s){
    auto u=(i+random_double())/(fx-1);
    auto v=(j+random_double())/(fy-1);
    Ray r = cam.get_ray(u,v);
    pixel_color+=ray_color(r,world, max_depth);
}
write_color(output,pixel_color,samples_per_pixel);

    
    }
}
cerr<<"\nDone.\n";
output.close();

    return 0;
}

