#ifndef ISOMETRIC_RENDERER_HPP
#define ISOMETRIC_RENDERER_HPP

#include <SFML/Graphics.hpp>
#include <memory>

#include "terrain.hpp"
#include "common.hpp"

class IsometricRenderer : public sf::RenderTexture {
    std::shared_ptr<Terrain> terrain;
    std::vector<std::vector<sf::ConvexShape>> sprites;
    sf::ConvexShape spriteCursor;

    sf::Vector2f e0;
    sf::Vector2f e1;
    sf::Vector2f e2;

    sf::Vector2f origin;
    sf::Vector2f center;

    sf::Vector2i cursorPosition;

    float width;
    float height;

    float scale = 0.5;

    int size;

    public:
    IsometricRenderer(std::shared_ptr<Terrain> terrain, float width, float height) {
        this->terrain = terrain;
        this->size = this->terrain.get()->getSize();

        this->width = width;
        this->height = height;

        this->create(width, height);

        this->center = sf::Vector2f(width / 2, height / 2);

        this->scale = 0.5;

        this->e0 = sf::Vector2f(15.0f, 9.0f);
        this->e1 = sf::Vector2f(-15.0f, 9.0f);
        this->e2 = sf::Vector2f(0.0f, -9.0f);

        this->origin.x = this->center.x + e0.x * scale * (-size / 2) + e1.x * scale * (-size/2);
        this->origin.y = this->center.y + e0.y * scale * (-size / 2) + e1.y * scale * (-size/2);

        sprites = std::vector<std::vector<sf::ConvexShape>>(size-1);
        for (int i = 0; i < size-1; ++i) {
            sprites[i] = std::vector<sf::ConvexShape>(size-1);
            for (int j = 0; j < size-1; ++j) {
                sprites[i][j] = sf::ConvexShape(4);
            }
        }

        spriteCursor = sf::ConvexShape(4);
        spriteCursor.setFillColor(sf::Color::Red);
    }

    void computeTerrainSprite(int i, int j);
    sf::Vector2f getIsometricTerrainPoint(int i, int j);
    sf::Vector2f getIsometricPoint(double x, double y, double z);

    sf::Vector2f convertToIsometric(double x, double y, double h = 0.0);
    sf::Vector2i getCursorPosition();
    sf::Vector2f guessIsometricPosition(double x, double y);

    void render();
    void translateView(double x, double y, double z);
    void zoomView(double t);
    void moveCursor(float cursorX, float cursorY);
};

#endif