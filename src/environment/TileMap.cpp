#include "TileMap.hpp"


TileMap::TileMap(TileSet const& tileSet, std::string const& tileMapId)
  : coordinateScale(160), nDrawableTiles(0), tileSet(tileSet) {

  char readBuffer[65536];
  FILE* mapFile = fopen(Resources::getPath(tileMapId).c_str(), "r");
  rapidjson::FileReadStream mapStream(mapFile, readBuffer, sizeof(readBuffer));

  rapidjson::Document doc;
  doc.ParseStream(mapStream);
  fclose(mapFile);

  if (doc.HasParseError()) {
    throw; // @todo
  }


  size.x = doc["tileswide"].GetUint();
  size.y = doc["tileshigh"].GetUint();
  vertices.setPrimitiveType(sf::Quads);

  map = new unsigned int*[size.x];
  for (unsigned int x = 0; x < size.x; ++x) {
    map[x] = new unsigned int[size.y];
  }

  auto tiles = doc["layers"].GetArray()[0].GetObject()["tiles"].GetArray();
  for (rapidjson::SizeType i = 0; i < tiles.Size(); ++i) {
    auto object = tiles[i].GetObject();
    unsigned int tileId = object["tile"].GetUint();
    unsigned int x = object["x"].GetUint();
    unsigned int y = object["y"].GetUint();
    map[x][y] = tileId;
    if (tileId != 0) {
      ++nDrawableTiles;
    }
  }
  updateVertices();
}


TileMap::~TileMap() {
  for (unsigned int x = 0; x < size.x; ++x) {
    delete map[x];
  }
  delete map;
}


bool TileMap::isSolid(unsigned int x, unsigned int y) const {
  x /= coordinateScale;
  y /= coordinateScale;
  return tileSet.getTiles()[map[x][y]].isSolid;
}


unsigned short TileMap::getCoordinateScale() const {
  return coordinateScale;
}


unsigned int TileMap::getTileId(unsigned int x, unsigned int y) const {
  return map[x][y];
}


sf::Vector2u TileMap::getSize() const {
  return size * (unsigned int)coordinateScale;
}


std::vector<Item> TileMap::hit(unsigned int x, unsigned int y) {
  x /= coordinateScale;
  y /= coordinateScale;

  std::vector<Item> drops;
  if (map[x][y] == 3) {
    map[x][y] = 0;
    ++nDrawableTiles;
    drops.emplace_back("Seeds", 2, 0.01);
    sound.setBuffer(Resources::getSoundBuffer("grass_rustling"));
    sound.play();
  }
  updateVertices();
  return drops;
}


TileSet const& TileMap::getTileSet() const {
  return tileSet;
}


void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  auto const& texture = Resources::getTexture("tileset");
  states.texture = &texture;
  target.draw(vertices, states);
}


void TileMap::updateVertices() {
  unsigned int i = 0;
  vertices.resize(nDrawableTiles * 4);
  for (unsigned int x = 0; x < size.x; ++x) {
    for (unsigned int y = 0; y < size.y; ++y) {
      if (map[x][y] == 0) {
        continue;
      }
      unsigned int sx = x * coordinateScale;
      unsigned int sy = y * coordinateScale;

      auto const& tileTypes = tileSet.getTiles();
      unsigned int tileSize = tileSet.getTileSize();
      unsigned int u = tileTypes[map[x][y]].textureCoordinates.x;
      unsigned int v = tileTypes[map[x][y]].textureCoordinates.y;

      sf::Vertex* quad = &vertices[i * 4];
      quad[0].position = sf::Vector2f(sx, sy);
      quad[1].position = sf::Vector2f(sx, sy + coordinateScale);
      quad[2].position = sf::Vector2f(sx + coordinateScale, sy + coordinateScale);
      quad[3].position = sf::Vector2f(sx + coordinateScale, sy);

      quad[0].texCoords = sf::Vector2f(u, v);
      quad[1].texCoords = sf::Vector2f(u, v + tileSize);
      quad[2].texCoords = sf::Vector2f(u + tileSize, v + tileSize);
      quad[3].texCoords = sf::Vector2f(u + tileSize, v);
      ++i;
    }
  }
}
