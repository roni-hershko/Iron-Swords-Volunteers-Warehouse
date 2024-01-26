#include "../include/Action.h"
#include <string>
#include <vector>
#include "../include/WareHouse.h"
#include "../include/Order.h"
using std::string;
using std::vector;


//base action
BaseAction::BaseAction():errorMsg("<error_msg>"),status(ActionStatus::ERROR){}

void BaseAction::complete(){
    status=ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg){
    status=ActionStatus::ERROR;
    this->errorMsg=errorMsg;
}//change status to error, and print

string BaseAction::getErrorMsg() const{ // מה הERROE MSG
    return errorMsg;
} 


//simulate step
SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps){} //constructor

void SimulateStep::act(WareHouse &wareHouse){
	for(int i=0; i<numOfSteps; i++){
		assignVolunteer(wareHouse);
		makeTheStep(wareHouse);
	}
	wareHouse.addAction(this);
	completed();
			

}
void SimulateStep::assignVolunteer(WareHouse &wareHouse){
	for(int i=0; i<WareHouse.getPendingOrders.size(); i++){
		if(WareHouse.getPendingOrders()[i].getOrderStatus() == OrderStatus::PENDING){ //if the order is pending fot collector
			for(int j=0; j<WareHouse.getVolunteers().size(); j++){ //go over the volunteers
			}
		}
	}
}
void SimulateStep::makeTheStep(WareHouse &wareHouse){  

}
SimulateStep *SimulateStep::clone() const{
	return new SimulateStep(numOfSteps);
}

string SimulateStep::toString() const{
	return "SimulateStep " + numOfSteps;
}


//AddOrder
AddOrder::AddOrder(int id) : customerId(id){} //constructor

void AddOrder::act(WareHouse &wareHouse){
    if(wareHouse.getCustomer(customerId) == nullptr || &wareHouse.getCustomer(customerId) == NULL){
        error("Customer doesn't exist");
    }
    else if (wareHouse.getCustomer(customerId).addOrder(customerId)==-1) //if the customer can't make order beausr reached max orders
        error("Customer reached max orders");
    else {
        Order* order = new Order(wareHouse.getOrderCounter(), customerId, (wareHouse.getCustomer(customerId)).getCustomerDistance());
        order->setStatus(OrderStatus::PENDING);
		// status(OrderStatus::PENDING); איך מעדכנים את השדות האלו
		//collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER)
        complete();
    }
    wareHouse.addAction(this);
} //add order to warehouse, maybe error

AddOrder *AddOrder::clone() const{
    return new AddOrder(customerId);
}

string AddOrder::toString() const{ 
   return "Order " + std:: to_string(customerId) + status;
}


//add customer
AddCustomer::AddCustomer(string customerName, string customerType, int distance, int maxOrders) 
    : customerName(customerName),distance(distance), maxOrders(maxOrders) {} //constructor    

void AddCustomer::act(WareHouse &wareHouse){
    customerId=wareHouse.getCustomerCounter();
     if(CustomerType(customerType)==CustomerType::Civilian){
           wareHouse.addCustomer(new CivilianCustomer(customerId ,customerName, distance, maxOrders));
     }
     else {
            wareHouse.addCustomer(new SoldierCustomer (customerId,customerName, distance, maxOrders));
     }   
    wareHouse.addAction(this);
	completed();
} //add customer to warehouse, never error

AddCustomer *AddCustomer::clone() const{
    if(customerType == CustomerType::Civilian){
        AddCustomer* newCcus= new AddCustomer(customerName, "Civilian",  distance,  maxOrders);
        newCcus->customerId=customerId;
        return newCcus;
    }
    AddCustomer* newScus= new AddCustomer(customerName, "Soldier",  distance,  maxOrders);
    newCcus->customerId=customerId;
    return newCcus;
}

string AddCustomer::toString() const{
    if(CustomerType(customerType)==CustomerType::Civilian){  
        return "AddCustomer " + customerName + ", Civilian, distance:" + std:: to_string(distance) + ", maxOrder: " + std:: to_string(maxOrders);
    }
    return "Customer: " + customerName + ", Soldier, distance:" + std:: to_string(distance) + ", maxOrder: " + std:: to_string(maxOrders);
}


//PrintOrderStatus
PrintOrderStatus::PrintOrderStatus(int id) : orderId(id){}//constructor

void PrintOrderStatus::act(WareHouse &wareHouse){

    if(wareHouse.getOrder(orderId)==(nullptr))
        error("Order doesn't exist");
    else{
        cout<< "OrderId <" <<orderId<< ">\n" 
        << "OrderStatus <" <<OrderStatus(wareHouse.getOrder(orderId).getStatus())<< ">\n" 
        << "CustomerId <" <<wareHouse.getOrder(orderId).getCustomer().getId()<< ">\n" 
        << "Collector <" <<wareHouse.getOrder(orderId).getCollectorId()<< ">\n" 
        << "Driver <" <<wareHouse.getOrder(orderId).getDriverId()<< ">\n" ;
    }
    complete();
    wareHouse.addAction(this);
} //print order status, maybe error 

PrintOrderStatus *PrintOrderStatus::clone() const{
    return new PrintOrderStatus(orderId);
}

string PrintOrderStatus::toString() const{ //need to check
    return "OrderStatus " + std:: to_string(orderId) + status;
}   
 

