// Copyright (c) 2011 Morgan Conbere. All rights reserved.
// Author: Morgan Conbere <mconbere@gmail.com>

#include "inventory.h"

namespace newt {

void Inventory::AddItem(const std::string& item, int count) {
  std::map<std::string, int>::iterator ours = items_map_.find(item);
  if (ours != items_map_.end()) {
    ours->second += count;
  } else {
    items_map_[item] = count;
  }    
}

void Inventory::AddInventory(const Inventory& other_inventory) {
  for (std::map<std::string, int>::const_iterator it = other_inventory.items_map_.begin();
       it != other_inventory.items_map_.end(); ++it) {
    AddItem(it->first, it->second);
  }
}
  
void Inventory::Clear() {
  items_map_.clear();
}

}