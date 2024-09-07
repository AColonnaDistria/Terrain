#include <SFML/Graphics.hpp>

sf::Color colorFromHeight(double height) {
    if (height < -10.0)
        return sf::Color(0, 0, 255);

    if (height < 0.0)
        return sf::Color(0, 0, -height*0.1 * 255);

    if (height > 10.0)
        return sf::Color(0, 255, 0);
    
    return sf::Color(0, height*0.1 * 255, 0);
}

double randomDouble(double min, double max) {
    return ((double) (rand() % 1000) * 0.001) * (max - min) + min;
}
