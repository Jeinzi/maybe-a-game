#ifndef _TILEMAP_HPP
#define _TILEMAP_HPP

#include <string>
#include <cstdio>
#include <vector>
#include <cmath>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

#include "Item.hpp"
#include "TileSet.hpp"
#include "Resources.hpp"


class TileMap : public sf::Drawable {
  public:
    TileMap(TileSet const& tileSet, std::string const& tileMapId);
    ~TileMap();
    bool isSolid(unsigned int x, unsigned int y) const;
    unsigned short getCoordinateScale() const;
    int getTileId(unsigned int x, unsigned int y) const;
    sf::Vector2u getSize() const;
    std::vector<Item> hit(unsigned int x, unsigned int y);
    TileSet const& getTileSet() const;
    void setLight(unsigned int x, unsigned int y);

  private:
    sf::Vector2u lightPosition;
    unsigned short coordinateScale;
    int** map;
    unsigned int nDrawableTiles;
    sf::Vector2u size;
    sf::Sound sound;
    TileSet const& tileSet;
    sf::VertexArray vertices;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void updateVertices();
};

#endif
