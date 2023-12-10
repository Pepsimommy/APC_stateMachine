#include <iostream>
#include "../include/VendingMachine.h"
#include "../include/product.h"

int main() {

    project::VendingMachine<std::string> VM;

    product<std::string> prodA1{"A1", "chips", 1.5, 6};
    product<std::string> prodB2{"B2", "chocolate", 3.2, 10};
    product<std::string> prodC3 {"C3", "snickers", 1.52, 3};
    product<std::string> prodD4 {"D4", "ur mom", 420, 69};
    product<std::string> prodE5 {"E5", "water", 0.57, 12};
    product<std::string> prodF6 {"F6", "ketamine", 10, 2};


    VM.add_product(prodA1);
    VM.add_product(prodB2);
    VM.add_product(prodC3);
    VM.add_product(prodD4);
    VM.add_product(prodE5);
    VM.add_product(prodF6);

   // VM.display_product_information("A1");

    VM.display_instructions();

//    VM.display_menu();
//
//    VM.idle();

    while (true) {
        VM.display_menu();
        std::string productCode;
        std::cout << "Please select a product code or enter 'cancel' to exit: ";
        std::getline(std::cin, productCode);

        if (productCode == "cancel") {
            std::cout << "Exiting program...\n";
            break;
        }

       // VM.display_product_information(productCode);
        VM.idle();
    }

    return 0;
}
