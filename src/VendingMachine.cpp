//
// Created by Gusita on 12/4/2023.
//
#include "../include/VendingMachine.h"



template<typename T>
void project::VendingMachine<T>::add_product(const product<T>& p){
    products.push_back(p);
}

template<typename T>
void project::VendingMachine<T>::idle() {
    std::cout<<"Please select product";
    std::string productCode;
    std::cin>>productCode;
    std::cout<<"You selected the product the code: "<< productCode<<"\n";
    currentState=VendingMachine_states::PROCESSING_SELECTION;
    processing_selection(90, productCode);


}

template<typename T>
bool project::VendingMachine<T>::is_productInStock(const T& productCode) {
    for(const auto& p: products){
        if( p.id==productCode  && p.stock>0){
            return true;
        }
    }
    return false;
}

template<typename T>
bool project::VendingMachine<T>::is_productCodeValid(const T& productCode) {
    for(const auto& p: products){
        if(p.id==productCode ){
            return true;
        }
    }
    return false;
}

template<typename T>
void project::VendingMachine<T>::processing_selection( const T& productCode) {

    auto startTime = std::chrono::steady_clock::now();
    auto timeout = std::chrono::seconds (90);

    while (std::chrono::steady_clock::now() - startTime < timeout)
    {
        if(!is_productCodeValid(productCode)){
            std::cout<<"Code product is not valid. Try again or cancel";
        }
        else
        {
            if(!is_productInStock(productCode))
            {
                std::cout << "product not in stock pls make other selection";
            }
            else
            {
                std::cout<<"processing selection for product code: " << productCode <<"\n";
                currentState=VendingMachine_states::ACCEPTING_MONEY;
                receiving_money(productCode);
                return;
            }
        }

        std::cout << "enter product code or cancel to cancel: ";
        std::cin >> productCode;
    }

    std::cout <<"timeout, transaction cancelled \n";
}

template<typename T>
void project::VendingMachine<T>::receiving_money(const T& productCode){
     double totalAmount=0.0;
     double productPrice=5.0;

     while(totalAmount<productPrice){
         double insertedAmount=0.0;
         std::cout<<"Insert money:";
         std::cin>>insertedAmount;

         totalAmount+=insertedAmount;

         if(totalAmount< productPrice){
             std::cout<<"Total amount received:"<< totalAmount<<".Insert more money.\n";

         }

     }

     std::cout<<"Amount received is sufficient.Proceeding to dispensing.\n";

}


template<typename T>
void dispensing() {

}

template<typename T>
void refund() {

}

template<typename T>
void start_timer() {

}

template<typename T>
void stock_handler() {

}