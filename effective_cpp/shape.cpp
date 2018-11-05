/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <iostream>

#include "shape.h"


namespace lxy {

/* ----------------- Shape ----------------- */
void Shape::draw() const {
    // Define a default action for the function interface.
    std::cout << "draw a shape" << std::endl;
}

void Shape::error(const std::string &msg) {
    std::cout << "ERROR: " << msg << std::endl;
}

int Shape::object_id() const {
    return 1;
}

/* ----------------- Rectangle --------------------*/
Rectangle::Rectangle(int len, int hgt) 
    : m_length(len), m_height(hgt) {
}

void Rectangle::draw() const {
    std::cout << "draw a rectangle" << std::endl;
}

void Rectangle::error(const std::string &msg) {
    Shape::error(msg);
}

}
