#include <iostream>
#include "../include/VendingMachine.h"
#include "../include/product.h"

int main() {

    project::VendingMachine<std::string> VM;

    product<std::string> prod1{"A1", "chips", 1.5, 6};
    product<std::string> prod2{"B2", "chocolate", 3.2, 10};

    VM.add_product(prod1);
    VM.add_product(prod2);

    VM.idle();

    return 0;
}
