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

        void event_handler(VendingMachine_events event);

        void add_product(const product<T> &p);

        void update_product(const T &productCode, double newPrice, int newStock);


    private:
        std::vector<product<T>> products;

        void idle();

        void receiving_money(const T &productCode);

        void processing_selection(const T &productCode);

        void dispensing(const T &productCode);

        void refund(double amount);

        void start_timer(std::chrono::seconds duration);

        void stock_handler();

        bool is_productInStock(const T &productCode);

        bool is_productCodeValid(const T &productCode);

    };
}

#endif //PROJECT_VENDINGMACHINE_H
