#include "../include/Action.h"
#include <string>
#include <vector>
<<<<<<< HEAD
#include <WareHouse.h>
=======
#include "WareHouse.h"
#include <iostream> // Added by roni
>>>>>>> 99ff69322ef4706983e2b0dd2e88428650d3cfac
using std::string;
using std::vector;


//base action
<<<<<<< HEAD
BaseAction::BaseAction():errorMsg("<error_msg>"),status(ActionStatus::ERROR){}
=======
BaseAction::BaseAction()
	: errorMsg("<error_msg>"),status(ActionStatus::ERROR){} //constructor

>>>>>>> 99ff69322ef4706983e2b0dd2e88428650d3cfac
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


<<<<<<< HEAD
//order
AddOrder::AddOrder(int id) : customerId(id){} //constructor

void AddOrder::act(WareHouse &wareHouse){
    if(wareHouse.getCustomer(customerId) == nullptr || &wareHouse.getCustomer(customerId) == NULL){
        BaseAction::error("Customer does not exist");
    }
    else if (wareHouse.getCustomer(customerId).addOrder(customerId)==-1)
        BaseAction::error("Customer reached max orders");
    Order* order = new Order(wareHouse.getOrderCounter(), customerId, (wareHouse.getCustomer(customerId)).getCustomerDistance());
    order->setStatus(OrderStatus::PENDING);
    wareHouse.getPendingOrders().push_back(order);
=======


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
>>>>>>> 99ff69322ef4706983e2b0dd2e88428650d3cfac
} //add order to warehouse, maybe error

string AddOrder::toString() const{
}

AddOrder *AddOrder::clone() const{
    return new AddOrder(customerId);
}


//add customer
AddCustomer::AddCustomer(string customerName, string customerType, int distance, int maxOrders) 
    : customerName(customerName),distance(distance), maxOrders(maxOrders){
    } //constructor    

void AddCustomer::act(WareHouse &wareHouse){
    customerId=wareHouse.getCustomerCounter();
     if(CustomerType(customerType)==CustomerType::Civilian){
           wareHouse.addCustomer(new CivilianCustomer(customerId ,customerName, distance, maxOrders));
     }
     else {
            wareHouse.addCustomer(new SoldierCustomer (customerId,customerName, distance, maxOrders));
     }   
} //add customer to warehouse, never error

AddCustomer *AddCustomer::clone() const{
    if(customerType == CustomerType::Civilian){
        AddCustomer* newCcus= new AddCustomer(customerName, "Civilian",  distance,  maxOrders);
        newCcus->customerId=customerId;
        return newCcus;
    }
    AddCustomer* newCcus= new AddCustomer(customerName, "Soldier",  distance,  maxOrders);
    newCcus->customerId=customerId;
    return newCcus;
}

string AddCustomer::toString() const{
    if(CustomerType(customerType)==CustomerType::Civilian){  
        return "AddCustomer " + customerName + ", Civilian, distance:" + distance + ", maxOrder: " + maxOrders;
    }
    return "AddCustomer: " + customerName + ", Soldier, distance:" + distance + ", maxOrder: " + maxOrders;
}



//PrintOrderStatus
PrintOrderStatus::PrintOrderStatus(int id) : orderId(id){}//constructor

void PrintOrderStatus::act(WareHouse &wareHouse){
    if(wareHouse.getOrder(orderId)==(nullptr))
        error("Order doesn't exist");
    else{
        cout<< "OrderId <" <<orderId<< ">\n" 
        << "OrderStatus <" <<OrderStatus(wareHouse.getOrder(orderId).getStatus())<< ">\n" 
        << "CustomerId <" <<wareHouse.getOrder(orderId)->getCustomer()->getId()<< ">\n" 
        << "Collector <" <<wareHouse.getOrder(orderId)->getCollectorId()<< ">\n" 
        << "Driver <" <<wareHouse.getOrder(orderId)->getDriverId()<< ">\n" ;
    }
} //print order status, maybe error 

PrintOrderStatus *PrintOrderStatus::clone() const{
    return new PrintOrderStatus(orderId);
}

string PrintOrderStatus::toString() const{ //need to check
}   
 


//PrintCustomerStatus

<<<<<<< HEAD
PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId){}//constructor

void PrintCustomerStatus::act(WareHouse &wareHouse){
=======
 PrintCustomerStatus::PrintCustomerStatus(int customerId){
        this->customerId=customerId;
    }//constructor
void act(WareHouse &wareHouse) override{
>>>>>>> 99ff69322ef4706983e2b0dd2e88428650d3cfac
    if(wareHouse.getCustomer(customerId)==nullptr)
        error("Customer doesn't exist");
    else{
        cout<<"customerId : "+ customerId +"\n";
        vector<Order*> orderList=wareHouse.((getCustomer(customerId))->getOrderList());
        for(int i=0; i < orderList.size(); i++){
             cout<<"orderId :" + (*orderList[i]).getId() +"\n orderStatus : "+ OrderStatus((*orderList[i]).getStatus());
        }
        if(wareHouse.getCustomer(customerId).canMakeOrder())
            cout<<"\n num order left : "+wareHouse.(getCustomer(customerId))->getMaxOrders()-wareHouse.(getCustomer(customerId))->getNumOrders() +"\n";
        else cout<<"\n num order left : 0 ";
    } 
}//print customer status, maybe error

PrintCustomerStatus *PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(customerId);
}

string PrintCustomerStatus::toString() const{ //need to check
}

        



//PrintVolunteerStatus
PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id){} //constructor

void act(WareHouse &wareHouse) {
	if(wareHouse.getVolunteer(volunteerId)==nullptr)
		error("Volunteer does not exist");
	else wareHouse.printVolunteerStatus(this);
}//print volunteer status, maybe error

PrintVolunteerStatus *SimulateStep::clone() const{
	return new PrintVolunteerStatus(id);
}


string PrintVolunteerStatus::toString() const{
	if(getStatus()==ActionStatus::COMPLETED)
		return "PrintVolunteerStatus Completed";
	return "PrintVolunteerStatus Error: "+getErrorMsg();
}


//PrintActionsLog


