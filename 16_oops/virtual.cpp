#include <iostream>
using namespace std;
class Vehicle  {
    public:
int noOfTyres ;
virtual void  show(){ 
    cout<<"mai vehicle ka show hu ";
}

} ;
class Bike: public Vehicle{
public: 
int speed;
void show (){ //  overriudes the function  of A
    cout <<"Bike ka show ";
}
};
int main() {
    //compiler pov - vehicle type ka hai
    // run pov -bike ka address hai 
   // runtime binding -virtual;
     Vehicle *a;
     a=new Bike ;
     
     a->show();// Bike ka show 
     a=new Vehicle ;
     a->show();// Bike ka show 
     

    return 0;
}