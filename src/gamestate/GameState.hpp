#ifndef _GAMESTATE_HPP
#define _GAMESTATE_HPP

#include <SFML/Graphics.hpp>

#include "GameStateManager.hpp"

class GameStateManager;


class GameState : public sf::Drawable {
  public:
    GameState(GameStateManager& gsm) : gsm(gsm) {};

    virtual void init() = 0;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
    virtual void update(sf::Time time) = 0;
    virtual void keyPressed(sf::Event event) = 0;
    virtual void mouseButtonPressed(sf::Event event, sf::Vector2f worldCoordinates) = 0;
    virtual void mouseWheelMoved(sf::Event) {}

    GameState(GameState const&) = delete;
    GameState(GameState const&&) = delete;
    void operator=(GameState&) = delete;
    void operator=(GameState&&) = delete;

  protected:
    GameStateManager& gsm;
};


#endif
