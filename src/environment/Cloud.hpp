#ifndef _CLOUD_HPP
#define _CLOUD_HPP

#include <SFML/Graphics.hpp>

#include "TileMap.hpp"
#include "Resources.hpp"


class Cloud : public sf::Drawable {
  public:
    Cloud(sf::Vector2f const& pos, sf::Vector2f const& v, TileMap const& tileMap);
    void update(sf::Time time);
    bool isVisible() const;

  private:
    sf::Vector2f v;
    sf::Vector2f pos;
    sf::Sprite sprite;
    TileMap const& tileMap;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};


#endif
