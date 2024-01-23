#pragma once
#include <string>
#include <vector>

#include "Order.h"
#include "Customer.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        void addVolunteer(Volunteer* volunteer); //new method that adds a volunteer to the warehouse
        void addCustomer(Customer* customer); //new method that adds a customer to the warehouse
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();
        int getCustomerCounter();
        int getVolunteerCounter();
        int getOrderCounter();//new metod that returns the order counter
        // maybe not needed
        vector<Order*> getPendingOrders(); //new method that returns the pending orders
        vector<Order*> getCompletedOrders(); //new method that returns the completed orders
        vector<Order*> getinProcessOrders(); //new method that returns the orders that are been worked by volunteers

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter=0; //For assigning unique customer IDs
        int volunteerCounter=0; //For assigning unique volunteer IDs
        int orderCounter=0; //For assigning unique order IDs, new field
};