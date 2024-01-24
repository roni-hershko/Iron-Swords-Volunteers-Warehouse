#include "../include/Action.h"
#include <string>
#include <vector>
#include <WareHouse.h>
using std::string;
using std::vector;

// vector<BaseAction*> actionsLog; האם בסוף כל פעולה צריך להכניס לוקטור של ה 



//base action
BaseAction::BaseAction():errorMsg("<error_msg>"),status(ActionStatus::ERROR){}
void BaseAction::complete(){
    status=ActionStatus::COMPLETED;
}//chage status to completed, if it was completed

void BaseAction::error(string errorMsg){
    status=ActionStatus::ERROR;
    this->errorMsg=errorMsg;
}//change status to error, and print

string BaseAction::getErrorMsg() const{
    return errorMsg;
} //return the error message







//simulate step
SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps){} //constructor

void SimulateStep::act(WareHouse &wareHouse){
	for(int i=0; i<numOfSteps; i++){
		for(int j=0; j<wareHouse.getVolunteers().size(); j++){ //לעבור על הלקוחות ההזמנות המתנדבים?
			

			if(wareHouse.getVolunteers()[j]->isBusy()){ //if the volunteer is busy
				if(wareHouse.getVolunteers()[j]->getTimeLeft() != nullptr){ //if the volunteer is a collector
					wareHouse.getVolunteers()[j]->setTimeLeft(wareHouse.getVolunteers()[j]->getTimeLeft()-1); //decrease the time left by 1
					if(wareHouse.getVolunteers()[j]->getTimeLeft() == 0){ //if the time left is 0
						wareHouse.getVolunteers()[j]->setBusy(false); //set the volunteer to not busy
						wareHouse.getVolunteers()[j]->setTimeLeft(nullptr); //set the time left to nullptr
						wareHouse.getVolunteers()[j]->setOrder(nullptr); //set the order to nullptr
						wareHouse.getVolunteers()[j]->setDistanceLeft(nullptr); //set the distance left to nullptr
						wareHouse.getVolunteers()[j]->setNumOrdersLeft(nullptr); //set the number of orders left to nullptr
					}
				}
				else { //if the volunteer is a driver
					wareHouse.getVolunteers()[j]->setDistanceLeft(wareHouse.getVolunteers()[j]->getDistanceLeft()-1); //decrease the distance left by 1
					if(wareHouse.getVolunteers()[j]->getDistanceLeft() == 0){ //if the distance left is 0
						wareHouse.getVolunteers()[j]->setBusy(false); //set the volunteer to not busy
						wareHouse.getVolunteers()[j]->setTimeLeft(nullptr); //set the time left to nullptr
						wareHouse.getVolunteers()[j]->setOrder(nullptr); //set the order to nullptr
						wareHouse.getVolunteers()[j]->setDistanceLeft(nullptr); //set the distance left to nullptr
						wareHouse.getVolunteers()[j]->setNumOrdersLeft(nullptr); //set the number of orders left to nullptr
					}
				}
			}
			else { //if the volunteer is not busy
				if(wareHouse.getVolunteers()[j]->getOrder() != nullptr) //if the volunteer has an order
					if(wareHouse.getVolunteers()[j]->getOrder()->getStatus() == OrderStatus::PENDING) //if the order is pending
			}
		}
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
        BaseAction::error("Customer does not exist");
    }
    else if (wareHouse.getCustomer(customerId).addOrder(customerId)==-1)
        BaseAction::error("Customer reached max orders");
    Order* order = new Order(wareHouse.getOrderCounter(), customerId, (wareHouse.getCustomer(customerId)).getCustomerDistance());
    order->setStatus(OrderStatus::PENDING);
    wareHouse.getPendingOrders().push_back(order);
    orderAdded=true;
} //add order to warehouse, maybe error

string AddOrder::toString() const{ 
 //   if(orderAdded)
  //      return "the order of: " + customerId + "was added";
   // return "the order of: " + customerId + "was not added";
   return "Order " + customerId + status;
}

AddOrder *AddOrder::clone() const{
    return new AddOrder(customerId);
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
} //add customer to warehouse, never error

