#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include <time.h>

#include "terrain.hpp"
#include "iso.hpp"
#include "common.hpp"

int main()
{
    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(1800, 1000), "Terrain procedural generation");
    int size = 65;

    std::shared_ptr<Terrain> terrain = std::make_shared<Terrain>(size);
    terrain.get()->midpointDisplacement();

    std::vector<std::vector<sf::RectangleShape>> squareArray;
    // Initialize a flat terrain
    squareArray = std::vector<std::vector<sf::RectangleShape>>(size);
    for (int i = 0; i < size; ++i) {
        squareArray[i] = std::vector<sf::RectangleShape>(size);
        for (int j = 0; j < size; ++j) {
            squareArray[i][j] = sf::RectangleShape(sf::Vector2f(800.0f / size, 800.0f / size));
            squareArray[i][j].setPosition(i * (800.0f / size), j * (800.0f / size));
            squareArray[i][j].setFillColor(colorFromHeight(terrain.get()->getHeight(i, j)));
        }
    }
    IsometricRenderer renderer(terrain, 1000, 800);

    sf::Sprite rendererSprite(renderer.getTexture());
    rendererSprite.setPosition(sf::Vector2f(800.0f, 0.0f));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {    
            switch (event.type)
            {
                // window closed
                case sf::Event::Closed:
                    window.close();
                    break;

                // key pressed
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Left) {
                        renderer.translateView(1.0, -1.0, 0.0);
                    }
                    if (event.key.code == sf::Keyboard::Right) {
                        renderer.translateView(-1.0, 1.0, 0.0);
                    }
                    if (event.key.code == sf::Keyboard::Up) {
                        renderer.translateView(1.0, 1.0, 0.0);
                    }
                    if (event.key.code == sf::Keyboard::Down) {
                        renderer.translateView(-1.0, -1.0, 0.0);
                    }
                    if (event.key.code == sf::Keyboard::Add) {
                        renderer.zoomView(2.0);
                    }
                    if (event.key.code == sf::Keyboard::Subtract) {
                        renderer.zoomView(0.5);
                    }
                    break;
                case sf::Event::MouseMoved: {
                    sf::Vector2i windowedPosition = sf::Mouse::getPosition(window);

                    float x = windowedPosition.x - rendererSprite.getPosition().x;
                    float y = windowedPosition.y - rendererSprite.getPosition().y;

                    if (0 <= x && x <= 1000.0f && 0 <= y && y <= 800.0f) {
                        renderer.moveCursor(x, y);
                        
                        sf::Vector2i cursorPosition = renderer.getCursorPosition();
                        std::string s = std::string("x:") + std::to_string(cursorPosition.x) + std::string(", y:") + std::to_string(cursorPosition.y);
                        window.setTitle(s);
                    }
                }
                    break;
                case sf::Event::MouseButtonPressed: {
                    if (event.mouseButton.button == sf::Mouse::Button::Left) {
                        // increase height
                        int x = renderer.getCursorPosition().x;
                        int y = renderer.getCursorPosition().y;

                        if (0 <= x && x+1 <= terrain.get()->getSize() && 0 <= y && y+1 <= terrain.get()->getSize()) {
                            terrain.get()->addHeightRadius(x, y, 1.0, -3, 5);
                        }
                    }
                    else if (event.mouseButton.button == sf::Mouse::Button::Right) {
                        // increase height
                        int x = renderer.getCursorPosition().x;
                        int y = renderer.getCursorPosition().y;

                        if (0 <= x && x+1 <= terrain.get()->getSize() && 0 <= y && y+1 <= terrain.get()->getSize()) {
                            terrain.get()->addHeightRadius(x, y, -1.0, -3, 5);
                        }
                    }
                }
                    break;
                default:
                    break;
            }
        }

        renderer.render();
        renderer.display();

        window.clear();
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                window.draw(squareArray[i][j]);
            }
        }
        window.draw(rendererSprite);
        window.display();
    }

    return 0;
}

