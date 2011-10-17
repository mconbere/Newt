// Copyright (c) 2011 Morgan Conbere. All rights reserved.
// Author: Morgan Conbere <mconbere@gmail.com>

#pragma once

#include <map>
#include <string>

namespace newt {

  class Inventory {
  public:
    void AddItem(const std::string& item, int count);
    void AddInventory(const Inventory& other_inventory);
    void Clear();
    // TODO(mconbere): implement this while not on a bus
    //bool IsEmpty() { return items_map_.empty(); }

    // Hacky interface to see items
    const std::map<std::string, int>& ItemsMap() const { return items_map_; }
    
    // TODO(mconbere): flesh out inventory features. Sharing items, etc. For now,
    // just add MORE!!!!
    
  private:
    // TODO(mconbere): Something better than a string to identify an item is
    // important here, but this will work for now.
    // Map of item name -> number of items possessed. number of items possessed
    // should always be greater than 0.
    std::map<std::string, int> items_map_;
  };

}  // namespace newt