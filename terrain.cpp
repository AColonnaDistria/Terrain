#include <array>
#include <vector>
#include <random>

#include "terrain.hpp"
#include "common.hpp"

void Terrain::flatTerrain(double h) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            heightmap[i][j] = h;
        }
    }
}

void Terrain::midpointDisplacement() {
    // sets the four corners at random values
    heightmap[0][0]           = randomDouble(-10.0, 10.0);
    heightmap[size-1][0]      = randomDouble(-10.0, 10.0);
    heightmap[0][size-1]      = randomDouble(-10.0, 10.0);
    heightmap[size-1][size-1] = randomDouble(-10.0, 10.0);

    /*
    // sets the midpoint edges
    heightmap[size/2-1][0]      = 0.5 * (heightmap[0][0] + heightmap[size-1][0]);
    heightmap[0][size/2-1]      = 0.5 * (heightmap[0][0] + heightmap[0][size-1]);
    heightmap[size/2-1][size-1] = 0.5 * (heightmap[0][size-1] + heightmap[size-1][size-1]);
    heightmap[size-1][size/2-1] = 0.5 * (heightmap[size-1][0] + heightmap[size-1][size-1]);

    // sets the center
    heightmap[size/2-1][size/2-1] = 0.25 * (heightmap[size/2-1][0] 
                                            + heightmap[0][size/2-1] 
                                            + heightmap[size/2-1][size-1] 
                                            + heightmap[size-1][size/2-1]);
    */

    // r = 2 is base case
    int r = size - 1;
    double spread = 3.0;
    while (r >= 2) {
        for (int x = 0; x < size - r; x += r) {
            for (int y = 0; y < size - r; y += r) {
                midpointCenter(x, y, r, spread);
            }
        }
        r /= 2;
        spread *= 0.5;
    }

    normalize();

    double biais = 2.5;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            heightmap[i][j] += biais;
        }
    }
}

void Terrain::midpointCenter(int x, int y, int r, double spread) {
    // sets the midpoint edges
    heightmap[x+r/2][y]   = randomDouble(-spread, spread) + 0.5 * (heightmap[x][y] + heightmap[x+r][y]);
    heightmap[x][y+r/2]   = randomDouble(-spread, spread) + 0.5 * (heightmap[x][y] + heightmap[x][y+r]);
    heightmap[x+r/2][y+r] = randomDouble(-spread, spread) + 0.5 * (heightmap[x][y+r] + heightmap[x+r][y+r]);
    heightmap[x+r][y+r/2] = randomDouble(-spread, spread) + 0.5 * (heightmap[x+r][y] + heightmap[x+r][y+r]);

    // sets the center
    heightmap[x+r/2][y+r/2] = randomDouble(-spread, spread) + 0.25 * (heightmap[x+r/2][y] 
                                                            + heightmap[x][y+r/2] 
                                                            + heightmap[x+r/2][y+r] 
                                                            + heightmap[x+r][y+r/2]);
}

void Terrain::normalize() {
    double max = -100000.0;
    double min = 100000.0;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (heightmap[i][j] > max) {
                max = heightmap[i][j];
            }
            if (heightmap[i][j] < min) {
                min = heightmap[i][j];
            }
        }
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            heightmap[i][j] = -9.0 + (heightmap[i][j] - min) / (max - min) * 18;
        }
    }
}

double Terrain::getHeight(int i, int j) {
    return heightmap[i][j];
}

void Terrain::setHeight(int i, int j, double height) {
    heightmap[i][j] = height;
}

void Terrain::addHeight(int i, int j, double h) {
    heightmap[i][j] += h;
}

void Terrain::addHeightRadius(int i, int j, double h, int k1, int k2) {
    for (int i2 = i + k1; i2 <= i + k2; ++i2) {
        for (int j2 = j + k1; j2 <= j + k2; ++j2) {
            if (0 <= i2 && i2 + 1 <= this->size && 0 <= j2 && j2 <= this->size) {
                double distance = (i2 - i) * (i2 - i) + (j2 - j) * (j2 - j);

                heightmap[i2][j2] += h * std::pow(0.9, distance);
            }
        }
    }
}

int Terrain::getSize() {
    return size;
}