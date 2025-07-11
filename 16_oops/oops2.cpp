#include <iostream>
using namespace std;
//constructors 
class Bike{
public:
static int noOfBikes; //this belongs to class
int tyreSize;
int engineSize;

//default constructor 
// Bike(){}

//parameterized constructor
Bike(int tyreSize,int engineSize){
    this->tyreSize=tyreSize;
    this->engineSize=engineSize;
    noOfBikes++;
}
//constructor overloading
 Bike(int ts):tyreSize(ts),engineSize(150){}
 Bike():tyreSize(17),engineSize(150){}

//initialisation list 
// Bike(int ts,int es):tyreSize(ts),engineSize(es){}




//Destructor //calls when object goes out of scope
  
~Bike (){
    cout<<"destructor call hua";
}

};
//static
// void print(){
// static int b =10;
// cout<<b<<endl;
// b++;
// }
int Bike::noOfBikes=0;
int main() {
    Bike tvs(18);//object creation //constructor call
   cout<<tvs.noOfBikes<<endl; //1
   Bike honda(17.5);
   cout<<honda.noOfBikes<<endl; //2
   
   Bike suzuki(19);
   cout<<suzuki.noOfBikes<<endl;//3
    
    //static 
    // print();//10
    // print();//11
    // print();//12


    return 0;
}