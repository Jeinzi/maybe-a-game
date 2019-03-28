#include "Cloud.hpp"


Cloud::Cloud(sf::Vector2f const& pos, sf::Vector2f const& v, TileMap const& tileMap)
  : v(v), pos(pos), sprite(Resources::getTexture("cloud0")), tileMap(tileMap) {
  auto scale = tileMap.getCoordinateScale() / sprite.getLocalBounds().height;
  sprite.setScale(scale, scale);
}


void Cloud::update(sf::Time time) {
  pos += v * time.asSeconds();
  sprite.setPosition(pos);
}


bool Cloud::isVisible() const {
  auto b = sprite.getGlobalBounds();
  auto mapSize = tileMap.getSize();
  if (b.left + b.width < 0) {
    return false;
  }
  if (b.left > mapSize.x) {
    return false;
  }
  if (b.top + b.height < 0) {
    return false;
  }
  if (b.top > mapSize.y) {
    return false;
  }
  return true;
}


void Cloud::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite, states);
}
