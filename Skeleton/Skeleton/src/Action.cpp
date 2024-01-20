#include "../include/Action.h"
#include <string>
#include <vector>
#include "WareHouse.h"
using std::string;
using std::vector;


BaseAction:BaseAction():errorMsg("<error_msg>"),status(ActionStatus::ERROR){}
void BaseAction:complete(){
    status=ActionStatus::COMPLETED;
}//chage status to completed, if it was completed

void BaseAction:error(string errorMsg){
    status=ActionStatus::ERROR;
    this->errorMsg=errorMsg;
}//change status to error, and print

string BaseAction:getErrorMsg() const{
    return errorMsg;
} //return the error message


//order
AddOrder::AddOrder(int id):customerId(id){} //constructor

void AddOrder::act(WareHouse &wareHouse){
    wareHouse.addOrder(this);
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

PrintVolunteerStatus :: PrintVolunteerStatus(int id) : VolunteerId(id){}//constructor

void PrintVolunteerStatus ::act(WareHouse &wareHouse){
	if(wareHouse.getVolunteer(VolunteerId)==nullptr)
		error("Volunteer doesnt exist");

	else {
		cout/n << "VolunteerID: " << VolunteerId <<; 	//print volunteer id	
		cout/n << "isBusy: " wareHouse.getVolunteer(VolunteerId)->isBusy()<<; //print is busy

		if (wareHouse.getVolunteer(VolunteerId)->isBusy() == false){ //if the volunteer is not busy
			cout/n << "OrderID: None " <<; 
			cout/n << "timeLeft: None " <<; 
			cout/n << "ordersLeft: None " <<; 
		}
		else if (wareHouse.getVolunteer(VolunteerId)->getTimeLeft() != nullptr){ //if the volunteer is a collector
			cout/n << "OrderID: " wareHouse.getVolunteer(VolunteerId)->getOrder()->getId()<<;
			cout/n << "timeLeft: " << wareHouse.getVolunteer(VolunteerId)->getTimeLeft() <<; 
			if(wareHouse.getVolunteer(VolunteerId)->getDistanceLeft() != nullptr)
				cout/n << "ordersLeft: " << wareHouse.getVolunteer(VolunteerId)->getDistanceLeft() <<;
				// USING DISTANCE LEFT AND NOT MAX Distance AS WRITTEN IN THE PAPER
		}
		else { //if the volunteer is a driver
			cout/n << "OrderID: " wareHouse.getVolunteer(VolunteerId)->getOrder()->getId()<<;
			cout/n << "distanceLeft: " << wareHouse.getVolunteer(VolunteerId)->getDistanceLeft() <<; 
			if (wareHouse.getVolunteer(VolunteerId)->getNumOrdersLeft() != nullptr)
				cout/n << "ordersLeft: " << wareHouse.getVolunteer(VolunteerId)->getNumOrdersLeft() <<;
				// USING ORDERS LEFT AND NOT MAX ORDER AS WRITTEN IN THE PAPER
		}
	}	
}
