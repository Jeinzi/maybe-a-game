#include "Resources.hpp"


bool Resources::isLoaded = false;
std::unordered_map<std::string, sf::Font> Resources::fonts;
std::unordered_map<std::string, TileSet> Resources::tileSets;
std::unordered_map<std::string, std::string> Resources::paths;
std::unordered_map<std::string, sf::Texture> Resources::textures;
std::unordered_map<std::string, sf::SoundBuffer> Resources::soundEffects;

void Resources::load() {
  isLoaded = true;

  // Fonts.
  fonts["8bit"].loadFromFile("res/font/PressStart2P-Regular.ttf");

  // Paths.
  paths["tileset_properties"] = "res/map/tileset.prop";
  paths["player_stand_idle_properties"] = "res/sprites/entities/player/stand_idle.json";
  paths["player_stand_walk_properties"] = "res/sprites/entities/player/stand_walk.json";
  paths["amann_stand_idle_properties"] = "res/sprites/entities/amann/idle.json";
  paths["amann_stand_walk_properties"] = "res/sprites/entities/amann/walk.json";
  paths["map"] = "res/map/level.map";

  // Textures.
  textures["tileset"].loadFromFile("res/map/tileset.png");
  textures["player_stand_idle"].loadFromFile("res/sprites/entities/player/stand_idle.png");
  textures["player_stand_walk"].loadFromFile("res/sprites/entities/player/stand_walk.png");
  textures["amann_stand_idle"].loadFromFile("res/sprites/entities/amann/idle.png");
  textures["amann_stand_walk"].loadFromFile("res/sprites/entities/amann/walk.png");
  textures["cloud0"].loadFromFile("res/sprites/clouds/cloud0.png");

  // TileSets
  tileSets.emplace("tileset", "tileset");

  // Sound effects.
  soundEffects["jump0"].loadFromFile("res/sfx/jump0.ogg");
  soundEffects["jump1"].loadFromFile("res/sfx/jump1.ogg");
  soundEffects["jump2"].loadFromFile("res/sfx/jump2.ogg");
  soundEffects["jump3"].loadFromFile("res/sfx/jump3.ogg");
  soundEffects["jump4"].loadFromFile("res/sfx/jump4.ogg");
  soundEffects["grass_rustling"].loadFromFile("res/sfx/grass_rustling.ogg");
}


sf::Font const& Resources::getFont(std::string const& id) {
  if (!isLoaded) {
    load();
  }
  return fonts.at(id);
}


TileSet const& Resources::getTileSet(std::string const& id) {
  if (!isLoaded) {
    load();
  }
  return tileSets.at(id);
}


std::string Resources::getPath(std::string const& id) {
  if (!isLoaded) {
    load();
  }
  return paths.at(id);
}


sf::Texture const& Resources::getTexture(std::string const& id) {
  if (!isLoaded) {
    load();
  }
  return textures.at(id);
}


sf::SoundBuffer const& Resources::getSoundBuffer(std::string const& id) {
  if (!isLoaded) {
    load();
  }
  return soundEffects.at(id);
}
