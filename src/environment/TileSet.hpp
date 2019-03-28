#ifndef _TILESET_HPP
#define _TILESET_HPP

#include <vector>
#include <SFML/Graphics.hpp>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

#include "Tile.hpp"
#include "Resources.hpp"


class TileSet {
  public:
    TileSet(std::string const& name);
    unsigned int getTileSize() const;
    sf::Texture const& getTexture() const;
    std::vector<Tile> const& getTiles() const;

  private:
    unsigned int size;
    std::vector<Tile> tiles;
    sf::Texture const& tileSet;
};

#endif
