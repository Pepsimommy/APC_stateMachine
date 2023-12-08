//
// Created by Gusita on 12/4/2023.
//
#include "../include/VendingMachine.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

namespace project {

    template class project::VendingMachine<std::string>;

    template<typename T>
    void project::VendingMachine<T>::add_product( product<T> &p) {
        products.push_back(p);
    }

    template<typename T>
    void project::VendingMachine<T>::update_product( T &productCode, double newPrice, int newStock) {
        for (auto &p: products)
            if (p.id == productCode) {
                p.price = newPrice;
                p.stock = newStock;
                std::cout << "product updated successfully\n";
                return;
            }
        std::cout << "product with code" << productCode << "not found\n";
    }
//TODO maybe add a function to display product info

    template<typename T>
    void project::VendingMachine<T>::idle() {
        std::cout << "Please select product";
        T productCode;
        std::cin >> productCode;
        std::cout << "You selected the product the code: " << productCode << "\n";
        currentState = VendingMachine_states::PROCESSING_SELECTION;
        processing_selection(productCode);
        //TODO think about this function a bit more cus i feel like its not complete
//        delete productCode;
    }

    template<typename T>
    bool project::VendingMachine<T>::is_productInStock( T &productCode) {
        for (const auto &p: products) {
            if (p.id == productCode && p.stock > 0) {
                return true;
            }
        }
        return false;
    }

    template<typename T>
    bool project::VendingMachine<T>::is_productCodeValid( T &productCode) {
        for (const auto &p: products) {
            if (p.id == productCode) {
                return true;
            }
        }
        return false;
    }

    template<typename T>
    void project::VendingMachine<T>::processing_selection( T &productCode) {

        auto startTime = std::chrono::steady_clock::now();
        auto timeout = std::chrono::seconds(90);
        bool transactionProcessed = false;

        while (std::chrono::steady_clock::now() - startTime < timeout) {
            if (!is_productCodeValid(productCode)) {
                std::cout << "Code product is not valid. Try again or cancel";
            } else {
                if (!is_productInStock(productCode)) {
                    std::cout << "product not in stock pls make other selection";
                } else {
                    std::cout << "processing selection for product code: " << productCode << "\n";
                    currentState = VendingMachine_states::ACCEPTING_MONEY;
                    receiving_money(productCode);
                    transactionProcessed = true;
                    return;
                }
            }

            std::cout << "enter product code or cancel to cancel: ";
            T tempProductCode;
            std::getline(std::cin, tempProductCode);

            productCode = tempProductCode;

            if (productCode == "cancel") {
                std::cout << "transaction cancelled\n";
                return;
            }
        }
        //TODO: rethink a bit timeout handling here
        if (!transactionProcessed)
            std::cout << "timeout, transaction cancelled \n";

    }

    template<typename T>
    void project::VendingMachine<T>::receiving_money( T &productCode) {

        //retrieve product price from the products vector
        double totalAmount = 0.0;
        double productPrice = -1.0;
        for (const auto &p: products) {
            if (p.id == productCode) {
                productPrice = p.price;
                break;
            }
        }

        //start timer for 90 seconds
        auto startTime = std::chrono::steady_clock::now();
        auto timeout = std::chrono::seconds(90);

        //start a loop that will continue until at least the full amount is inserted or until the timer expires
        while (totalAmount < productPrice && std::chrono::steady_clock::now() - startTime < timeout) {

            double insertedAmount = 0.0;
            std::cout << "Insert money:";

            //error handling for the user input
            while (!(std::cin >> insertedAmount)) {
                std::cout << "Error: Invalid input. Please enter a numeric value.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                // Add a prompt to enter valid input
                std::cout << "Insert money:";
            }

            totalAmount += insertedAmount;

            if (totalAmount < productPrice) {
                std::cout << "Total amount received:" << totalAmount << ".Insert more money.\n";
            }
        }

        if (totalAmount >= productPrice) {
            std::cout << "Amount received is sufficient.Proceeding to dispensing.\n";
            currentState = VendingMachine_states::DISPENSING;
            dispensing(productCode);
            if (totalAmount > productPrice) {
                std::cout << "amount received is too much, initializing refund after dispensing\n";
                currentState = VendingMachine_states::REFUNDING_MONEY;
                refund(productPrice - totalAmount);
            } else
                currentState = VendingMachine_states::IDLE;

        } else {
            std::cout << "timeout, transaction cancelled, initialize refund\n";
            currentState = VendingMachine_states::REFUNDING_MONEY;
            refund(totalAmount);
        }
    }


    template<typename T>
    void project::VendingMachine<T>::dispensing( T &productCode) {

        //search for product name and update stock to account for the dispensing
        std::string productName{};
        for (const auto &p: products) {
            if (p.id == productCode) {
                productName = p.name;
                p.stock--;
                break;
            }
        }
        std::cout << "dispensing product" << productName << std::endl;
        std::this_thread::sleep_for(
                std::chrono::milliseconds(1000));//simulate the dispensing time of an actual vending machine

    }

    template<typename T>
    void project::VendingMachine<T>::refund(double amount) {
        std::cout << "refunding " << amount << "euros";
        currentState = VendingMachine_states::IDLE;
    }

//TODO think if the start_timer and stock_handler functions are needed or can be removed
    template<typename T>
    void start_timer() {

    }

    template<typename T>
    void stock_handler() {

    }
}