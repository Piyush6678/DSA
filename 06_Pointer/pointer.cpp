#include<iostream>
using namespace std;
//Calculate sum using Pointer
int sumPointer(int a,int b ){
    int *p=&a;
    int *q=&b;
    return ((*p)+(*q));

}
//pass by Value
void swapValue(int a,int b){
    int temp =a;a=b;b=temp;return ;

}
//pass by reference
void swapReference(int* a,int* b){
    int temp = *a;*a=*b ;*b=temp;return;
}
int main(){
    int x =4;
    int* p=&x;
    cout<<&x<<endl; //Address of x
    cout<<p<<endl; //Address of x
    cout<<*p<<endl; //4 VALUE OF X
    *p=6 ;   //x becomes 6
    cout<<x;

// pass by value 
int a=5,b=6;
swapValue(a,b);
cout<<a<<b;  //5 6 
//pass by Reference
swapReference(&a,&b);
cout<<a<<b;  // 6 5 
// null Pointer
int c;
int*ptr=&c;

//Double Pointer 
int  d;
int* ptr1=&d;
int** ptr2=&ptr1;
    return 0;
}