#include "../include/Action.h"
#include <string>
#include <vector>
#include "../include/WareHouse.h"
#include "../include/Order.h"
#include "../include/Volunteer.h"
#include "../include/Customer.h"
using std::string;
using std::vector;

#include <iostream>
using namespace std;

//base action
BaseAction::BaseAction():errorMsg("<error_msg>"),status(ActionStatus::ERROR){}

ActionStatus BaseAction::getStatus() const{
	return status;
}

string BaseAction::ActionStatusToString(ActionStatus status) const{
	if(status==ActionStatus::COMPLETED)
		return "COMPLETED";
	return "ERROR";
}

void BaseAction::complete(){
    status=ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg){
    status=ActionStatus::ERROR;
    this->errorMsg=errorMsg;
	cout<<errorMsg<<endl;
}//change status to error, and print

string BaseAction::getErrorMsg() const{ 
    return errorMsg;
} 


//simulate step
SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps){} //constructor

void SimulateStep::act(WareHouse &wareHouse){
	for(int i=0; i<numOfSteps; i++){
		makeTheStep(wareHouse);
	}
	wareHouse.addAction(this);
    complete();
}

void SimulateStep::makeTheStep(WareHouse &wareHouse){
	vector<Order*> &pendingOrders = wareHouse.getPendingOrders();
    vector<Order*> &inProcessOrders = wareHouse.getinProcessOrders();
	vector<Order*> &completedOrders = wareHouse.getCompletedOrders();
	
	//treat the case of pending orders
	assignVolunteer(wareHouse);


	int counter=0;
	//treat the case of orders in process
	for (Order *order : inProcessOrders){
		// if the order is delivering
		if (order->getStatus() == OrderStatus::DELIVERING){
			int driverId = order->getDriverId();
			//the volunteer is a driver
			DriverVolunteer &driver = dynamic_cast<DriverVolunteer&> (wareHouse.getVolunteer(driverId));
			driver.step();
			// if the order has finished delivering move to completedOrders vector
			if (driver.getActiveOrderId() == NO_ORDER){
				order->setStatus(OrderStatus::COMPLETED);
				completedOrders.push_back(order);
				inProcessOrders.erase(inProcessOrders.begin() + counter);
				counter--;
			}
			if (!driver.hasOrdersLeft())
				wareHouse.deleteVolunteer(driverId);

		}
		//if the order is collecting
		else if(order->getStatus() == OrderStatus::COLLECTING){ 
            int collectorId = order->getCollectorId();
            CollectorVolunteer &collector = dynamic_cast<CollectorVolunteer&>(wareHouse.getVolunteer(collectorId));
			cout<<to_string(collector.getTimeLeft())<<endl;
            collector.step(); //step the collector
			// if order has finished collecting move to pendingOrders vector
			cout<<to_string(collector.getTimeLeft())<<endl;
            if (collector.getActiveOrderId() == NO_ORDER) {
				//no need to change the order status untill the driver will take the order only happen in the next step
				pendingOrders.push_back(order);
				inProcessOrders.erase(inProcessOrders.begin() + counter);
                counter--;
                
            }
			// if the collector is limitedCollector and has no orders left delete the collector
            if (!collector.hasOrdersLeft()) 
                wareHouse.deleteVolunteer(collectorId);
            
        }
	counter++;
    }
}	
			
