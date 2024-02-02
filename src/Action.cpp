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
BaseAction::BaseAction():errorMsg("<error_msg>"),status(ActionStatus::COMPLETED){}

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
	cout<< "error:" <<errorMsg << endl;
}//change status to error, and print

string BaseAction::getErrorMsg() const{ 
    return errorMsg;
} 


//simulate step
SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps){} //constructor

void SimulateStep::act(WareHouse &wareHouse){
	for(int i=0; i<numOfSteps; i++){
		assignVolunteer(wareHouse);
		makeTheStep(wareHouse);
	}
}

void SimulateStep::makeTheStep(WareHouse &wareHouse){
	vector<Order*> &pendingOrders = wareHouse.getPendingOrders();
    vector<Order*> &inProcessOrders = wareHouse.getinProcessOrders();
	vector<Order*> &completedOrders = wareHouse.getCompletedOrders();
	
	//treat the case of orders in process
    for (vector<Order *>::size_type i = 0; i < inProcessOrders.size(); i++) {
		cout << "iteration: " + to_string(i) << endl; //print to delete
		cout << "inProcessOrders size: " + to_string(inProcessOrders.size()) << endl; //print to delete
		Order *order = inProcessOrders[i];
		cout << "current order:" <<to_string(order->getId()) << endl; //print to delete

		// if the order is delivering
		if (order->getStatus() == OrderStatus::DELIVERING){
			cout <<to_string(order->getId()) + " delivering "<< endl; //print to delete
			int driverId = order->getDriverId();
			DriverVolunteer &driver = dynamic_cast<DriverVolunteer&> (wareHouse.getVolunteer(driverId));
			cout <<driver.getName()<< endl; //print to delete
			cout<<to_string(driver.getDistanceLeft()) +" distance left"<<endl; //print to delete
			driver.step();
			cout <<to_string(order->getId()) + " delivering2 "<< endl; //print to delete
			cout<<to_string(driver.getDistanceLeft()) +" distance left"<<endl; //print to delete
			cout<<to_string(driver.getActiveOrderId()) +" active order"<<endl; //print to delete
			if (driver.getActiveOrderId() == NO_ORDER){
				order->setStatus(OrderStatus::COMPLETED);
				completedOrders.push_back(order);
				inProcessOrders.erase(inProcessOrders.begin() + i);
				i--;
				if (!driver.hasOrdersLeft()){
					wareHouse.deleteVolunteer(driverId);
				}
			}
		}
		//if the order is collecting
		else if(order->getStatus() == OrderStatus::COLLECTING){ 
			cout <<to_string(order->getId()) + " entered_collecting "<< endl; //print to delete
            int collectorId = order->getCollectorId();
            CollectorVolunteer &collector = dynamic_cast<CollectorVolunteer&>(wareHouse.getVolunteer(collectorId));
			cout <<collector.getName()<< endl; //print to delete
			cout<<to_string(collector.getTimeLeft()) +" time left" <<endl; //print to delete
            collector.step();
			cout <<to_string(order->getId()) + " collecting2 "<< endl; //print to delete
			cout<<to_string(collector.getTimeLeft()) +" time left"<<endl; //print to delete
			cout<<to_string(collector.getActiveOrderId()) +" active order"<<endl; //print to delete
            if (collector.getActiveOrderId() == NO_ORDER) {
				cout<< "order id __ is here : __ =" << order->getId() <<endl; //print to delete
				pendingOrders.push_back(order);
				inProcessOrders.erase(inProcessOrders.begin() + i);
                i--;
				if (!collector.hasOrdersLeft()){
					cout<<"hasOrdersLeft  false"<<endl; //print to delete
					wareHouse.deleteVolunteer(collectorId);
				}
            }
            
		cout<<"hasOrdersLeft true"<<endl; //print to delete
        }	
    }
}	
			
