//
// Created by Administrator on 2022/8/22.
//
#pragma once
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

    ~perlin() {
        delete[] randfloat;
        delete[] perm_x;
        delete[] perm_y;
        delete[] perm_z;
    }

    double turb(const point3& p, int depth=7) const {
        auto accum = 0.0;
        auto temp_p = p;
        auto weight = 1.0;

        for (int i = 0; i < depth; i++) {
            accum += weight*noise(temp_p);
            weight *= 0.5;
            temp_p *= 2;
        }

        return fabs(accum);
    }


//    使用三线性插值的 Perlin
//    https://zhuanlan.zhihu.com/p/77496615
    double noise(const point3 &p) const {
        //差值比率，构造三线性差值时用
        auto u = p.x() - floor(p.x());
        auto v = p.y() - floor(p.y());
        auto w = p.z() - floor(p.z());


//        Hermitian 厄米平滑改进
        u = u * u * (3 - 2 * u);
        v = v * v * (3 - 2 * v);
        w = w * w * (3 - 2 * w);


        //
        auto i = static_cast<int>(floor(p.x()));
        auto j = static_cast<int>(floor(p.y()));
        auto k = static_cast<int>(floor(p.z()));
        double c[2][2][2];

        for (int di = 0; di < 2; ++di) {
            for (int dj = 0; dj < 2; ++dj) {
                for (int dk = 0; dk < 2; ++dk) {
                    c[di][dj][dk] = randfloat[
                            perm_x[(i + di) & 255] ^
                            perm_y[(j + dj) & 255] ^
                            perm_z[(k + dk) & 255]
                    ];
                }
            }
        }
        return trilinear_interp(c, u, v, w);
    }

    static double trilinear_interp(double c[2][2][2], double u, double v, double w) {
        auto accum = 0.0;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 2; ++k) {
                    accum += (i * u + (1 - i) * (1 - u)) *
                            (j * v + (1 - j) * (1 - v)) *
                            (k * w + (1 - k) * (1 - w)) * c[i][j][k];
                }
            }
        }
        return accum;
    }


    static int *perlin_generate_perm() {
        auto p = new int[point_count];
        for (int i = 0; i < point_count; ++i) {
            p[i] = i;
        }
        permute(p, point_count);
        return p;
    }

    static void permute(int *p, int n) {
        for (int i = n - 1; i > 0; i--) {
            int target = random_int(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }


public:
    static const int point_count = 256;
    double *randfloat;
    int *perm_x, *perm_y, *perm_z;
};

#endif //RAY_TRACER_PERLIN_H
