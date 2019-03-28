#ifndef _PLAYER_HPP
#define _PLAYER_HPP

#include <cmath>
#include <cstdlib>
#include <functional>

#include "TileMap.hpp"
#include "Resources.hpp"
#include "Animation.hpp"
#include "Inventory.hpp"


class Player : public sf::Drawable {
  public:
    Player(float x, float y);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void update(sf::Time time);
    void keyPressed(sf::Event event);
    void mouseButtonPressed(sf::Event event, sf::Vector2f worldCoordinates);
    void setTileMap(TileMap& tileMap);

  protected:
    void jump(float speed);
    bool isBlocked(sf::Vector2f pos) const;

  private:
    bool isFacingRight;
    bool isInventoryVisible;
    float friction;
    sf::Vector2f v;
    sf::Vector2f pos;
    sf::Vector2f size;
    sf::Vector2f vMax;
    sf::Vector2f integerPos;
    Animation animationStandIdle;
    Animation animationStandWalk;
    std::reference_wrapper<Animation> currentAnimation;
    sf::Sound jumpSound;
    TileMap* tileMap;
    Inventory inventory;

    sf::Vector2f tr;
    sf::Vector2f br;
    sf::Vector2f bl;
    bool tlBlocked;
    bool trBlocked;
    bool brBlocked;
    bool blBlocked;

    void updateCorners();
    void hit(float x, float y);
};

#endif
