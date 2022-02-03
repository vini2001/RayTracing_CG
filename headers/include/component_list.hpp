#ifndef COMPONENT_LIST_HPP
#define COMPONENT_LIST_HPP

#include "hittable.hpp"

#include <memory>
#include <vector>

using namespace std;

class ComponentList : public Hittable {
    public:

        vector<shared_ptr<Hittable>> objects;

        ComponentList() {}
        ComponentList(shared_ptr<Hittable> object) {
            add(object);
        }

        void clear() { 
            objects.clear();
        }

        void add(shared_ptr<Hittable> object) {
            objects.push_back(object);
        }

        virtual bool hit(const Ray& r, double tMin, double tmaX, HitRecord& rec) const override;
};

bool ComponentList::hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const {
    HitRecord tempRecord;
    bool hitAnything = false;
    double closest = tMax;

    for(const auto& ob : objects) {
        if(ob->hit(r, tMin, closest, tempRecord)) {
            hitAnything = true;
            closest = tempRecord.t;
            rec = tempRecord;
        }
    }

    return hitAnything;
}

#endif // !COMPONENT_LIST_HPP