AddCustomer *AddCustomer::clone() const{
    if(customerType == CustomerType::Civilian){
        AddCustomer* newCcus= new AddCustomer(customerName, "Civilian",  distance,  maxOrders);
        newCcus->customerId=customerId;
        return newCcus;
    }
    AddCustomer* newCcus= new AddCustomer(customerName, "Soldier",  distance,  maxOrders);
    newCcus->customerId=customerId;
    return newCcus;
}

string AddCustomer::toString() const{
    if(CustomerType(customerType)==CustomerType::Civilian){  
        return "AddCustomer " + customerName + ", Civilian, distance:" + distance + ", maxOrder: " + maxOrders;
    }
    return "Customer: " + customerName + ", Soldier, distance:" + distance + ", maxOrder: " + maxOrders;
}





//PrintOrderStatus
PrintOrderStatus::PrintOrderStatus(int id) : orderId(id){}//constructor

void PrintOrderStatus::act(WareHouse &wareHouse){
    if(wareHouse.getOrder(orderId)==(nullptr))
        error("Order doesn't exist");
    else{
        cout<< "OrderId <" <<orderId<< ">\n" 
        << "OrderStatus <" <<OrderStatus(wareHouse.getOrder(orderId).getStatus())<< ">\n" 
        << "CustomerId <" <<wareHouse.getOrder(orderId)->getCustomer()->getId()<< ">\n" 
        << "Collector <" <<wareHouse.getOrder(orderId)->getCollectorId()<< ">\n" 
        << "Driver <" <<wareHouse.getOrder(orderId)->getDriverId()<< ">\n" ;
    }
} //print order status, maybe error 

PrintOrderStatus *PrintOrderStatus::clone() const{
    return new PrintOrderStatus(orderId);
}

string PrintOrderStatus::toString() const{ //need to check
}   
 






//PrintCustomerStatus
PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId){}//constructor

void PrintCustomerStatus::act(WareHouse &wareHouse){
    if(wareHouse.getCustomer(customerId)==nullptr)
        error("Customer doesn't exist");
    else{
        cout<<"customerId : "+ customerId +"\n";
        vector<Order*> orderList=wareHouse.((getCustomer(customerId))->getOrderList());
        for(int i=0; i < orderList.size(); i++){
             cout<<"orderId :" + (*orderList[i]).getId() +"\n orderStatus : "+ OrderStatus((*orderList[i]).getStatus());
        }
        if(wareHouse.getCustomer(customerId).canMakeOrder())
            cout<<"\n num order left : "+wareHouse.(getCustomer(customerId))->getMaxOrders()-wareHouse.(getCustomer(customerId))->getNumOrders() +"\n";
        else cout<<"\n num order left : 0 ";
    } 
}//print customer status, maybe error

PrintCustomerStatus *PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(customerId);
}

string PrintCustomerStatus::toString() const{ //need to check
	return "PrintCustomerStatus " + customerId + status;
}

        


//PrintVolunteerStatus
PrintVolunteerStatus :: PrintVolunteerStatus(int id) : VolunteerId(id){}//constructor