void SimulateStep::assignVolunteer(WareHouse &wareHouse){
	vector<Order*> &pendingOrders = wareHouse.getPendingOrders();
	vector<Volunteer*> &volunteers = wareHouse.getVolunteers();
	vector<Order*> &inProcessOrders = wareHouse.getinProcessOrders();
	int sizeVolunteers = volunteers.size();

	for (vector<Order *>::size_type i = 0; i < pendingOrders.size(); i++){
		if(pendingOrders[i]->getStatus() ==  OrderStatus::PENDING){ //if the order is pending for collector
			bool isAssigned = false;
			for (vector<Volunteer *>::size_type j = 0; j < volunteers.size() && !isAssigned; j++){
				if ((*volunteers[j]).isCollector()  && (volunteers[j]->canTakeOrder(*pendingOrders[i]))){
					(*volunteers[j]).acceptOrder(*pendingOrders[i]); //accept the order
					(*pendingOrders[i]).setStatus(OrderStatus::COLLECTING); //change the order status
					(*pendingOrders[i]).setCollectorId((*volunteers[j]).getId()); //change the order collector id
					isAssigned = true;
					inProcessOrders.push_back(pendingOrders[i]);
					pendingOrders.erase(pendingOrders.begin() + i);// Move the order from pendingOrders to inProcessOrders
					i--;
				}
			}
		}
	
		else if(pendingOrders[i]->getStatus() ==  OrderStatus::COLLECTING){ //if the order is pending for driver
			bool isAssigned = false;
			for (int j = 0; j< sizeVolunteers && !isAssigned; j++){
				if (!(*volunteers[j]).isCollector()  && (volunteers[j]->canTakeOrder(*pendingOrders[i]))){
						(*volunteers[j]).acceptOrder(*pendingOrders[i]); 
						(*pendingOrders[i]).setStatus(OrderStatus::DELIVERING); 
						(*pendingOrders[i]).setDriverId((*volunteers[j]).getId()); 
						isAssigned = true;
						inProcessOrders.push_back(pendingOrders[i]);
						pendingOrders.erase(pendingOrders.begin() + i);// Move the order from pendingOrders to inProcessOrders
						i--;
				}
			}

		}
	}
}
	
SimulateStep *SimulateStep::clone() const{
	return new SimulateStep(numOfSteps);
}

string SimulateStep::toString() const{
	string stringStatus= ActionStatusToString(getStatus());
	return "SimulateStep " + std::to_string(numOfSteps) + stringStatus;
}


//AddOrder
AddOrder::AddOrder(int id) : customerId(id){}

void AddOrder::act(WareHouse &wareHouse){
	Customer &customer = wareHouse.getCustomer(customerId);
    if(customer.getId() == -1)
        error("Customer doesn't exist");
    
    else if (!customer.canMakeOrder())
        error("cannot place this order");
    else {
        Order* order = new Order(wareHouse.getOrderCounter(), customerId, (wareHouse.getCustomer(customerId)).getCustomerDistance());
		customer.addOrder(order->getId());
		wareHouse.addOrder(order);
        complete();
    }
    wareHouse.addAction(this);
} //add order to warehouse, maybe error

AddOrder *AddOrder::clone() const{
    return new AddOrder(customerId);
}

string AddOrder::toString() const{ 
	string stringStatus= ActionStatusToString(getStatus());
	return "Order " + std::to_string(customerId) + stringStatus;
}


//add customer
AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders) :
 customerName(customerName),customerType(StringToCustomerType(customerType)), distance(distance), maxOrders(maxOrders) {} //constructor    

void AddCustomer::act(WareHouse &wareHouse){
    int customerId=wareHouse.getCustomerCounter();
    if(CustomerType(customerType)==CustomerType::Civilian){
        wareHouse.addCustomer(new CivilianCustomer(customerId ,customerName, distance, maxOrders));
    }
    else {
        wareHouse.addCustomer(new SoldierCustomer (customerId,customerName, distance, maxOrders));
    }
	complete();   
    wareHouse.addAction(this);
} //add customer to warehouse, never error

AddCustomer *AddCustomer::clone() const{
    if(customerType == CustomerType::Civilian){
       return new AddCustomer(customerName, "Civilian",  distance,  maxOrders);
    }
    return new AddCustomer(customerName, "Soldier",  distance,  maxOrders);
}

