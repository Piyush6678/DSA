#include <iostream>
using namespace std;
class A {
    public:
int a ;
void show(){
    cout<<"mai a ka show hu ";
}

} ;
class B: public A{
public: 
int b;
void show (){ //  overriudes the function  of A
    cout <<"B ka show ";
}
};
int main() {
   // B b ;
    //b.show(); // "B ka show" 
    //b.A::show(); // "A ka show"

    //
     B b ;
     A *a;
     a=&b;
     a->a=1;
     a->show();// a ka show 

    return 0;
}