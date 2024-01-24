#include "Order.h"

Order::Order(int id, int customerId, int distance)
	: id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {}

int Order::getId() const {
	return id;
}

int Order::getCustomerId() const {
	return customerId;
}

void Order::setStatus(OrderStatus status) {
    this->status = status; 
}

void Order::setCollectorId(int collectorId) {
    this->collectorId = collectorId; 
}

void Order::setDriverId(int driverId) {
    this->driverId = driverId; 
}

int Order::getCollectorId() const {
    return collectorId;
}

int Order::getDriverId() const {
    return driverId;
}

OrderStatus Order::getStatus() const {
    return status;
}

const string Order::toString() const { //ready for printOrderStatus function

    string statusString;
    OrderStatus status; 
	string collector;
	string driver;

    switch(status){
        case OrderStatus::PENDING:
            statusString= "PENDING";
            break;
        case OrderStatus::COLLECTING:
            statusString= "COLLECTING";
            break;
        case OrderStatus::DELIVERING:
            statusString= "DELIVERING";
            break;
        case OrderStatus::COMPLETED:
            statusString= "COMPLETED";
            break;
    }	
	if(collectorId == NO_VOLUNTEER)
		collector = "None";
	else
		collector = std::to_string(collectorId);

	if(driverId == NO_VOLUNTEER)
		collector = "None";
	else	
		driver = std::to_string(driverId);

	string str= "OrderId:" + std::to_string(id) + ", orderStatus:" + statusString + ", CustomerId:" + std::to_string(customerId) + ", distance" + std::to_string(distance) +
				 ", collectorId:" + collector + ", driverId:" + driver;
	return str;
 } 