string AddCustomer::toString() const{
	string stringStatus= ActionStatusToString(getStatus());

    if(CustomerType(customerType)==CustomerType::Civilian){  
        return "AddCustomer " + customerName + ", Civilian, distance:" + std::to_string(distance) + ", maxOrder: " + std::to_string(maxOrders) + stringStatus;
    }
    return "Customer: " + customerName + ", Soldier, distance:" + std::to_string(distance) + ", maxOrder: " + std::to_string(maxOrders) + stringStatus;
}

CustomerType AddCustomer::StringToCustomerType(string customerType){ 
	if(customerType=="Civilian")
		return CustomerType::Civilian;
	return CustomerType::Soldier;
}


//PrintOrderStatus
PrintOrderStatus::PrintOrderStatus(int id) : orderId(id){}

void PrintOrderStatus::act(WareHouse &wareHouse){
	Order &order = wareHouse.getOrder(orderId);
    if(order.getId() == -1)
        error("Order doesn't exist");
    else{
        cout << "OrderId: " + std::to_string(order.getId()) << endl;
		cout << "OrderStatus: " + order.OrderStatusToString() << endl; 
        cout << "CustomerId: " + std::to_string(order.getCustomerId()) << endl; 
		if(order.getCollectorId()==-1){
			cout << "Collector: None" << endl;
		}
		else {
        	cout << "Collector: " + std::to_string(order.getCollectorId()) << endl;
		}
		if(order.getDriverId()==-1){
			cout << "Driver: None" << endl;
		}
		else{
        	cout << "Driver: " + std::to_string(order.getDriverId())<< endl;
		}
    }
    complete();
    wareHouse.addAction(this);
} 

PrintOrderStatus *PrintOrderStatus::clone() const{
    return new PrintOrderStatus(orderId);
}

string PrintOrderStatus::toString() const{ 
	string stringStatus= ActionStatusToString(getStatus());
    return "OrderStatus " + std::to_string(orderId) + stringStatus;
}  


//PrintCustomerStatus
PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId){}//constructor

void PrintCustomerStatus::act(WareHouse &wareHouse){
	Customer &customer = wareHouse.getCustomer(customerId);
	if(customer.getId() == -1){
		error("Customer doesn't exist");
	}
	else{
		cout<<"CustomerID: "+ std::to_string(customerId) <<endl;
		vector<int> orderList = wareHouse.getCustomer(customerId).getOrdersIds();
		for(int i=0; i < wareHouse.getCustomer(customerId).getNumOrders(); i++){
			cout<< "orderID: " + to_string(orderList[i]) <<endl;
			cout<< "orderStatus: " + wareHouse.getOrder(orderList[i]).OrderStatusToString() <<endl;
		}
		if(wareHouse.getCustomer(customerId).canMakeOrder()){
			cout<<"num order left: " + to_string(wareHouse.getCustomer(customerId).getMaxOrders() - wareHouse.getCustomer(customerId).getNumOrders()) <<endl;
		}
		else {
			cout<<"num order left: 0 "<<endl;
		}
		complete();
	} 
    wareHouse.addAction(this);
}

PrintCustomerStatus *PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(customerId);
}

string PrintCustomerStatus::toString() const{ 
	string stringStatus= ActionStatusToString(getStatus());
	return "CustomerStatus " + std::to_string(customerId) + stringStatus;
}

         
//PrintVolunteerStatus
PrintVolunteerStatus :: PrintVolunteerStatus(int id) : volunteerId(id){}//constructor

