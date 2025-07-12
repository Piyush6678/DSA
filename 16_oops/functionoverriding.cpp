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
    B b ;
    b.show(); // "b ka show" 
    return 0;
}