#pragma once
#include <string>
#include <vector>
using namespace std;

#include "Order.h"
#include "Customer.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers and Actions.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        const vector<BaseAction*> &getActionsLog() const;
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        void addVolunteer(Volunteer* volunteer); //new method that adds a volunteer to the warehouse
        void addCustomer(Customer* customer); //new method that adds a customer to the warehouse
        void printActionsLogs();
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        void close();
        void open();
        int getCustomerCounter();
        int getVolunteerCounter();
        // maybe not needed
        vector<Order*> getPendingOrders(); //new method that returns the pending orders
        vector<Order*> getCompletedOrders(); //new method that returns the completed orders
        vector<Order*> getVol(); //new method that returns the orders that are been worked by volunteers

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> vol;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
};