#include "../include/Volunteer.h"

Volunteer::Volunteer(int id, const string &name) : completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id), name(name) {}

int Volunteer::getId() const{
    return id;
}

const string &Volunteer::getName() const{
    return name;
}

int Volunteer::getActiveOrderId() const{
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const{
    return completedOrderId;
}

bool Volunteer::isBusy() const{
    return activeOrderId != NO_ORDER;
}

bool Volunteer::isCollector() const{
	return false;
}



//CollectorVolunteer
CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown) 
	: Volunteer(id, name), coolDown(coolDown), timeLeft(0) {}

CollectorVolunteer *CollectorVolunteer::clone() const {
	CollectorVolunteer* clone = new CollectorVolunteer(getId(), getName(), getCoolDown());
    clone->setTimeLeft(getTimeLeft());
    clone->activeOrderId = getActiveOrderId();
    clone->completedOrderId = getCompletedOrderId();

    return clone;
}

void CollectorVolunteer::setTimeLeft(int newOrdersLeft) {
	timeLeft = newOrdersLeft;
}

void CollectorVolunteer::step(){
    if (decreaseCoolDown()) {
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

int CollectorVolunteer::getCoolDown() const{
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const{
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown(){
     if (timeLeft > 0) {
        timeLeft--;
    }
    return timeLeft == 0;
}

bool CollectorVolunteer::hasOrdersLeft() const{
    return true; // always true for CollectorVolunteer
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const{
    return !isBusy();
}

void CollectorVolunteer::acceptOrder(const Order &order){
    activeOrderId = order.getId();
    timeLeft = coolDown;

}

bool CollectorVolunteer::isCollector() const{
    return true;
}

bool CollectorVolunteer::isLimited() const{
	return false;
}

string CollectorVolunteer::toString() const{ //get ready for printVolunteerStatus function ???
    return "Collector Volunteer: " + getName() +" id " + std::to_string(getId());
}



//LimitedCollectorVolunteer
LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown, int maxOrders)
    : CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders) {}

LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const{
    LimitedCollectorVolunteer* clone = new LimitedCollectorVolunteer(getId(), getName(), getCoolDown(), getMaxOrders());
    clone->setTimeLeft(getTimeLeft());
	clone->setOrdersLeft(getNumOrdersLeft());
    clone->activeOrderId = getActiveOrderId();
    clone->completedOrderId = getCompletedOrderId();
  
    return clone;
}

void LimitedCollectorVolunteer::setOrdersLeft(int newOrdersLeft) {
	ordersLeft = newOrdersLeft;
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const{
    return ordersLeft > 0;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const{
    return CollectorVolunteer::canTakeOrder(order) && ordersLeft > 0;
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order){
    CollectorVolunteer::acceptOrder(order);
    ordersLeft--;
}

int LimitedCollectorVolunteer::getMaxOrders() const{
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}

bool LimitedCollectorVolunteer::isLimited() const{
	return true;
}

string LimitedCollectorVolunteer::toString() const{ 
    return "Limited Collector Volunteer: " + getName() +" id " + std::to_string(getId());
}



//DriverVolunteer
DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep)
    : Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0) {}

DriverVolunteer *DriverVolunteer::clone() const{
	return new DriverVolunteer(*this);
}

void DriverVolunteer::setDistanceLeft(int newDistanceLeft) {
    distanceLeft = newDistanceLeft;
}	

int DriverVolunteer::getDistanceLeft() const{
    return distanceLeft;
}

int DriverVolunteer::getMaxDistance() const{
    return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const{
    return distancePerStep;
}

bool DriverVolunteer::decreaseDistanceLeft(){
	if(distanceLeft > 0 ){
		distanceLeft -= distancePerStep;
	}	
	if(distanceLeft <= 0){
		completedOrderId = activeOrderId;
		activeOrderId = NO_ORDER;
		return true;
	}	
	return false;
}

bool DriverVolunteer::hasOrdersLeft() const{
    return true;
}

bool DriverVolunteer::canTakeOrder(const Order &order) const{
    return !isBusy() && order.getDistance() <= maxDistance;
}

void DriverVolunteer::acceptOrder(const Order &order){
    activeOrderId = order.getId();
    distanceLeft = order.getDistance();
}

void DriverVolunteer::step(){
    if (decreaseDistanceLeft()) {
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

bool DriverVolunteer::isLimited() const{
	return false;
}

string DriverVolunteer::toString() const{ //get ready for printVolunteerStatus function ???
    return "Driver Volunteer: " + getName() +" id " + std::to_string(getId());
}



//LimitedDriverVolunteer
LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders)
    : DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders) {}

LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const{
	return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const{
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const{
    return ordersLeft > 0;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const{
    return DriverVolunteer::canTakeOrder(order) && ordersLeft > 0;
}

void LimitedDriverVolunteer::acceptOrder(const Order &order){
    DriverVolunteer::acceptOrder(order);
    ordersLeft--;
}

bool LimitedDriverVolunteer::isLimited() const{
	return true;
}

string LimitedDriverVolunteer::toString() const{ 
    return "Limited Driver Volunteer: " + getName() +" id " + std::to_string(getId());
}