void PrintVolunteerStatus ::act(WareHouse &wareHouse){
	if(wareHouse.getVolunteer(VolunteerId)==nullptr)
		error("Volunteer doesnt exist");

	else {
		cout << "VolunteerID:" << VolunteerId << endl; //print volunteer id	
		cout << "isBusy: " wareHouse.getVolunteer(VolunteerId)->isBusy()<< endl; //print is busy

		if (wareHouse.getVolunteer(VolunteerId)->isBusy() == false){ //if the volunteer is not busy
			cout << "OrderID: None" << endl; 
			cout << "timeLeft: None" << endl; 
			cout << "ordersLeft: None" << endl; 
		}
		else if (wareHouse.getVolunteer(VolunteerId)->getTimeLeft() != nullptr){ //if the volunteer is a collector
			cout << "OrderID:" wareHouse.getVolunteer(VolunteerId)->getOrder()->getId()<< endl;
			cout << "timeLeft:" << wareHouse.getVolunteer(VolunteerId)->getTimeLeft() << endl; 
			if(wareHouse.getVolunteer(VolunteerId)->getDistanceLeft() != nullptr)
				cout << "ordersLeft:" << wareHouse.getVolunteer(VolunteerId)->getDistanceLeft() << endl;
				// USING DISTANCE LEFT AND NOT MAX Distance AS WRITTEN IN THE PAPER
		}
		else { //if the volunteer is a driver
			cout << "OrderID:" wareHouse.getVolunteer(VolunteerId)->getOrder()->getId()<< endl;
			cout << "distanceLeft:" << wareHouse.getVolunteer(VolunteerId)->getDistanceLeft() << endl; 
			if (wareHouse.getVolunteer(VolunteerId)->getNumOrdersLeft() != nullptr)
				cout << "ordersLeft:" << wareHouse.getVolunteer(VolunteerId)->getNumOrdersLeft() << endl;
				// USING ORDERS LEFT AND NOT MAX ORDER AS WRITTEN IN THE PAPER
		}
	}	
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const{
	return new PrintVolunteerStatus(VolunteerId);
}

string PrintVolunteerStatus::toString() const{
	return "PrintVolunteerStatus" + VolunteerId + status;
}






//PrintActionsLog
PrintActionsLog::PrintActionsLog(){}//constructor

void PrintActionsLog::act(WareHouse &wareHouse){
	for(int i=0; i<wareHouse.actionsLog.size(); i++){ //להכניס לרשימה את כל הפעולות שנעשו 
		cout<< wareHouse.operator[i]."\n"; // האם יש צורך למחוק את הפעולות
	}
} 

PrintActionsLog *PrintActionsLog::clone() const{
	return new PrintActionsLog();
}

string PrintActionsLog::toString() const{
	return "PrintActionsLog" + status; 
}






//close
close::close(){}//constructor

void close::act(warehouse &wareHouse){ 
	for(int i=0; i<wareHouse.getPendingOrders().size(); i++){
		cout<< "order" wareHouse.getPendingOrders()[i]->getOrderCounter() + " " + wareHouse.getPendingOrders()[i]->getId() + " " + wareHouse.getPendingOrders()[i]->getCustomerId() + " " + wareHouse.getPendingOrders()[i]->getStatus() + "\n";
	}
	for(int i=0; i<wareHouse.getCompletedOrders().size(); i++){
		cout<< wareHouse.getCompletedOrders()[i]->getOrderCounter() + " " + wareHouse.getCompletedOrders()[i]->getId() + " " + wareHouse.getCompletedOrders()[i]->getCustomerId() + " " + wareHouse.getCompletedOrders()[i]->getStatus() + "\n";
	}
	//free all memory
	for(int i=0; i<wareHouse.getVolunteers().size(); i++){
		delete wareHouse.getVolunteers()[i];
	}
	for(int i=0; i<wareHouse.getPendingOrders().size(); i++){
		delete wareHouse.getPendingOrders()[i];
	}
	for(int i=0; i<wareHouse.getCompletedOrders().size(); i++){
		delete wareHouse.getCompletedOrders()[i];
	}
	for(int i=0; i<wareHouse.getinProcessOrders().size(); i++){
		delete wareHouse.getinProcessOrders()[i];
	}
	for(int i=0; i<wareHouse.getCustomers().size(); i++){
		delete wareHouse.getCustomers()[i];
	}

	isOpen=false;
}

close *close::clone() const{
	return new close();
}

string close::toString() const{
	return "close";
}







//backup warehouse
extern WareHouse *backup;
BackupWareHouse::BackupWareHouse(){}//constructor
	backup = wareHouse; 
}

BackupWareHouse *BackupWareHouse::clone() const {
	return new BackupWareHouse();
}

string BackupWareHouse::toString() const {
	return "BackupWareHouse";
}







//restore warehouse
RestoreWareHouse::RestoreWareHouse(){}//constructor

void RestoreWareHouse::act(WareHouse &wareHouse){

}

RestoreWareHouse *RestoreWareHouse::clone() const{
	return new RestoreWareHouse();
}

string RestoreWareHouse::toString() const{
	return "RestoreWareHouse";
}

