#include "../include/BaseAction.h"
#include <string>
#include <vector>
#include "WareHouse.h"
using std::string;
using std::vector;

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