void SimulateStep::assignVolunteer(WareHouse &wareHouse){
	vector<Order*> &pendingOrders = wareHouse.getPendingOrders();
	vector<Volunteer*> &volunteers = wareHouse.getVolunteers();
	vector<Order*> &inProcessOrders = wareHouse.getinProcessOrders();

	for (vector<Order *>::size_type i = 0; i < pendingOrders.size(); i++){
		cout << "pendingOrders size: " + to_string(pendingOrders.size()) << endl; //print to delete
		cout << "iteration: " + to_string(i) << endl; //print to delete		
		Order* order =pendingOrders[i];

		// if the order is pending for driver
		if(order->getStatus() ==  OrderStatus::COLLECTING){ 
			cout << to_string(order->getId())+ " " + order->OrderStatusToString() << endl; //print to delete
			bool isAssigned = false;
			for (vector<Volunteer *>::size_type j = 0; j< volunteers.size() && !isAssigned; j++){
				if (!((*volunteers[j]).isCollector())  && (volunteers[j]->canTakeOrder(*order))){
						(*volunteers[j]).acceptOrder(*order); 
						order->setStatus(OrderStatus::DELIVERING); 
						order->setDriverId((*volunteers[j]).getId()); 
						isAssigned = true;
						inProcessOrders.push_back(order);
						pendingOrders.erase(pendingOrders.begin() + i);// Move the order from pendingOrders to inProcessOrders
						i--;
				}
			}
			cout << to_string(order->getId())+ " " + order->OrderStatusToString() << endl; //print to delete
		}

		//if the order is pending for collector
		if(order->getStatus() ==  OrderStatus::PENDING){ 
			cout << to_string(order->getId())+ " " + order->OrderStatusToString() << endl; //print to delete
			bool isAssigned = false;
			for (vector<Volunteer *>::size_type j = 0; j < volunteers.size() && !isAssigned; j++){
				if ((*volunteers[j]).isCollector()  && (volunteers[j]->canTakeOrder(*order))){
					(*volunteers[j]).acceptOrder(*order); 
					order->setStatus(OrderStatus::COLLECTING); 
					order->setCollectorId((*volunteers[j]).getId()); 
					isAssigned = true;
					inProcessOrders.push_back(order);
					cout << "inserted to inProcess:" <<to_string(inProcessOrders.back()->getId()) << endl; //print to delete
					pendingOrders.erase(pendingOrders.begin() + i);// Move the order from pendingOrders to inProcessOrders
					i--;
				}
			}
			cout << to_string(order->getId())+ " " + order->OrderStatusToString() << endl; //print to delete
		}
	}
}
	
SimulateStep *SimulateStep::clone() const{
	return new SimulateStep(*this);
}

string SimulateStep::toString() const{
	string stringStatus= ActionStatusToString(getStatus());
	return "SimulateStep " + std::to_string(numOfSteps) + " " + stringStatus;
}


//AddOrder
AddOrder::AddOrder(int id) : customerId(id){}

void AddOrder::act(WareHouse &wareHouse){
	Customer &customer = wareHouse.getCustomer(customerId);
    if(customerId < 0)
        error("Customer doesn't exist");
    
    else if (!customer.canMakeOrder())
        error("cannot place this order");
    else {
        Order* order = new Order(wareHouse.getOrderCounter(), customerId, (wareHouse.getCustomer(customerId)).getCustomerDistance());
		customer.addOrder(order->getId());
		wareHouse.addOrder(order);
    }
} 

AddOrder *AddOrder::clone() const{
    return new AddOrder(*this);
}

string AddOrder::toString() const{ 
	string stringStatus= ActionStatusToString(getStatus());
	return "Order " + std::to_string(customerId) + " " + stringStatus;
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
} 

AddCustomer *AddCustomer::clone() const{
   return new AddCustomer(*this);
}

string AddCustomer::toString() const{
	string stringStatus= ActionStatusToString(getStatus());

    if(CustomerType(customerType)==CustomerType::Civilian){  
        return "Customer " + customerName + ", Civilian, distance:" + std::to_string(distance) + ", maxOrder: " + std::to_string(maxOrders) + " " + stringStatus;
    }
    return "Customer: " + customerName + ", Soldier, distance:" + std::to_string(distance) + ", maxOrder: " + std::to_string(maxOrders) + " " + stringStatus;
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
		else {
        	cout << "Driver: " + std::to_string(order.getDriverId())<< endl;
		}
    }
} 

PrintOrderStatus *PrintOrderStatus::clone() const{
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const{ 
	string stringStatus= ActionStatusToString(getStatus());
    return "OrderStatus " + std::to_string(orderId) + " " + stringStatus;
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
			cout<<"numOrdersLeft: " + to_string(wareHouse.getCustomer(customerId).getMaxOrders() - wareHouse.getCustomer(customerId).getNumOrders()) <<endl;
		}
		else {
			cout<<"numOrdersLeft: 0 "<<endl;
		}
	} 
}

PrintCustomerStatus *PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const{ 
	string stringStatus= ActionStatusToString(getStatus());
	return "CustomerStatus " + std::to_string(customerId) + " " +stringStatus;
}

         
//PrintVolunteerStatus
PrintVolunteerStatus :: PrintVolunteerStatus(int id) : volunteerId(id){}//constructor

