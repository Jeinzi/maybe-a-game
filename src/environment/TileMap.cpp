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

  map = new int*[size.x];
  for (unsigned int x = 0; x < size.x; ++x) {
    map[x] = new int[size.y];
  }

  auto tiles = doc["layers"].GetArray()[0].GetObject()["tiles"].GetArray();
  for (rapidjson::SizeType i = 0; i < tiles.Size(); ++i) {
    auto object = tiles[i].GetObject();
    int tileId = object["tile"].GetInt();
    unsigned int x = object["x"].GetUint();
    unsigned int y = object["y"].GetUint();
    map[x][y] = tileId >= 0 ? tileId : 0;
    if (tileId > 0) {
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


int TileMap::getTileId(unsigned int x, unsigned int y) const {
  // TODO: check bounds.
  x /= coordinateScale;
  y /= coordinateScale;
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
    // Grass.
    map[x][y] = 0;
    --nDrawableTiles;
    drops.emplace_back("Seeds", 2, 0.01);
    sound.setBuffer(Resources::getSoundBuffer("grass_rustling"));
    sound.play();
  }
  else if (map[x][y] == 10) {
    // Coal.
    map[x][y] = 0;
    --nDrawableTiles;
    drops.emplace_back("Coal", 100, 1);
  }
  else if (isSolid(x * coordinateScale, y * coordinateScale)) {
    if (true || map[x][y] == 11) { //TODO
      if (x >= 1 && map[x-1][y] == 11) {
        map[x-1][y] = 12;
      }
      if (x < size.x - 1 && map[x+1][y] == 11) {
        map[x+1][y] = 13;
      }
      if (y > 0 && map[x][y-1] == 11) {
        map[x][y-1] = 15;
      }
    }
    map[x][y] = 0;
    --nDrawableTiles;
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


void TileMap::setLight(unsigned int x, unsigned int y) {
  lightPosition = sf::Vector2u(x, y);
  updateVertices(); // TODO: This is not optimized at all.
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

      unsigned short gray = 0 * 255;
      sf::Color dark(gray, gray, gray, 255);
      sf::Color light(255, 255, 255, 255);
      sf::Color const unmodifiedLight = light;
      quad[0].color = dark;
      quad[1].color = dark;
      quad[2].color = dark;
      quad[3].color = dark;


      // Calculate player light.
      unsigned int lightRadius = 3 * coordinateScale;
      unsigned int distanceFromLight = std::sqrt(pow((int)lightPosition.x - (int)x * coordinateScale, 2) +
                                                pow((int)lightPosition.y - (int)y * coordinateScale, 2)); // TODO this will break if the unsigned values are bigger than the signed int limit.

      // No light in the depth.
      int const unmodifiedDepth = 34; //TODO should not be defined here AND in MenuState.cpp
      int const blackDepth = 44; //TODO should not be defined here AND in MenuState.cpp
      if (y > unmodifiedDepth) {
        light.r -= y > blackDepth ? light.r : (float)light.r / (blackDepth - unmodifiedDepth) * (y - unmodifiedDepth);
        light.g -= y > blackDepth ? light.g : (float)light.g / (blackDepth - unmodifiedDepth) * (y - unmodifiedDepth);
        light.b -= y > blackDepth ? light.b : (float)light.b / (blackDepth - unmodifiedDepth) * (y - unmodifiedDepth);

        if (distanceFromLight < lightRadius) {
          light.r = unmodifiedLight.r - (float)(unmodifiedLight.r - light.r) / lightRadius * distanceFromLight;
          light.g = unmodifiedLight.g - (float)(unmodifiedLight.g - light.g) / lightRadius * distanceFromLight;
          light.b = unmodifiedLight.b - (float)(unmodifiedLight.b - light.b) / lightRadius * distanceFromLight;
        }
      }

      // As x (and y) are unsigned, x - 1 underflows when x is 0
      // and therefore x - 1 < size.x checks the lower bound.
      if (!tileTypes[map[x][y]].isSolid) {
        if (y - 1 < size.y) {
          if (!tileTypes[map[x][y-1]].isSolid) {
            quad[0].color = light;
            quad[1].color = light;
            quad[2].color = light;
            quad[3].color = light;
            ++i;
            continue;
          }
        }
        else {
          quad[0].color = light;
          quad[1].color = light;
          quad[2].color = light;
          quad[3].color = light;
          ++i;
          continue;
        }
      }

      if (x - 1 < size.x) {
        if (!tileTypes[map[x-1][y]].isSolid) {
          quad[0].color = light;
          quad[1].color = light;
        }
        else {
          if (y-1 < size.y && !tileTypes[map[x-1][y-1]].isSolid) {
            quad[0].color = light;
          }
          if (y+1 < size.y && !tileTypes[map[x-1][y+1]].isSolid) {
            quad[1].color = light;
          }
        }
      }

      if (y - 1 < size.y) {
        if (!tileTypes[map[x][y-1]].isSolid) {
          quad[0].color = light;
          quad[3].color = light;
        }
        else {
          if (x+1 < size.x && !tileTypes[map[x+1][y-1]].isSolid) {
            quad[3].color = light;
          }
        }
      }

      if (x + 1 < size.x) {
        if (!tileTypes[map[x+1][y]].isSolid) {
          quad[2].color = light;
          quad[3].color = light;
        }
        else {
          if (y+1 < size.y && !tileTypes[map[x+1][y+1]].isSolid) {
            quad[2].color = light;
          }
        }
      }

      if (y + 1 < size.y) {
        if (!tileTypes[map[x][y+1]].isSolid) {
          quad[1].color = light;
          quad[2].color = light;
        }
      }



      ++i;
    }
  }
}
