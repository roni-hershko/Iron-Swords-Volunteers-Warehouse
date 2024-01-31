#include <string>
#include <vector>
#include "../include/Customer.h"
using std::string;
using std::vector;


Customer::Customer(int id, const string &name, int locationDistance, int maxOrders)
	: id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), ordersId(){ }

const std::string &Customer::getName() const {
	return name;
}

int Customer::getId() const {
	return id;
}
    
int Customer::getCustomerDistance() const {
	return locationDistance;
}
    
int Customer::getMaxOrders() const {
	return maxOrders;
} 
    
int Customer::getNumOrders() const {
	return ordersId.size();
}
    
bool Customer::canMakeOrder() const {//Returns true if the customer didn't reach max orders
    if(getNumOrders() < maxOrders)
         return true;
    return false;
} 
   
const vector<int>& Customer::getOrdersIds() const {   
	return ordersId;
}
    
int Customer::addOrder(int orderId){
    if(canMakeOrder()){
        ordersId.push_back(orderId); 
    	return orderId;
    }
    return -1;
}; //return OrderId if order was added successfully, -1 otherwise


SoldierCustomer::SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders)
	: Customer(id, name, locationDistance, maxOrders) {}

SoldierCustomer* SoldierCustomer::clone() const {
	SoldierCustomer* newSoldierCustomer= new SoldierCustomer(*this);
	for(int i=0; i<newSoldierCustomer->getNumOrders(); i++){
		newSoldierCustomer->addOrder(getOrdersIds()[i]);
	}
	return newSoldierCustomer;
}


CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders)
	: Customer(id, name, locationDistance, maxOrders) {}
	
CivilianCustomer* CivilianCustomer::clone() const{
    CivilianCustomer* newCivilianCustomer= new CivilianCustomer(*this);
    for(int i=0; i<newCivilianCustomer->getNumOrders(); i++){
        newCivilianCustomer->addOrder(getOrdersIds()[i]);
    }
    return newCivilianCustomer;
}
        
        
        