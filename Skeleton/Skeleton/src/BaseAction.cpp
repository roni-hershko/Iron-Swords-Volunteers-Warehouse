#include "../include/BaseAction.h"

Order::Order(int id) : BaseAction(), customerId(id) {}

void Order::act(WareHouse &wareHouse) {
    // Implementation of Order action
}

Order* Order::clone() const {
    return new Order(*this);
}

string Order::toString() const {
    // Implementation of toString for Order action
}

AddCustomer::AddCustomer(string customerName, string customerType, int distance, int maxOrders)
    : BaseAction(), customerName(customerName), customerType(customerType), distance(distance), maxOrders(maxOrders) {}

void AddCustomer::act(WareHouse &wareHouse) {
    // Implementation of AddCustomer action
}

AddCustomer* AddCustomer::clone() const {
    return new AddCustomer(*this);
}

string AddCustomer::toString() const {
    // Implementation of toString for AddCustomer action
}

PrintOrderStatus::PrintOrderStatus(int id) : BaseAction(), orderId(id) {}

void PrintOrderStatus::act(WareHouse &wareHouse) {
    // Implementation of PrintOrderStatus action
}

PrintOrderStatus* PrintOrderStatus::clone() const {
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const {
    // Implementation of toString for PrintOrderStatus action
}

PrintCustomerStatus::PrintCustomerStatus(int customerId) : BaseAction(), customerId(customerId) {}

void PrintCustomerStatus::act(WareHouse &wareHouse) {
    // Implementation of PrintCustomerStatus action
}

PrintCustomerStatus* PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const {
    // Implementation of toString for PrintCustomerStatus action
}

PrintVolunteerStatus::PrintVolunteerStatus(int id) : BaseAction(), VolunteerId(id) {}

void PrintVolunteerStatus::act(WareHouse &wareHouse) {
    // Implementation of PrintVolunteerStatus action
}

PrintVolunteerStatus* PrintVolunteerStatus::clone() const {
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const {
    // Implementation of toString for PrintVolunteerStatus action
}

PrintActionsLog::PrintActionsLog() : BaseAction() {}

void PrintActionsLog::act(WareHouse &wareHouse) {
    // Implementation of PrintActionsLog action
}

PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const {
    // Implementation of toString for PrintActionsLog action
}

Close::Close() : BaseAction() {}

void Close::act(WareHouse &wareHouse) {
    // Implementation of Close action
}

string Close::toString() const {
    // Implementation of toString for Close action
}

BackupWareHouse::BackupWareHouse() : BaseAction() {}

void BackupWareHouse::act(WareHouse &wareHouse) {
    // Implementation of BackupWareHouse action
}

BackupWareHouse* BackupWareHouse::clone() const {
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const {
    // Implementation of toString for BackupWareHouse action
}

RestoreWareHouse::RestoreWareHouse() : BaseAction() {}

void RestoreWareHouse::act(WareHouse &wareHouse) {
    // Implementation of RestoreWareHouse action
}

RestoreWareHouse* RestoreWareHouse::clone() const {
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const {
    // Implementation of toString for RestoreWareHouse action
}
