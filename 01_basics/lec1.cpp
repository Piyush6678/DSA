// Hello World 
#include<iostream>
using namespace std;
int main(){
    cout<<"hello world";

    //escape sequence
    cout<<endl<<"name:Piyush Aggarwal ";
    cout<<"\nAge:21";

//Printing numbers
cout<<endl<<4<<endl<<4+3<<endl<<"4+3"<<endl;

//Variable
int x;
x=7 ;
cout<<x<<endl;
x=10;
cout<<x<<endl;
x=x+3 ;// x= 10 +3 =13
cout<<x<<endl;

//Arithmetic opertion
int a=5,b=2;
cout<<a+b<<endl;  // 7
cout<<a-b<<endl;  // 3
cout<<a*b<<endl;  // 10
cout<<a/b<<endl;  // 2
cout<<a%b<<endl;  // 1


//increment and decrement operator
x=5;
cout<<++x<<endl;
cout<<x<<endl;
cout<<x++<<endl;
cout<<x ;// 6 6 6 7

//find the remainder
a=7;b=6;
cout<<a%b; // 1
a=3;
cout<<a%b; // if a<b return a means 3
b=-6; a=7; 
cout<<a%b; //  1 (a%b)
a=-a;b=-b; //a=-7 b=6
cout<<a%b; // -(a%b) -1
b=-b;
cout<<a%b; // -(a%b) -1
a=6;b=0;
cout<<endl<<a%b;



// Float Data type

float x =3.1;

cout<< 5/2;//2

cout<< 5.0/2;//2.5

cout<< 5/2.0;//2.5

cout<< 5.0/2.0;//2.5

float d=5,e=2;

cout<<d/e;//2.5

//Q Area of circle 
float r =5;
float pi =3.1415;
float area=pi*r*r;

// /Q Calculate simple interest
 float p,r,t,si;
 p=100;
 r=10;
 t=2;
 si=(p*r*t)/100;
  

return 0;

}