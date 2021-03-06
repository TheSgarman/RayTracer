#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "Hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class Hittable_List:public Hittable {
    public:
    Hittable_List(){}
    Hittable_List(shared_ptr<Hittable> object) {add(object);}

    void clear(){objects.clear();}
    void add(shared_ptr<Hittable> object) {objects.push_back(object);}

    virtual bool Hittable_List::hit (const Ray& r, double t_min, double t_max, Hit_record& rec) const override;

    public:
    std::vector<shared_ptr<Hittable>> objects;
};

bool Hittable_List::hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const {
    Hit_record temp_rec;
    bool hit_anything=false;
    auto closest_so_far=t_max;
        
        for (const auto& object:objects) {
            if (object->hit(r, t_min, closest_so_far, temp_rec))
            {
                hit_anything=true;
                closest_so_far=temp_rec.t;
                rec=temp_rec;
            }
        
    }
    return hit_anything;
}

#endif