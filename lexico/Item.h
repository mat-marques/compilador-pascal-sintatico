#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string>

using namespace std;

class Item {
  public:
    string var_name;
    string var_category;
    int var_level;
    string var_type;
    int var_displacement;
    string var_reference;
    Item();
    Item(string var_name, string var_category, int var_level, string var_type, int var_displacement, string var_reference);
    Item(string var_name);
    void show();
};

#endif
