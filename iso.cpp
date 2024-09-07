#include <SFML/Graphics.hpp>
#include <memory>

#include "terrain.hpp"
#include "iso.hpp"
#include "common.hpp"

void IsometricRenderer::computeTerrainSprite(int i, int j) {
    sprites[i][j].setOutlineColor(sf::Color::Black);
    sprites[i][j].setOutlineThickness(1.0);
    sprites[i][j].setFillColor(colorFromHeight(terrain.get()->getHeight(i, j)));
    // computes the 4 points
    sprites[i][j].setPoint(0, getIsometricTerrainPoint(i, j));
    sprites[i][j].setPoint(1, getIsometricTerrainPoint(i+1, j));
    sprites[i][j].setPoint(2, getIsometricTerrainPoint(i+1, j+1));
    sprites[i][j].setPoint(3, getIsometricTerrainPoint(i, j+1));
}

sf::Vector2f IsometricRenderer::getIsometricTerrainPoint(int i, int j) {
    double height = this->terrain.get()->getHeight(i, j);

    return getIsometricPoint(i, j, height);
}

sf::Vector2i IsometricRenderer::getCursorPosition() {
    return cursorPosition;
}

sf::Vector2f IsometricRenderer::getIsometricPoint(double x, double y, double z) {
    return sf::Vector2f(origin.x + e0.x * scale * x + e1.x * scale * y + e2.x * scale * z, origin.y + e0.y * scale * x + e1.y * scale * y + e2.y * scale * z);
}

sf::Vector2f IsometricRenderer::guessIsometricPosition(double x, double y) {
    /*
    sf::Vector2f firstGuess = convertToIsometric(x, y, h);
    sf::Vector2f guess = firstGuess;

    if (0 <= guess.x && guess.x + 1 < size && 0 <= guess.y && guess.y + 1 < size) {
        h = terrain.get()->getHeight((int) guess.x, (int) guess.y);
        double old_h = h;
        int n = 0;
        int max = 32;

        do {
            guess = convertToIsometric(x, y, h);
            n++;
        } while (n < max);
    }

    return guess;
    */

    double distance = 1000.0;
    sf::Vector2f guess;

    for (double h = -10.0; h <= 10.0; h += 0.5) {
        sf::Vector2f newGuess = convertToIsometric(x, y, h);
        if (0 <= newGuess.x && newGuess.x + 1 < size && 0 <= newGuess.y && newGuess.y + 1 < size) {
            double h2 = terrain.get()->getHeight((int) newGuess.x, (int) newGuess.y);

            if (std::abs(h - h2) < distance) {
                guess = newGuess;
                distance = std::abs(h - h2);
            }
        }
    }

    return guess;
}

sf::Vector2f IsometricRenderer::convertToIsometric(double x, double y, double h) {
    double x2 = (x - origin.x - h * scale * e2.x);
    double y2 = (y - origin.y - h * scale * e2.y);

    double det = (scale * scale) * (e1.y * e0.x - e0.y * e1.x);

    double x3 = 1.0 / det * (e1.y * scale * x2 - e1.x * scale * y2);
    double y3 = 1.0 / det * (-e0.y * scale * x2 + e0.x * scale * y2);

    return sf::Vector2f(x3, y3);
}

void IsometricRenderer::render() {
    this->clear(sf::Color::White);

    for (int i = 0; i < size-1; ++i) {
        for (int j = 0; j < size-1; ++j) {
            computeTerrainSprite(i, j);
            this->draw(sprites[i][j]);
        }
    }

    int i = cursorPosition.x;
    int j = cursorPosition.y;

    if (0 <= i && i + 1 < size && 0 <= j && j + 1 < size) {
        spriteCursor.setOutlineColor(sf::Color::Black);
        spriteCursor.setOutlineThickness(1.0);
        spriteCursor.setFillColor(sf::Color::Red);
        // computes the 4 points

        spriteCursor.setPoint(0, getIsometricTerrainPoint(i, j));
        spriteCursor.setPoint(1, getIsometricTerrainPoint(i+1, j));
        spriteCursor.setPoint(2, getIsometricTerrainPoint(i+1, j+1));
        spriteCursor.setPoint(3, getIsometricTerrainPoint(i, j+1));

        this->draw(spriteCursor);
    }
}

void IsometricRenderer::translateView(double x, double y, double z) {
    this->center.x += e0.x * scale * x + e1.x * scale * y + e2.x * scale * z;
    this->center.y += e0.y * scale * x + e1.y * scale * y + e2.y * scale * z;

    this->origin.x = this->center.x + e0.x * scale * (-size / 2) + e1.x * scale * (-size/2);
    this->origin.y = this->center.y + e0.y * scale * (-size / 2) + e1.y * scale * (-size/2);
}

void IsometricRenderer::zoomView(double t) {
    scale *= t;

    this->origin.x = this->center.x + e0.x * scale * (-size / 2) + e1.x * scale * (-size/2);
    this->origin.y = this->center.y + e0.y * scale * (-size / 2) + e1.y * scale * (-size/2);
}

void IsometricRenderer::moveCursor(float cursorX, float cursorY) {
    sf::Vector2f position = guessIsometricPosition(cursorX, cursorY);

    cursorPosition.x = (int) position.x;
    cursorPosition.y = (int) position.y;
}