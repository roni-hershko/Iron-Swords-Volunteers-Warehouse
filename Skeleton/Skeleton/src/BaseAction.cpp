#include "../include/BaseAction.h"
#include <string>
#include <vector>
#include "WareHouse.h"
using std::string;
using std::vector;

BaseAction::BaseAction():errorMsg("<error_msg>"),status(ActionStatus::ERROR){}
void BaseAction::complete(){
    status=ActionStatus::COMPLETED;
}//chage status to completed, if it was completed

void BaseAction::error(string errorMsg){
    status=ActionStatus::ERROR;
    this->errorMsg=errorMsg;
}//change status to error, and print

string BaseAction::getErrorMsg() const{
    return errorMsg;
} //return the error message


//order
Order::Order(int id):customerId(id){} //constructor

void Order::act(WareHouse &wareHouse){
    wareHouse.addOrder(this);
} //add order to warehouse, maybe error

string Order::toString() const{
    if(getStatus()==ActionStatus::COMPLETED)
        return "Order Completed";
    return "Order Error: "+getErrorMsg();
}

Order *Order::clone() const{
    
     return new Order(customerId);
}
   
   
AddCustomer::AddCustomer(string customerName, string customerType, int distance, int maxOrders)
    :customerName(customerName), distance(distance),  maxOrders(maxOrders){
        if(customerType.at(0)=='c' || customerType.at(0)=='C') 
             this->customerType = CustomerType::Civilian;
        else this->customerType = CustomerType::Soldier;
    }//constructor


void customerName::act(WareHouse &wareHouse){
    wareHouse.addCustomer(this);
} //add customer to warehouse, never error

customerName *AddCustomer::clone() const{
    return new AddCustomer(customerName, customerType,  distance,  maxOrders);
}
string customerName::toString() const{
    if(getStatus()==ActionStatus::COMPLETED)
        return "AddCustomer Completed";
    return "AddCustomer Error: "+getErrorMsg();
}

