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
    this->status = status; // DO WE NEED TO USE THIS->? OR CAN WE JUST USE status = status?
}

void Order::setCollectorId(int collectorId) {
    this->collectorId = collectorId; //DO WE NEED TO USE THIS->? OR CAN WE JUST USE collectorId = collectorId?
}

void Order::setDriverId(int driverId) {
    this->driverId = driverId; //DO WE NEED TO USE THIS->? OR CAN WE JUST USE driverId = driverId?
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
    // TO COMPLITE
  
    return "";
}

