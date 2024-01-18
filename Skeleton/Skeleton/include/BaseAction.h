#pragma once
#include <string>
#include <vector>
#include "WareHouse.h"
using std::string;
using std::vector;

enum class ActionStatus{
    COMPLETED, ERROR
};

enum class CustomerType{
    Soldier, Civilian
};


class Customer;

class BaseAction{
    public:
        BaseAction();
        ActionStatus getStatus() const;
        virtual void act(WareHouse& wareHouse)=0;
        virtual string toString() const=0;
        virtual BaseAction* clone() const=0;

    protected:
        void complete(); //chage status to completed, if it was completed
        void error(string errorMsg); //change status to error, and print
        string getErrorMsg() const; //return the error message

    private:
        string errorMsg;
        ActionStatus status;
};

class SimulateStep : public BaseAction {

    public:
        SimulateStep(int numOfSteps);//constructor
        void act(WareHouse &wareHouse) override;//simulate numOfSteps steps
        std::string toString() const override;
        SimulateStep *clone() const override;

    private:
        const int numOfSteps;
};

class Order : public BaseAction {
    public:
        Order(int id); //constructor
        void act(WareHouse &wareHouse) override; //add order to warehouse, maybe error
        string toString() const override;
        Order *clone() const override;
    private:
        const int customerId;
};


class AddCustomer : public BaseAction {
    public:
        AddCustomer(string customerName, string customerType, int distance, int maxOrders);//constructor
        void act(WareHouse &wareHouse) override; //add customer to warehouse, never error
        AddCustomer *clone() const override;
        string toString() const override;
    private:
        const string customerName;
        const CustomerType customerType;
        const int distance;
        const int maxOrders;
};



class PrintOrderStatus : public BaseAction {
    public:
        PrintOrderStatus(int id);//constructor
        void act(WareHouse &wareHouse) override;//print order status, maybe error   
        PrintOrderStatus *clone() const override;
        string toString() const override;
    private:
        const int orderId;
};

class PrintCustomerStatus: public BaseAction {
    public:
        PrintCustomerStatus(int customerId);   //constructor
        void act(WareHouse &wareHouse) override;//print customer status, maybe error
        PrintCustomerStatus *clone() const override;
        string toString() const override;
    private:
        const int customerId;
};


class PrintVolunteerStatus : public BaseAction {
    public:
        PrintVolunteerStatus(int id);//constructor
        void act(WareHouse &wareHouse) override; //print volunteer status, maybe error
        PrintVolunteerStatus *clone() const override;
        string toString() const override;
    private:
        const int VolunteerId;
};


class PrintActionsLog : public BaseAction {
    public:
        PrintActionsLog();//constructor
        void act(WareHouse &wareHouse) override;//print actions log, never error
        PrintActionsLog *clone() const override;
        string toString() const override;
    private:
};

class Close : public BaseAction {
    public:
        Close();//constructor
        void act(WareHouse &wareHouse) override;//close warehouse, never error
        string toString() const override;
    private:
};

class BackupWareHouse : public BaseAction {
    public:
        BackupWareHouse();//constructor
        void act(WareHouse &wareHouse) override;//backup warehouse, never error, if has one already, just replace pointer never error
        BackupWareHouse *clone() const override;
        string toString() const override;
    private:
};


class RestoreWareHouse : public BaseAction {
    public:
        RestoreWareHouse();//constructor
        void act(WareHouse &wareHouse) override;//change warehouse to the backup one, never error
        RestoreWareHouse *clone() const override;
        string toString() const override;
    private:
};