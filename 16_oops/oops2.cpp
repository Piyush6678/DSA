#include <iostream>
using namespace std;
//constructors 
class Bike{
public:
int tyreSize;
int engineSize;

//default constructor 
// Bike(){}

//parameterized constructor
Bike(int tyreSize,int engineSize=150){
    this->tyreSize=tyreSize;
    this->engineSize=engineSize;
}


//Destructor //calls when object goes out of scope
  
~Bike (){
    cout<<"destructor call hua";
}
};
int main() {
    Bike tvs(18);//object creation //constructor call
    Bike honda(17.5);
    Bike suzuki(19);
      
    return 0;
}