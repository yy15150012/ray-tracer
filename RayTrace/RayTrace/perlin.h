//
// Created by Administrator on 2022/8/22.
//

#ifndef RAY_TRACER_PERLIN_H
#define RAY_TRACER_PERLIN_H
#include "rtweekend.h"

#include "vec3.h"
class perlin {
public:
    perlin() {
        randfloat = new double[point_count];
        for (int i = 0; i < point_count; i++) {
            randfloat[i] = random_double();
        }

        perm_x = perlin_generate_perm();
        perm_y = perlin_generate_perm();
        perm_z = perlin_generate_perm();
    }

    ~perlin(){
        delete[] randfloat;
        delete[] perm_x;
        delete[] perm_y;
        delete[] perm_z;
    }

    double noise(const point3 &p) const {
        auto i = static_cast<int>(4*p.x()) & 255;
        auto j = static_cast<int>(4*p.y()) & 255;
        auto k = static_cast<int>(4*p.z()) & 255;
        return randfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
    }

    static int* perlin_generate_perm() {
        auto p = new int [point_count];
        for (int i = 0; i < point_count; ++i) {
            p[i] = i;
        }
        permute(p, point_count);
        return p;
    }

    static void permute(int* p, int n) {
        for (int i = n - 1; i > 0; i--) {
            int target = random_int(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }


public:
    static const int point_count = 256;
    double* randfloat;
    int *perm_x, *perm_y, *perm_z;
};

#endif //RAY_TRACER_PERLIN_H
