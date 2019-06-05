#include <iostream>
#include <string>
#include "Item.h"

Item::Item() {}

Item::Item(string var_name) {
    this->var_name = var_name;
}

Item::Item(string var_name, string var_category, string var_level, string var_type, string var_displacement, string var_reference){
    this->var_name = var_name;
    this->var_category = var_category;
    this->var_level = var_level;
    this->var_type = var_type;
    this->var_displacement = var_displacement;
    this->var_reference = var_reference;
}

void Item::show(){
    cout<< this->var_name << " " << this->var_category << " " << this->var_level << " " << this->var_type  << " " << this->var_displacement << " " << this->var_reference << endl;
}