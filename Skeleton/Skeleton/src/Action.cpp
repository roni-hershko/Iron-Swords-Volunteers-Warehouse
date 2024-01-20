#include "../include/Action.h"
#include <string>
#include <vector>
#include <WareHouse.h>
using std::string;
using std::vector;


//base action
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

PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId){}//constructor

void PrintCustomerStatus::act(WareHouse &wareHouse){
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
