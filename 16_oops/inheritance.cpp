#include <iostream>
using namespace std;
class vehicle{ //base ,parent class
    public:
    int tyreSize;
    int engineSize;
    string company;
};
class Bike:public vehicle {//bike inherits vehicle 
    public:
    
    int mileage;
    int year;
    
};
class Car:public vehicle { // derived class ,child class
    public: 
   
    int seats;
    int topSpeed;
    int length ;
};

class A{
    private: // cannot be access cannot be inherited
    int a_private;
    protected :  // cannot be access can be inherited
    int a_protected;
    public:// can be access can be inherited
    int a_public;
};
class B :public  A{
public:
int b_public;
};



int main() {
    Bike honda;
    honda.engineSize=150;
    honda.mileage=45;
    return 0;
}