#include "MenuState.hpp"


MenuState::MenuState(GameStateManager& gsm, Player& player)
  : GameState(gsm), zoom(1), player(player),
    tileMap(Resources::getTileSet("tileset"), "map") {

  for (int i = 0; i < 10; ++i) {
    addCloud(false);
  }
}

MenuState::~MenuState() {}


void MenuState::init() {
  player.setTileMap(tileMap);
}


void MenuState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  auto view = target.getView();
  view.zoom(zoom);
  zoom = 1;
  target.setView(view);

  target.clear(sf::Color(249, 173, 186, 1));
  for (auto const& cloud: clouds) {
    target.draw(cloud, states);
  }
  target.draw(tileMap, states);
  target.draw(player, states);
}


void MenuState::update(sf::Time time) {
  auto it = clouds.begin();
  while (it != clouds.end()) {
    it->update(time);
    if (!it->isVisible()) {
      clouds.erase(it++);
      addCloud(true);
    }
    else {
      ++it;
    }
  }

  player.update(time);
}


void MenuState::keyPressed(sf::Event event) {
  player.keyPressed(event);
}


void MenuState::mouseButtonPressed(sf::Event event, sf::Vector2f worldCoordinates) {
  player.mouseButtonPressed(event, worldCoordinates);
}


void MenuState::addCloud(bool createAtMaxX) {
  auto scale = tileMap.getCoordinateScale();
  unsigned short maxDepth = 5 * scale;
  auto mapSize = tileMap.getSize();
  float x;
  if (createAtMaxX) {
    x = mapSize.x - 1;
  }
  else {
    x = rand() % mapSize.x;
  }
  sf::Vector2f pos = { x, static_cast<float>(rand() % maxDepth) };
  sf::Vector2f v = { -maxDepth / pos.y * 0.1f * scale, 0 };
  clouds.emplace_back(pos, v, tileMap);
}
