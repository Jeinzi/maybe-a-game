#include "TileSet.hpp"
#include <iostream>

TileSet::TileSet(std::string const& name)
  : tileSet(Resources::getTexture(name)) {

  char readBuffer[65536];
  std::string propertyPath = Resources::getPath(name + "_properties");
  FILE* propertyFile = fopen(propertyPath.c_str(), "r");
  rapidjson::FileReadStream propertyStream(propertyFile, readBuffer, sizeof(readBuffer));

  rapidjson::Document doc;
  doc.ParseStream(propertyStream);
  fclose(propertyFile);

  if (doc.HasParseError()) {
    throw; // @todo
  }

  size = doc["size"].GetUint();
  unsigned int tilesPerRow = tileSet.getSize().x / size;

  auto tileArray = doc["tiles"].GetArray();
  for (rapidjson::SizeType i = 0; i < tileArray.Size(); ++i) {
    auto tile = tileArray[i].GetObject();
    unsigned int u = (i % tilesPerRow) * size;
    unsigned int v = (i / tilesPerRow) * size;
    tiles.push_back({
      tile["solid"].GetBool(),
      sf::Vector2f(u, v),
      tile["name"].GetString()
    });
  }
}


unsigned int TileSet::getTileSize() const {
  return size;
}

sf::Texture const& TileSet::getTexture() const {
  return tileSet;
}

std::vector<Tile> const& TileSet::getTiles() const {
  return tiles;
}