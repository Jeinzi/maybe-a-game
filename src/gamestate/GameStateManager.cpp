#include "GameStateManager.hpp"


GameStateManager::GameStateManager()
  : player(5*160, 4*160) {
  states.reserve(2);
  states.push_back(new MenuState(*this, player));

  changeState(State::Menu);
}


GameStateManager::~GameStateManager() {

}

void GameStateManager::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(*this->states[(int)currentState], states);
}


void GameStateManager::update(sf::Time time) {
  states[(int)currentState]->update(time);
}


void GameStateManager::changeState(State state) {
  currentState = state;
  states[(int)currentState]->init();
}


void GameStateManager::keyPressed(sf::Event event) {
  states[(int)currentState]->keyPressed(event);
}


void GameStateManager::mouseButtonPressed(sf::Event event, sf::Vector2f worldCoordinates) {
  states[(int)currentState]->mouseButtonPressed(event, worldCoordinates);
}


void GameStateManager::mouseWheelMoved(sf::Event event) {
  states[(int)currentState]->mouseWheelMoved(event);
}
