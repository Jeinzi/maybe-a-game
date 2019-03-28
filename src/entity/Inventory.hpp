#ifndef _INVENTORY_HPP
#define _INVENTORY_HPP

#include <vector>
#include <SFML/Graphics.hpp>

#include "Item.hpp"
#include "Resources.hpp"
#include <iostream>


class Inventory : public sf::Drawable {
  public:
    Inventory(float weightLimit, float volumeLimit);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void update();
    bool store(Item&& item);

  private:
    float weightGrams;
    float volumeLiters;
    float weightLimitGrams;
    float volumeLimitLiters;
    sf::RectangleShape background;
    sf::Text caption;
    sf::Text content;
    std::vector<Item> items;
};

#endif
