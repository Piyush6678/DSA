#include <iostream>
using namespace std;
//constructors 
class Bike{
public:
int tyreSize;

//default constructor 
// Bike(){}

//parameterized constructor
Bike(int tyreSize){
    this->tyreSize=tyreSize;
}
};
int main() {
    Bike tvs(18);//object creation //constructor call
    Bike honda(17.5);
    Bike suzuki(19);
      
    return 0;
}