#ifndef _TILE_HPP
#define _TILE_HPP

#include <string>
#include <SFML/Graphics.hpp>


class Tile {
  public:
    bool isSolid;
    sf::Vector2f textureCoordinates;
    std::string name;
};

#endif