//PrintCustomerStatus
PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId){}//constructor

void PrintCustomerStatus::act(WareHouse &wareHouse){
    if(wareHouse.getCustomer(customerId)==nullptr){
        error("Customer doesn't exist");
        error();
    }
    else{
        cout<<"customerId : "+ customerId +"\n";
        vector<Order*> orderList=wareHouse.((getCustomer(customerId))->getOrderList());
        for(int i=0; i < orderList.size(); i++){
             cout<<"orderId :" + (*orderList[i]).getId() +"\n orderStatus : "+ OrderStatus((*orderList[i]).getStatus());
        }
        if(wareHouse.getCustomer(customerId).canMakeOrder())
            cout<<"\n num order left : "+wareHouse.((getCustomer(customerId)).getMaxOrders())-(wareHouse.(getCustomer(customerId)).getNumOrders()) +"\n";
        else cout<<"\n num order left : 0 ";
        complete();
    } 
    wareHouse.addAction(this);

}

PrintCustomerStatus *PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(customerId);
}

string PrintCustomerStatus::toString() const{ 
	return "CustomerStatus " + std:: to_string(customerId) + status;
}

         
//PrintVolunteerStatus
PrintVolunteerStatus::PrintVolunteerStatus(int id) : VolunteerId(id){}//constructor

void PrintVolunteerStatus::act(WareHouse &wareHouse){
	Volunteer &volunteer = wareHouse.getVolunteer(VolunteerId);
	if(volunteer==nullptr){
		error("Volunteer doesnt exist");
		error();
	}
	else {
		cout << "VolunteerID:" << VolunteerId << endl; //print volunteer id	
		cout << "isBusy: " volunteer->isBusy()<< endl; //print is busy

		if (volunteer->isBusy() == false){ //if the volunteer is not busy
			cout << "OrderID: None" << endl; 
			cout << "timeLeft: None" << endl; 
			cout << "ordersLeft: None" << endl; 
		}
		else if (volunteer.isCollector()){ //if the volunteer is a collector
			cout << "OrderID:" volunteer.getOrder().getId()<< endl;
			cout << "timeLeft:" << volunteer.getTimeLeft() << endl; 
			if(volunteer.getDistanceLeft() != nullptr)
				cout << "ordersLeft:" << volunteer.getDistanceLeft() << endl;
		}
		else { //if the volunteer is a driver
			cout << "OrderID:" volunteer.getOrder().getId()<< endl;
			cout << "distanceLeft:" << volunteer.getDistanceLeft() << endl; 
			if (volunteer.getNumOrdersLeft() != nullptr)
				cout << "ordersLeft:" << volunteer.getNumOrdersLeft() << endl;
		}
		complete();
	}
    wareHouse.addAction(this);
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const{
	return new PrintVolunteerStatus(VolunteerId);
}

string PrintVolunteerStatus::toString() const{
	return "PrintVolunteerStatus" + std:: to_string(VolunteerId) + status;
}


//PrintActionsLog
PrintActionsLog::PrintActionsLog(){}//constructor

void PrintActionsLog::act(WareHouse &wareHouse){
	for(int i=0; i<wareHouse.getActions().size(); i++)
		cout<< wareHouse.getActions()[i].toString()"\n"; 
	complete();
    wareHouse.addAction(this);
} 

PrintActionsLog *PrintActionsLog::clone() const{
	return new PrintActionsLog();
}

string PrintActionsLog::toString() const{
	return "log" + status; 
}


//close
close::close(){}//constructor

void close::act(warehouse &wareHouse){ 
	for(int i=0; i<wareHouse.getPendingOrders().size(); i++){
		cout<< "order" wareHouse.getPendingOrders()[i].getOrderCounter() + " " + wareHouse.getPendingOrders()[i]->getId() + " " + wareHouse.getPendingOrders()[i]->getCustomerId() + " " + wareHouse.getPendingOrders()[i]->getStatus() + "\n";
	}
	for(int i=0; i<wareHouse.getCompletedOrders().size(); i++){
		cout<< "order" wareHouse.getCompletedOrders()[i].getOrderCounter() + " " + wareHouse.getCompletedOrders()[i]->getId() + " " + wareHouse.getCompletedOrders()[i]->getCustomerId() + " " + wareHouse.getCompletedOrders()[i]->getStatus() + "\n";
	}
	for(int i=0; i<wareHouse.getinProcessOrders().size(); i++){
		cout<< "order" wareHouse.getinProcessOrders()[i].getOrderCounter() + " " + wareHouse.getinProcessOrders()[i]->getId() + " " + wareHouse.getinProcessOrders()[i]->getCustomerId() + " " + wareHouse.getinProcessOrders()[i]->getStatus() + "\n";
	}
    wareHouse.~WareHouse();
}

close *close::clone() const{
	return new close();
}

string close::toString() const{
	return "close";
}


//backup warehouse
BackupWareHouse::BackupWareHouse(){}//constructor

void BackupWareHouse::act(WareHouse &wareHouse){
    if(backup!=wareHouse){
        delete backup;
        backup = wareHouse;
    }
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
	wareHouse = backup; //move assignment opertor
}

RestoreWareHouse *RestoreWareHouse::clone() const{
	return new RestoreWareHouse();
}

string RestoreWareHouse::toString() const{
	return "RestoreWareHouse";
}

