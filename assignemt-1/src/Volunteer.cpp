#include "../include/Volunteer.h"

Volunteer::Volunteer(int id, const string &name) : id(id), name(name), completedOrderId(NO_ORDER), activeOrderId(NO_ORDER) {}

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






//CollectorVolunteer
CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown) 
	: Volunteer(id, name), coolDown(coolDown), timeLeft(0) {}

CollectorVolunteer *CollectorVolunteer::clone() const {
	return new CollectorVolunteer(*this);
}


//rule of 3 rulle of 5??????
// Destructor
CollectorVolunteer::~CollectorVolunteer() {}
// copy constructor
CollectorVolunteer ::CollectorVolunteer(const CollectorVolunteer &other) 
	: Volunteer(other), coolDown(other.coolDown), timeLeft(other.timeLeft) {}
// assignment operator
CollectorVolunteer &CollectorVolunteer::operator=(const CollectorVolunteer &other){
    if (this != &other){
        Volunteer::operator=(other);
        coolDown = other.coolDown;
        timeLeft = other.timeLeft;
    }
    return *this;
}
// move constructor
CollectorVolunteer::CollectorVolunteer(CollectorVolunteer &&other) noexcept 
	: Volunteer(std::move(other)), coolDown(std::move(other.coolDown)), timeLeft(std::move(other.timeLeft)) {}
// move assignment operator
CollectorVolunteer &CollectorVolunteer::operator=(CollectorVolunteer &&other) noexcept{
    if (this != &other){
        Volunteer::operator=(std::move(other));
        coolDown = std::move(other.coolDown);
        timeLeft = std::move(other.timeLeft);
    }
    return *this;
}


void CollectorVolunteer::step(){
    if (timeLeft > 0){
        timeLeft--;
        if (timeLeft == 0){
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }
}

int CollectorVolunteer::getCoolDown() const{
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const{
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown(){
    if (timeLeft > 0){
        timeLeft--;
        if (timeLeft == 0){
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
        return true;
    }
    return false;
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

string CollectorVolunteer::toString() const{ //get ready for printVolunteerStatus function ???
    return "Collector Volunteer: " + getName();
}





//LimitedCollectorVolunteer
LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown, int maxOrders)
    : CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders) {}

LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const{
    return new LimitedCollectorVolunteer(*this);
}
 //rule of 3 rulle of 5??????


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

string LimitedCollectorVolunteer::toString() const{ //get ready for printVolunteerStatus function ???
    return "Limited Collector Volunteer: " + getName();
}





//DriverVolunteer
DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep)
    : Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0) {}

DriverVolunteer *DriverVolunteer::clone() const{
    return new DriverVolunteer(*this);
}

//rulle of 3 rulle of 5??????
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
    if (distanceLeft > 0){
        distanceLeft -= distancePerStep;
        if (distanceLeft <= 0){
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
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
    distanceLeft = maxDistance;
}

void DriverVolunteer::step(){
    if (distanceLeft > 0){
        distanceLeft -= distancePerStep;
        if (distanceLeft <= 0){
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }
}

string DriverVolunteer::toString() const{ //get ready for printVolunteerStatus function ???
    return "Driver Volunteer: " + getName();
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

string LimitedDriverVolunteer::toString() const{ //get ready for printVolunteerStatus function ???
    return "Limited Driver Volunteer: " + getName();
}
