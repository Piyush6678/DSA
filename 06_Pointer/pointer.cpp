#include<iostream>
using namespace std;
//Calculate sum using Pointer
int sumPointer(int a,int b ){
    int *p=&a;
    int *q=&b;
    return ((*p)+(*q))

}
int main(){
    int x =4;
    int* p=&x;
    cout<<&x<<endl; //Address of x
    cout<<p<<endl; //Address of x
    cout<<*p<<endl; //4 VALUE OF X
    *p=6 ;   //x becomes 6
    cout<<x;
    return 0;
}