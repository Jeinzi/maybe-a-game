#include "Inventory.hpp"


Inventory::Inventory(float weightLimitGrams, float volumeLimitLiters)
  : weightGrams(0), volumeLiters(0),
    weightLimitGrams(weightLimitGrams), volumeLimitLiters(volumeLimitLiters),
    background({150, 200}) {

  background.setPosition(10, 10);
  background.setOutlineThickness(3);
  background.setFillColor(sf::Color::White);
  background.setOutlineColor(sf::Color::Black);

  auto& font = Resources::getFont("8bit");
  caption.setFont(font);
  caption.setString("Inventory");
  caption.setStyle(sf::Text::Underlined);
  caption.setFillColor(sf::Color::Black);
  auto bounds = caption.getLocalBounds();
  auto charSize = caption.getCharacterSize();
  unsigned int newCharSize = (150.f / bounds.width) * (float)charSize * 0.9;
  caption.setCharacterSize(newCharSize);
  sf::Vector2f position((int)(10 + 0.05 * 150), (int)(10 + 0.05 * 150));
  caption.setPosition(position);

  content.setFont(font);
  content.setFillColor(sf::Color::Black);
  content.setCharacterSize(newCharSize - 3);
  position.y += newCharSize + 10;
  content.setPosition(position);
  content.setLineSpacing(1.5);
}


void Inventory::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  auto view = target.getView();
  target.setView(target.getDefaultView());
  target.draw(background, states);
  target.draw(caption, states);
  target.draw(content, states);
  target.setView(view);
}


void Inventory::update() {
  std::string contentText;
  for (auto const& item: items) {
    contentText.append("-").append(item.getName()).append("\n");
  }
  content.setString(contentText);
}


bool Inventory::store(Item&& item) {
  float newWeightGrams = weightGrams + item.getWeightGrams();
  float newVolumeLiters = volumeLiters + item.getVolumeLiters();

  if (newWeightGrams > weightLimitGrams || newVolumeLiters > volumeLimitLiters) {
    return false;
  }

  items.push_back(item);
  weightGrams = newWeightGrams;
  volumeLiters = newVolumeLiters;
  update();
  return true;
}