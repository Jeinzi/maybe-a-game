#include <ctime>
#include <cstdlib>
#include <SFML/Graphics.hpp>

#include "Player.hpp"
#include "Resources.hpp"
#include "GameStateManager.hpp"

#include <iostream>



int main() {
  srand(time(NULL));
  auto screen = sf::VideoMode::getDesktopMode();
  auto videoMode = sf::VideoMode(screen.width, screen.height);
  sf::RenderWindow window(videoMode, "Flower Power", sf::Style::Fullscreen);
  window.setVerticalSyncEnabled(true);
  window.setActive(false);
  //window.setMouseCursorVisible(false);

  Resources::load();
  sf::Vector2f size(screen.width / 16.f * 25, screen.height / 16.f * 25);
  sf::Vector2f center(8*160, 4*160);
  sf::View view(center, size);
  window.setView(view);
  GameStateManager gsm;

  sf::Clock clock;
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      else if (event.type == sf::Event::KeyPressed) {
        gsm.keyPressed(event);
      }
      else if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i coordinates = { event.mouseButton.x, event.mouseButton.y };
        gsm.mouseButtonPressed(event, window.mapPixelToCoords(coordinates));
      }
      else if (event.type == sf::Event::MouseWheelScrolled) {
        gsm.mouseWheelMoved(event);
      }
    }
    gsm.update(clock.restart());
    window.draw(gsm);
    window.display();
  }

  return EXIT_SUCCESS;
}
