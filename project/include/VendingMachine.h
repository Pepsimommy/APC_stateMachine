//
// Created by Gusita on 12/4/2023.
//

#ifndef PROJECT_VENDINGMACHINE_H
#define PROJECT_VENDINGMACHINE_H

#include <iostream>
#include <chrono>

#include "VendingMachine_events.h"
#include "VendingMachine_states.h"
#include "product.h"


class seconds;
namespace project {

    template<typename T>
    class VendingMachine {

    public:
        VendingMachine_states currentState;

        // constructor
        VendingMachine() : currentState(VendingMachine_states::IDLE) {};

        //destructor
        ~VendingMachine() = default;

    //    void event_handler(VendingMachine_events event);

        void add_product( product<T> &p);

        void update_product( T &productCode, double newPrice, int newStock);

        void display_product_information(const T &productCode);

        void display_instructions();

        void display_menu();

        void idle();

    private:
        std::vector<product<T>> products;

        void receiving_money( T &productCode);

        void processing_selection( T &productCode);

        void dispensing( T &productCode);

        void refund(double amount);

        bool is_productInStock( T &productCode);

        bool is_productCodeValid( T &productCode);

    };
}

#endif //PROJECT_VENDINGMACHINE_H
