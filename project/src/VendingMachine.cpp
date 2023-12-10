//
// Created by Gusita on 12/4/2023.
//
#include "../include/VendingMachine.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>



namespace project {

    template
    class project::VendingMachine<std::string>;

    template<typename T>
    void project::VendingMachine<T>::display_instructions() {


        std::cout << "\n";
        std::cout << "Welcome to the Vending Machine!\n";
        std::cout << "To make a selection, enter the product code and press Enter.\n";
        std::cout << "To cancel a transaction, enter 'cancel' and press Enter.\n";
        std::cout << "You have 30 seconds to complete a transaction.\n";
        std::cout << "Let's get started!\n";
        std::cout << "\n";

    }

    template<typename T>
    void project::VendingMachine<T>::display_menu() {
        std::cout << "\n";
        std::cout << "=== Vending Machine Menu ===\n";

        for (const auto &p: products) {
            std::cout << "Product Code: " << p.id << "\n";
            std::cout << "Name: " << p.name << "\n";
            std::cout << "Price: $" << p.price << "\n";
            std::cout << "Stock: " << p.stock << "\n";
            std::cout << "---------------------------\n";
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    template<typename T>
    void project::VendingMachine<T>::add_product(product<T> &p) {
        products.push_back(p);
    }

    template<typename T>
    void project::VendingMachine<T>::display_product_information(const T &productCode) {
        for (auto &p: products)
            if (p.id == productCode) {
                std::cout << p.id << '\n';
                std::cout << p.name << '\n';
                std::cout << p.price << '\n';
                std::cout << p.stock << '\n';
            }
    }

    template<typename T>
    void project::VendingMachine<T>::update_product(T &productCode, double newPrice, int newStock) {
        for (auto &p: products)
            if (p.id == productCode) {
                p.price = newPrice;
                p.stock = newStock;
                std::cout << "product updated successfully \n";
                return;
            }
        std::cout << "product with code " << productCode << "  not found\n";
    }

    template<typename T>
    void project::VendingMachine<T>::idle() {
        std::cout << "Please select product.\n";
        T productCode;
        std::cin >> productCode;
        std::cout << "You selected the product with code: " << productCode << "\n";
        currentState = VendingMachine_states::PROCESSING_SELECTION;
        processing_selection(productCode);
        //TODO think about this function a bit more cus i feel like its not complete
    }

    template<typename T>
    bool project::VendingMachine<T>::is_productInStock(T &productCode) {
        for (const auto &p: products) {
            if (p.id == productCode && p.stock > 0) {
                return true;
            }
        }
        return false;
    }

    template<typename T>
    bool project::VendingMachine<T>::is_productCodeValid(T &productCode) {
        for (const auto &p: products) {
            if (p.id == productCode) {
                return true;
            }
        }
        return false;
    }

    template<typename T>
    void project::VendingMachine<T>::processing_selection(T &productCode) {

        auto startTime = std::chrono::steady_clock::now();
        auto timeout = std::chrono::seconds(30);

        while (std::chrono::steady_clock::now() - startTime < timeout)

        {
            if (!is_productCodeValid(productCode))
            {
                std::cout << "Code product is not valid. Please try again or cancel.\n";
            }
            else if (!is_productInStock(productCode))
            {
                std::cout << "Product is not in stock. Please make another selection\n";
            }
            else
            {
                std::cout << "Processing selection for product code: " << productCode << "\n";
                currentState = VendingMachine_states::ACCEPTING_MONEY;
                receiving_money(productCode);
                return;
            }

            std::cout << "New product code or cancel\n";
            T tempProductCode;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, tempProductCode);
            std::cin.ignore();

            productCode = tempProductCode;

            if (productCode == "cancel") {
                std::cout << "Transaction cancelled\n";
                return;
            }

            if (std::chrono::steady_clock::now() - startTime >= timeout) {
                std::cout << "Timeout, transaction cancelled \n";
                currentState = VendingMachine_states::IDLE;
                return;
            }

        }
    }

    template<typename T>
    void project::VendingMachine<T>::receiving_money(T &productCode) {

        //retrieve product price from the products vector
        double totalAmount = 0.0;
        double productPrice = -1.0;

        for (const auto &p: products) {
            if (p.id == productCode) {
                productPrice = p.price;
                break;
            }
        }

        //start timer for 30 seconds
        auto startTime = std::chrono::steady_clock::now();
        auto timeout = std::chrono::seconds(30);

        //start a loop that will continue until at least the full amount is inserted or until the timer expires
        while (totalAmount < productPrice && std::chrono::steady_clock::now() - startTime < timeout)
        {

            double insertedAmount = 0.0;
            std::cout << "Insert money: ";

            //error handling for the user input
            while (!(std::cin >> insertedAmount))
            {
                std::cout << "Error: Invalid input. Please enter a numeric value.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                // Add a prompt to enter valid input
                std::cout << "Insert money:";
            }

            totalAmount += insertedAmount;

            if (totalAmount < productPrice) {
                std::cout << "Total amount received: " << totalAmount << " Insert more money.\n";
            }

            if (std::chrono::steady_clock::now() - startTime >= timeout) {
                std::cout << "Timeout, transaction cancelled, initializing refund.\n";
                currentState = VendingMachine_states::REFUNDING_MONEY;
                refund(totalAmount);
                return;
            }
        }

        if (totalAmount >= productPrice) {
            std::cout << "Amount received is sufficient.Proceeding to dispensing.\n";
            currentState = VendingMachine_states::DISPENSING;
            dispensing(productCode);
            if (totalAmount > productPrice) {
                std::cout << "amount received is too much, initializing refund after dispensing.\n";
                currentState = VendingMachine_states::REFUNDING_MONEY;
                refund(-(productPrice - totalAmount));
            } else
                currentState = VendingMachine_states::IDLE;

         }
    }


    template<typename T>
    void project::VendingMachine<T>::dispensing(T &productCode) {

        //search for product name and update stock to account for the dispensing
        std::string productName{};
        for (const auto &p: products) {
            if (p.id == productCode) {
                productName = p.name;
                p.stock--;
                break;
            }
        }
        std::cout << "dispensing product: " << productName << "\n";
        std::this_thread::sleep_for(
                std::chrono::milliseconds(1000));//simulate the dispensing time of an actual vending machine

    }

    template<typename T>
    void project::VendingMachine<T>::refund(double amount) {
        std::cout << "refunding " << amount << " euros\n";
        currentState = VendingMachine_states::IDLE;
    }
}