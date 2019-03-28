#include "Item.hpp"


Item::Item(std::string const& name, float weightGrams, float volumeLiters)
  : weightGrams(weightGrams), volumeLiters(volumeLiters), name(name) {}


float Item::getWeightGrams() const {
  return weightGrams;
}

float Item::getVolumeLiters() const {
  return volumeLiters;
}


std::string const& Item::getName() const {
  return name;
}