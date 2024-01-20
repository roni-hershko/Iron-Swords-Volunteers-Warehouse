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
    

    const vector<int>& Customer::getOrdersIds() const { // need to check!!!!!!!!!
        const vector<int> copyOrdersId=ordersId;
        return copyOrdersId;   
     }
    
    int Customer::addOrder(int orderId){
        if(canMakeOrder()){
            ordersId.push_back(orderId);
            return orderId;
        }
        return -1;
    }; //return OrderId if order was added successfully, -1 otherwise
        
    // Copy Constructor 
    SoldierCustomer* SoldierCustomer::clone() const {
         SoldierCustomer* newCcus= new SoldierCustomer(getId(), getName(), getCustomerDistance(), getMaxOrders());
        for(int i=0; i<getOrdersIds().size(); i++){
            newCcus->addOrder(getOrdersIds()[i]);
        }
        return newCcus;
    }

    // Copy Constructor
    CivilianCustomer* CivilianCustomer::clone() const{
       CivilianCustomer* newCcus= new CivilianCustomer(getId(), getName(), getCustomerDistance(), getMaxOrders());
        for(int i=0; i<getOrdersIds().size(); i++){
            newCcus->addOrder(getOrdersIds()[i]);
        }
        return newCcus;
    }
        

// Destructor
Customer::~Customer() {}

// Move Constructor
Customer::Customer(Customer &&other) noexcept : 
    id(std::move(other.id)), name(std::move(other.name)), locationDistance(std::move(other.locationDistance)), maxOrders(std::move(other.maxOrders)), ordersId(std::move(other.ordersId)) {}

// Move Assignment Operator and assignment operator- not possible because of const members
        
        