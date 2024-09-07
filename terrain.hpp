#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include <array>
#include <vector>
#include <random>

#include "common.hpp"

class Terrain {
    int size;
    std::vector<std::vector<double>> heightmap;

public:
    Terrain(int size) {
        this->size = size;

        // Initialize a flat terrain
        heightmap = std::vector<std::vector<double>>(size);
        for (int i = 0; i < size; ++i) {
            heightmap[i] = std::vector<double>(size);
            for (int j = 0; j < size; ++j) {
                heightmap[i][j] = 0.0;
            }
        }
    }

    void flatTerrain(double h = 0.0);
    void midpointDisplacement();

    double getHeight(int i, int j);
    void setHeight(int i, int j, double height);
    void addHeight(int i, int j, double h);
    void addHeightRadius(int i, int j, double h, int k1, int k2);

    int getSize();
private:
    void normalize();
    void midpointCenter(int x, int y, int r, double spread);
};

#endif