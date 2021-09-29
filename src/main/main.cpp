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
  sf::RenderWindow window(videoMode, "Maybe a Game", sf::Style::Fullscreen);
  window.setVerticalSyncEnabled(true);
  window.setActive(false);

  Resources::load();

  sf::Vector2f viewSize((float)screen.width / 16.f * 25.f, (float)screen.height / 16.f * 25.f);
  sf::Vector2f center(8*160, 4*160);
  sf::View view(center, viewSize);
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
      else if (event.type == sf::Event::Resized)
      {
        auto newWindowSize = window.getSize();
        auto oldViewSize = view.getSize();

        viewSize = sf::Vector2f((float)newWindowSize.x / 16.f * 25.f, (float)newWindowSize.y / 16.f * 25.f);
        view.setSize(viewSize);

        // This is tricky. To 
        view.zoom(std::round(oldViewSize.x / viewSize.x * 10) / 10);
        window.setView(view);
      }
    }
    gsm.update(clock.restart());
    window.draw(gsm);
    window.display();
  }

  return EXIT_SUCCESS;
}