void PrintVolunteerStatus::act(WareHouse &wareHouse){
	
	Volunteer &volunteer = wareHouse.getVolunteer(volunteerId);

	if(volunteer.getId() == -1){
		error("Volunteer doesnt exist");
	}

	else {
		cout << "VolunteerID:" << std::to_string(volunteerId) << endl; 
		if(!volunteer.isBusy()){
			cout << "isBusy: false" << endl;
			cout << "OrderID: None" << endl; 
			cout << "timeLeft: None" << endl; 
			cout << "ordersLeft: None" << endl; 
		}
		else if (volunteer.isCollector()){ //if the volunteer is a collector
			CollectorVolunteer &collector = dynamic_cast<CollectorVolunteer&>(volunteer);
			cout << "isBusy: true" << endl; //check if bool can be printed
			cout << "OrderID: " + std::to_string(volunteer.getActiveOrderId())<< endl; 
			cout << "timeLeft: " + std::to_string(collector.getTimeLeft()) << endl; 
			if(collector.isLimited()){
				LimitedCollectorVolunteer &limitedCollector = dynamic_cast<LimitedCollectorVolunteer&>(volunteer);
				cout << "ordersLeft: " + std::to_string(limitedCollector.getNumOrdersLeft()) << endl;
			}
			else{
				cout << "ordersLeft: No Limit" << endl;
			}
		}	
		else { //if the volunteer is a driver
			DriverVolunteer &driver = dynamic_cast<DriverVolunteer&>(volunteer);
			cout << "isBusy: true" << endl; //check if bool can be printed
			cout << "OrderID: " + std::to_string(volunteer.getActiveOrderId())<< endl;
			cout << "distanceLeft: " + std::to_string(driver.getDistanceLeft()) << endl; 
			if (driver.isLimited()){
				LimitedDriverVolunteer &limitedDriver = dynamic_cast<LimitedDriverVolunteer&>(volunteer);
				cout << "ordersLeft: " + std::to_string(limitedDriver.getNumOrdersLeft()) << endl;
			}
			else{
				cout << "ordersLeft: No Limit" << endl;
			}
		}
		complete();
	}	
    wareHouse.addAction(this);
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const{
	return new PrintVolunteerStatus(volunteerId);
}

string PrintVolunteerStatus::toString() const{
	string stringStatus = ActionStatusToString(getStatus());
	return "PrintVolunteerStatus" + std::to_string(volunteerId) + stringStatus;
}


//PrintActionsLog
PrintActionsLog::PrintActionsLog(){}//constructor

void PrintActionsLog::act(WareHouse &wareHouse){
	for (const auto& action : wareHouse.getActions()) {
    	cout << action->toString() << endl;
	} 

	complete();
    wareHouse.addAction(this);
} 


PrintActionsLog *PrintActionsLog::clone() const{
	return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const{
	string stringStatus = ActionStatusToString(getStatus());
	return "log" + stringStatus; 
}


//close
Close::Close(){}//constructor

void Close::act(WareHouse &wareHouse){ 
    printAll(wareHouse);
	wareHouse.close();
}

void Close::printAll(WareHouse &wareHouse){

    for (auto order : wareHouse.getPendingOrders()){
        cout << to_string(order->getId())+" "+ to_string(order->getCustomerId())+" pendeing" << endl;
    }
    for (auto order : wareHouse.getinProcessOrders()){
        cout << to_string(order->getId())+" "+ to_string(order->getCustomerId())+" inProcess" << endl;
    }
    for (auto order : wareHouse.getCompletedOrders()){
        cout << to_string(order->getId())+" "+ to_string(order->getCustomerId())+" complete" << endl;
    }
}

Close *Close::clone() const{
	return new Close();
}

string Close::toString() const{
	return "close";
}

extern WareHouse* backup;
//backup warehouse
BackupWareHouse::BackupWareHouse(){}//constructor

void BackupWareHouse::act(WareHouse &wareHouse){
    backup= &wareHouse;
}//copy assignment operator

BackupWareHouse *BackupWareHouse::clone() const {
	return new BackupWareHouse();
}

string BackupWareHouse::toString() const {
	return "BackupWareHouse";
}


//restore warehouse
RestoreWareHouse::RestoreWareHouse(){}//constructor

void RestoreWareHouse::act(WareHouse &wareHouse){
if(backup == nullptr)
	error("No backup available");
else
	wareHouse = *backup; //move assignment opertor
}

RestoreWareHouse *RestoreWareHouse::clone() const{
	return new RestoreWareHouse();
}

string RestoreWareHouse::toString() const{
	return "RestoreWareHouse";
}

