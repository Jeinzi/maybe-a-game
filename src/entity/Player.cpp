#include "Player.hpp"


Player::Player(float x, float y)
  : isFacingRight(false), isInventoryVisible(false), friction(640),
    v(0, 0), pos(x, y), vMax(2*160, 3*160),
    animationStandIdle("amann_stand_idle", {0}),
    animationStandWalk("amann_stand_walk", {100}),
    currentAnimation(animationStandWalk),
    tileMap(nullptr), inventory(116, 10) {

  inventory.store(Item("Apple", 100, 0.1));
}


void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  auto view = target.getView();
  auto viewSize = view.getSize();
  auto mapSize = tileMap->getSize();

  sf::Vector2f viewPos = integerPos;
  viewPos.x += size.x / 2;
  viewPos.y += size.y / 2;


  if (viewPos.x - viewSize.x / 2 < 0) {
    viewPos.x = viewSize.x / 2;
  }
  if (viewPos.x + viewSize.x / 2 > mapSize.x) {
    viewPos.x = mapSize.x - viewSize.x / 2;
  }
  if (viewPos.y - viewSize.y / 2 < 0) {
    viewPos.y = viewSize.y / 2;
  }
  if (viewPos.y + viewSize.y / 2 > mapSize.y) {
    viewPos.y = mapSize.y - viewSize.y / 2;
  }

  view.setCenter(viewPos);
  target.setView(view);

  target.draw(currentAnimation, states);
  if (isInventoryVisible) {
    target.draw(inventory, states);
  }
}


void Player::update(sf::Time time) {
  size = currentAnimation.get().getSize();
  auto s = tileMap->getCoordinateScale();
  sf::Vector2f acc { 3 * (float)s, 6 * (float)s };

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    v.x -= acc.x * time.asSeconds();
    if (v.x < -vMax.x) {
      v.x = -vMax.x;
    }
    if (v.x < 1 * s) {
      isFacingRight = false;
    }
  }
  else if (v.x < 0) {
    v.x += friction * time.asSeconds();
    if (v.x > 0) {
      v.x = 0;
    }
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    v.x += acc.x * time.asSeconds();
    if (v.x > vMax.x) {
      v.x = vMax.x;
    }
    if (v.x > -1 * s) {
      isFacingRight = true;
    }
  }
  else if (v.x > 0) {
    v.x -= friction * time.asSeconds();
    if (v.x < 0) {
      v.x = 0;
    }
  }


  // Gravity.
  v.y += acc.y * time.asSeconds();

  // Vertical collision.
  pos.y += v.y * time.asSeconds();
  updateCorners();
  if (v.y > 0 && (blBlocked || brBlocked)) {
    v.y = 0;
    pos.y = ((int)(pos.y + size.y) / s) * s - size.y;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
      jump(7 * s);
    }
  }

  if (v.y < 0 && (tlBlocked || trBlocked)) {
    v.y = 0;
    pos.y = ((int)(pos.y + s) / s) * s;
  }

  // Horizontal collision.
  pos.x += v.x * time.asSeconds();
  updateCorners();
  if (v.x > 0 && (trBlocked || brBlocked)) {
    v.x = 0;
    pos.x = ((int)(pos.x + size.x) / s) * s - size.x;
  }

  if (v.x < 0 && (tlBlocked || blBlocked)) {
    v.x = 0;
    pos.x = ((int)(pos.x + s) / s) * s;
  }
  updateCorners();

  // Update animation.
  if (fabs(v.x) > 0) {
    currentAnimation = std::ref(animationStandWalk);
  }
  else {
    currentAnimation = std::ref(animationStandIdle);
  }

  currentAnimation.get().flipHorizontal(!isFacingRight);
  integerPos = { std::round(pos.x), std::round(pos.y) };
  currentAnimation.get().setPosition(integerPos);
  currentAnimation.get().update(time);
}


void Player::keyPressed(sf::Event event) {
  if (event.key.code == sf::Keyboard::I) {
    isInventoryVisible = !isInventoryVisible;
  }

  if (event.key.code == sf::Keyboard::X) {
    hit(pos.x + size.x / 2, pos.y + size.y - 0.01);
  }
}


void Player::mouseButtonPressed(sf::Event event, sf::Vector2f worldCoordinates) {
  if (event.mouseButton.button == sf::Mouse::Left) {
    hit(worldCoordinates.x, worldCoordinates.y);
  }
}


void Player::setTileMap(TileMap& tileMap) {
  this->tileMap = &tileMap;
  auto s = tileMap.getCoordinateScale();
  animationStandIdle.setHeight(1.2 * s);
  animationStandWalk.setHeight(1.2 * s);
}

void Player::jump(float speed) {
  v.y = -speed;
  short soundId = rand() % 5;
  std::string soundName = "jump" + std::to_string(soundId);
  jumpSound.setBuffer(Resources::getSoundBuffer(soundName));
  jumpSound.play();
}


bool Player::isBlocked(sf::Vector2f pos) const {
  auto mapSize = tileMap->getSize();
  if (pos.x < 0 || pos.y < 0 || pos.x > mapSize.x || pos.y > mapSize.y) {
    return true;
  }
  if (tileMap->isSolid(pos.x, pos.y) || tileMap->isSolid(pos.x, pos.y)) {
    return true;
  }
  return false;
}


void Player::updateCorners() {
  float delta = 0.01f;
  tr = { pos.x + size.x - delta, pos.y };
  br = { tr.x, pos.y + size.y - delta };
  bl = { pos.x, br.y };
  tlBlocked = isBlocked(pos);
  trBlocked = isBlocked(tr);
  brBlocked = isBlocked(br);
  blBlocked = isBlocked(bl);
}


void Player::hit(float x, float y) {
  auto items = tileMap->hit(x, y);
  for (auto& item: items) {
    inventory.store(std::move(item));
  }
}
