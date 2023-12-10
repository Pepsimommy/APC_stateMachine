//
// Created by Gusita on 12/7/2023.
//

#ifndef PROJECT_PRODUCT_H
#define PROJECT_PRODUCT_H

#include <iostream>
#include <string>
#include <vector>

template <typename T>
class product{
public:

    T id;
    std::string name;
    double price;
    mutable int stock;

    //constructor
    product(T _id, const std::string& _name, double _price, int _stock): id(_id), name(_name), price(_price), stock(_stock) {}

    //destructor
    ~product()=default;


};

#endif //PROJECT_PRODUCT_H
