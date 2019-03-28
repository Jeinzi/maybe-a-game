#ifndef _ITEM_HPP
#define _ITEM_HPP

#include <string>


class Item {
  public:
    Item(std::string const& name, float weightGrams, float volumeLiters);
    float getWeightGrams() const;
    float getVolumeLiters() const;
    std::string const& getName() const;

  private:
    float weightGrams;
    float volumeLiters;
    std::string name;
};


#endif
