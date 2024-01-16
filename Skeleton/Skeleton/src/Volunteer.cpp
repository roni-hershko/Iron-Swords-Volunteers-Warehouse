#include "../Volunteer.h"

Volunteer::Volunteer(int id, const string& name) : id(id), name(name), completedOrderId(NO_ORDER), activeOrderId(NO_ORDER) {}

int Volunteer::getId() const {
    return id;
}

const string& Volunteer::getName() const {
    return name;
}

int Volunteer::getActiveOrderId() const {
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const {
    return completedOrderId;
}

bool Volunteer::isBusy() const {
    return activeOrderId != NO_ORDER;
}

CollectorVolunteer::CollectorVolunteer(int id, string name, int coolDown)
    : Volunteer(id, name), coolDown(coolDown), timeLeft(0) {}

CollectorVolunteer* CollectorVolunteer::clone() const {
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step() {
    if (timeLeft > 0) {
        timeLeft--;
        if (timeLeft == 0) {
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }
}

int CollectorVolunteer::getCoolDown() const {
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const {
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown() {
    if (timeLeft > 0) {
        timeLeft--;
        if (timeLeft == 0) {
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
        return true;
    }
    return false;
}

bool CollectorVolunteer::hasOrdersLeft() const {
    return false; //always false for CollectorVolunteer
}

bool CollectorVolunteer::canTakeOrder(const Order& order) const {
    return !isBusy();
}

void CollectorVolunteer::acceptOrder(const Order& order) {
    activeOrderId = order.getId();
    timeLeft = coolDown;
}

string CollectorVolunteer::toString() const {
    return "Collector Volunteer: " + getName();
}

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown, int maxOrders)
    : CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders) {}

LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const {
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const {
    return ordersLeft > 0;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order& order) const {
    return CollectorVolunteer::canTakeOrder(order) && ordersLeft > 0;
}

void LimitedCollectorVolunteer::acceptOrder(const Order& order) {
    CollectorVolunteer::acceptOrder(order);
    ordersLeft--;
}

int LimitedCollectorVolunteer::getMaxOrders() const {
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
}

string LimitedCollectorVolunteer::toString() const {
    return "Limited Collector Volunteer: " + getName();
}

DriverVolunteer::DriverVolunteer(int id, string name, int maxDistance, int distancePerStep)
    : Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0) {}

DriverVolunteer* DriverVolunteer::clone() const {
    return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const {
    return distanceLeft;
}

int DriverVolunteer::getMaxDistance() const {
    return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const {
    return distancePerStep;
}

bool DriverVolunteer::decreaseDistanceLeft() {
    if (distanceLeft > 0) {
        distanceLeft -= distancePerStep;
        if (distanceLeft <= 0) {
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
        return true;
    }
    return false;
}

bool DriverVolunteer::hasOrdersLeft() const {
    return true;
}

bool DriverVolunteer::canTakeOrder(const Order& order) const {
    return !isBusy() && order.getDistance() <= maxDistance;
}

void DriverVolunteer::acceptOrder(const Order& order) {
    activeOrderId = order.getId();
    distanceLeft = maxDistance;
}

void DriverVolunteer::step() {
    if (distanceLeft > 0) {
        decreaseDistanceLeft();
    }
}

string DriverVolunteer::toString() const {
    return "Driver Volunteer: " + getName();
}

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string& name, int maxDistance, int distancePerStep, int maxOrders)
    : DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders) {}

LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const {
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const {
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const {
    return ordersLeft > 0;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order& order) const {
    return DriverVolunteer::canTakeOrder(order) && ordersLeft > 0;
}

void LimitedDriverVolunteer::acceptOrder(const Order& order) {
    DriverVolunteer::acceptOrder(order);
    ordersLeft--;
}

string LimitedDriverVolunteer::toString() const {
    return "Limited Driver Volunteer: " + getName();
}
