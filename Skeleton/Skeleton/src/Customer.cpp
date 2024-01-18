#include <string>
#include <vector>
#include <Customer.h>
using std::string;
using std::vector;


    Customer::Customer(int id, const std::string& name, int locationDistance, int maxOrders):
        id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders){
        }

    const std::string &Customer::getName() const {return name;}

    int Customer::getId() const {return id;}
    
    int Customer::getCustomerDistance() const {return locationDistance;}
    
    int Customer::getMaxOrders() const {return maxOrders;} //Returns maxOrders
    
    int Customer::getNumOrders() const {return ordersId.size();} //Returns num of orders the customer has made so far
    
    bool Customer::canMakeOrder() const {//Returns true if the customer didn't reach max orders
        if(getNumOrders()<maxOrders)
            return true;
         return false;
    } 
    
    vector<int>& Customer::getOrders() const { // need to check!!!!!!!!!
        return ordersId;
    }
    
    int Customer::addOrder(int orderId){
        if(ordersId.size() < maxOrders){
            ordersId.push_back(orderId);
            return orderId;
        }
        return -1;
    }; //return OrderId if order was added successfully, -1 otherwise
        

    SoldierCustomer* SoldierCustomer::clone() const {
        SoldierCustomer* solCus = new SoldierCustomer(getId(), getName(), getCustomerDistance(), getMaxOrders());
        return solCus;
    }

    SoldierCustomer& SoldierCustomer::operator=(const SoldierCustomer &other){
        if(this != &other){
            this->id = other.id;
            this->name = other.name;
            this->locationDistance = other.locationDistance;
            this->maxOrders = other.maxOrders;
            this->ordersId = other.ordersId;
        }
        return *this;
    }


    CivilianCustomer* CivilianCustomer::clone() const{
        CivilianCustomer* civCus = new CivilianCustomer(getId(), getName(), getCustomerDistance(), getMaxOrders());
        return civCus;
    }
        
