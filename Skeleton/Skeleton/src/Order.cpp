#include "../include/order.h"

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

const string Order::toString() const {
    string order = "Order id #" + std::to_string(id) + " , Customer id #" + std::to_string(customerId) + 
	", Distance: " + std::to_string(distance) + ", ";
	 status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER)
    return "";
}

