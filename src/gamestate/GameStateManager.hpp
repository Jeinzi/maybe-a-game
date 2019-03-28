#ifndef _GAMESTATEMANAGER_HPP
#define _GAMESTATEMANAGER_HPP

#include <array>
#include "SFML/Graphics.hpp"

#include "TileSet.hpp"
#include "GameState.hpp"
#include "MenuState.hpp"

class GameState;


class GameStateManager : public sf::Drawable {
  public:
    enum class State {
      Menu,
      Level
    };

    GameStateManager();
    ~GameStateManager();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void update(sf::Time time);
    void changeState(State state);
    void keyPressed(sf::Event event);
    void mouseButtonPressed(sf::Event event, sf::Vector2f worldCoordinates);
    void mouseWheelMoved(sf::Event event);

  private:
    State currentState;
    Player player;
    std::vector<GameState*> states;
};

#endif

