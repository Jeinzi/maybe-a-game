#ifndef _MENUSTATE_HPP
#define _MENUSTATE_HPP

#include <list>
#include <cstdlib>

#include "Cloud.hpp"
#include "Player.hpp"
#include "TileMap.hpp"
#include "GameState.hpp"
#include "GameStateManager.hpp"

#include <iostream>

class MenuState : public GameState {
  public:
    MenuState(GameStateManager& gsm, Player& player);
    ~MenuState();

    void init() override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void update(sf::Time time) override;
    void keyPressed(sf::Event event) override;
    void mouseButtonPressed(sf::Event event, sf::Vector2f worldCoordinates) override;

  private:
    mutable float zoom;
    Player& player;
    TileMap tileMap;
    std::list<Cloud> clouds;

    void addCloud(bool createAtMaxX);
};


#endif
