#include "../include/Action.h"
#include <string>
#include <vector>
#include "WareHouse.h"
#include <iostream> // Added by roni
using std::string;
using std::vector;


//base action
BaseAction::BaseAction()
	: errorMsg("<error_msg>"),status(ActionStatus::ERROR){} //constructor

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




//SimulateStep
SimulateStep::SimulateStep(int numOfSteps):numOfSteps(numOfSteps){} //constructor

void SimulateStep::act(WareHouse &wareHouse){
	for(int i=0; i<numOfSteps; i++){
		//complete
	
	}
	std::cout << toString() << std::endl; //i dont know if its gonna work
} //step the warehouse, never error	

SimulateStep *SimulateStep::clone() const{
	return new SimulateStep(numOfSteps);
}		

string SimulateStep::toString() const{
	if(getStatus()==ActionStatus::COMPLETED)
		return "SimulateStep Completed";
	return "SimulateStep Error: "+getErrorMsg();
}




//AddOrder
AddOrder::AddOrder(int id):customerId(id){} //constructor

void AddOrder::act(WareHouse &wareHouse){
    if(wareHouse.getCustomer(customerId) == nullptr || &wareHouse.getCustomer(customerId) == NULL)
        BaseAction::error("Canot place this order");
    else if (wareHouse.getCustomer(customerId).addOrder(customerId)==-1)
        BaseAction::error("Canot place this order");
	else {
   		wareHouse.addOrder(this);
		wareHouse.get
	}
		//pending status
		//what the constructor need
} //add order to warehouse, maybe error

string AddOrder::toString() const{
    if(getStatus()==ActionStatus::COMPLETED)
        return "Order Completed";
    return "Order Error: "+getErrorMsg();
}

AddOrder *AddOrder::clone() const{
    
     return new AddOrder(customerId);
}


 //add customer  
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



//PrintOrderStatus
PrintOrderStatus(int id):orderId(id){}//constructor

void PrintOrderStatus::act(WareHouse &wareHouse){
    if(wareHouse.getOrder(orderId)==nullptr)
        error("Order does not exist");
    else wareHouse.printOrderStatus(this);
} //print order status, maybe error 

PrintOrderStatus *PrintOrderStatus::clone() const{
    return new PrintOrderStatus(orderId);
}

string PrintOrderStatus::toString() const{
    if(getStatus()==ActionStatus::COMPLETED)
        return "PrintOrderStatus Completed";
    return "PrintOrderStatus Error: "+getErrorMsg();
}   
 

//PrintCustomerStatus

 PrintCustomerStatus::PrintCustomerStatus(int customerId){
        this->customerId=customerId;
    }//constructor
void act(WareHouse &wareHouse) override{
    if(wareHouse.getCustomer(customerId)==nullptr)
        error("Customer does not exist");
    else wareHouse.printCustomerStatus(this);
}//print customer status, maybe error
PrintCustomerStatus *clone() const override;
string toString() const override;

        const int customerId;



//PrintVolunteerStatus
PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id){} //constructor

void act(WareHouse &wareHouse) {
	if(wareHouse.getVolunteer(volunteerId)==nullptr)
		error("Volunteer does not exist");
	else wareHouse.printVolunteerStatus(this);
}//print volunteer status, maybe error

PrintVolunteerStatus *clone() const{
	return new PrintVolunteerStatus(volunteerId);
}


string toString() const{
	if(getStatus()==ActionStatus::COMPLETED)
		return "PrintVolunteerStatus Completed";
	return "PrintVolunteerStatus Error: "+getErrorMsg();
}


//PrintActionsLog


