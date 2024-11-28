//
// Created by david on 11/28/24.
//
#include <iostream>
#include "base_handler.h"

base_handler::~base_handler() {
    std::cout << "base_handler events: " << _events << std::endl;
    detach();
}