void PrintVolunteerStatus::act(WareHouse &wareHouse){
	
	Volunteer &volunteer = wareHouse.getVolunteer(volunteerId);

	//if the volunteer doesnt exist
	if(volunteer.getId() == -1)
		error("Volunteer doesnt exist");
	
	//if the volunteer exist
	else {
		cout << "VolunteerID:" << std::to_string(volunteerId) << endl; 

		//if the volunteer is a collector
		if (volunteer.isCollector()){ 
			CollectorVolunteer &collector = dynamic_cast<CollectorVolunteer&>(volunteer);
			if(!collector.isBusy()){
				cout << "isBusy: false" << endl;
				cout << "OrderID: None" << endl; 	
				cout << "timeLeft: None" << endl; 
				cout << "ordersLeft: No Limit" << endl; 
			}
			else{
				cout << "isBusy: true" << endl; 
				cout << "OrderID: " + std::to_string(volunteer.getActiveOrderId())<< endl; 
				cout << "timeLeft: " + std::to_string(collector.getTimeLeft()) << endl; 
				if(collector.isLimited()){
					LimitedCollectorVolunteer &limitedCollector = dynamic_cast<LimitedCollectorVolunteer&>(volunteer);
					cout << "ordersLeft: " + std::to_string(limitedCollector.getNumOrdersLeft()) << endl;
				}	
				else
					cout << "ordersLeft: No Limit" << endl;
			}
		}	
		else{ //if the volunteer is a driver
			DriverVolunteer &driver = dynamic_cast<DriverVolunteer&>(volunteer);
			if(!driver.isBusy()){
				cout << "isBusy: false" << endl;
				cout << "OrderID: None" << endl; 	
				cout << "timeLeft: None" << endl; 
				cout << "ordersLeft: No Limit" << endl; 
			}	
			else{	
				cout << "isBusy: true" << endl; //check if bool can be printed
				cout << "OrderID: " + std::to_string(volunteer.getActiveOrderId())<< endl;
				cout << "distanceLeft: " + std::to_string(driver.getDistanceLeft()) << endl; 
				if (driver.isLimited()){
					LimitedDriverVolunteer &limitedDriver = dynamic_cast<LimitedDriverVolunteer&>(volunteer);
					cout << "ordersLeft: " + std::to_string(limitedDriver.getNumOrdersLeft()) << endl;
				}
				else
					cout << "ordersLeft: No Limit" << endl;
			}
		}	
	}
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const{
	return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const{
	string stringStatus = ActionStatusToString(getStatus());
	return "VolunteerStatus " + std::to_string(volunteerId) + " " + stringStatus;
}


//PrintActionsLog
PrintActionsLog::PrintActionsLog(){}//constructor

void PrintActionsLog::act(WareHouse &wareHouse){
	vector<BaseAction*> actionsLog = wareHouse.getActions();
	for (BaseAction* action : actionsLog) 
    	cout << action->toString() << endl;
} 

PrintActionsLog *PrintActionsLog::clone() const{
	return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const{
	string stringStatus = ActionStatusToString(getStatus());
	return "log " + stringStatus; 
}


//close
Close::Close(){}//constructor

void Close::act(WareHouse &wareHouse){ 
    printAll(wareHouse);
	wareHouse.close();
}

void Close::printAll(WareHouse &wareHouse){

    for (auto order : wareHouse.getPendingOrders()){
        cout << to_string(order->getId())+" "+ to_string(order->getCustomerId())+ " " + order->OrderStatusToString() << endl;
    }
    for (auto order : wareHouse.getinProcessOrders()){
        cout << to_string(order->getId())+" "+ to_string(order->getCustomerId())+ " " + order->OrderStatusToString() << endl;
    }
    for (auto order : wareHouse.getCompletedOrders()){
        cout << to_string(order->getId())+" "+ to_string(order->getCustomerId())+" " + order->OrderStatusToString() << endl;
    }
}

Close *Close::clone() const{
	return new Close(*this);
}

string Close::toString() const{
	return "close ";
}


extern WareHouse* backup;
//backup warehouse
BackupWareHouse::BackupWareHouse(){
}//constructor

void BackupWareHouse::act(WareHouse &wareHouse){
    if(backup == nullptr)
        backup =  new WareHouse(wareHouse);
    else
        *backup = wareHouse;
}//copy constructor

BackupWareHouse *BackupWareHouse::clone() const {
	return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const {
	string stringStatus = ActionStatusToString(getStatus());
	return "BackupWareHouse " + stringStatus;
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
	return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const{
	string stringStatus = ActionStatusToString(getStatus());
	return "RestoreWareHouse " + stringStatus;
}

