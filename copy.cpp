PrintVolunteerStatus :: PrintVolunteerStatus(int id) : VolunteerId(id){}//constructor

void PrintVolunteerStatus ::act(WareHouse &wareHouse){
	if(wareHouse.getVolunteer(VolunteerId)==nullptr)
		error("Volunteer doesnt exist");

	else {
		cout/n << "VolunteerID: " << VolunteerId <<; 	//print volunteer id	
		cout/n << "isBusy: " wareHouse.getVolunteer(VolunteerId)->isBusy()<<; //print is busy

		if (wareHouse.getVolunteer(VolunteerId)->isBusy() == false){ //if the volunteer is not busy
			cout/n << "OrderID: None " <<; 
			cout/n << "timeLeft: None " <<; 
			cout/n << "ordersLeft: None " <<; 
		}
		else if (wareHouse.getVolunteer(VolunteerId)->getTimeLeft() != nullptr){ //if the volunteer is a collector
			cout/n << "OrderID: " wareHouse.getVolunteer(VolunteerId)->getOrder()->getId()<<;
			cout/n << "timeLeft: " << wareHouse.getVolunteer(VolunteerId)->getTimeLeft() <<; 
			if(wareHouse.getVolunteer(VolunteerId)->getDistanceLeft() != nullptr)
				cout/n << "ordersLeft: " << wareHouse.getVolunteer(VolunteerId)->getDistanceLeft() <<;
				// USING DISTANCE LEFT AND NOT MAX Distance AS WRITTEN IN THE PAPER
		}
		else { //if the volunteer is a driver
			cout/n << "OrderID: " wareHouse.getVolunteer(VolunteerId)->getOrder()->getId()<<;
			cout/n << "distanceLeft: " << wareHouse.getVolunteer(VolunteerId)->getDistanceLeft() <<; 
			if (wareHouse.getVolunteer(VolunteerId)->getNumOrdersLeft() != nullptr)
				cout/n << "ordersLeft: " << wareHouse.getVolunteer(VolunteerId)->getNumOrdersLeft() <<;
				// USING ORDERS LEFT AND NOT MAX ORDER AS WRITTEN IN THE PAPER
		}
	}	
}
