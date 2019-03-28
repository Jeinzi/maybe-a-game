#ifndef _ANIMATION_HPP
#define _ANIMATION_HPP

#include <vector>
#include <SFML/Graphics.hpp>

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

#include "Resources.hpp"


class Animation : public sf::Drawable {
  public:
    Animation(std::string const& animationName,
              std::vector<unsigned int> delaysMs);
    void update(sf::Time time);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setPosition(sf::Vector2f const& pos);
    void setHeight(float height);
    void flipHorizontal(bool flipped);
    sf::Vector2f getSize() const;

    Animation(Animation const&) = delete;
    Animation(Animation&&) = delete;
    void operator=(Animation const&) = delete;
    void operator=(Animation&&) = delete;

  private:
    struct Frame {
      unsigned int delayMs;
      sf::IntRect textureRectangle;
    };

    unsigned short currentFrame;
    unsigned int timeSinceFrameChangeMs;
    sf::Vector2f pos;
    sf::Vector2u size;
    sf::Sprite currentSprite;
    sf::Texture const& spriteSet;
    std::vector<Frame> frames;
};


#endif
