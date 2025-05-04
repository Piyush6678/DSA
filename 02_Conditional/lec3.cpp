#include <iostream>
using namespace std;

int main() {
//Take positve integer input from user and check if it is even or odd
int n;
cout<<"enter the numnber ";
cin>>n;
if(n%2==0){
    cout<<"even number"<<endl;
}
else{
cout<<"odd number"<<endl;}  

//divisible by 5 or not 
if (n%5==0){
    cout<<endl<<"number is divisible by five";
}

//absolute value 
if (n<0){
    n=-n;
}
cout<<endl<<"absolute value of n is n";

//profit loss
int cp , sp ;
cout<<endl<<"enter the cost price";
cin>>cp;
cout<<endl<<"Enter the selling price";
cin>>sp;
if(sp>cp){
    cout<<endl<<"Your profit is "<<sp-cp;
}
else if(cp==sp){
cout<<endl<<"your profit is 0";
}
else{
    cout<<endl<<"Your loss is "<<cp-sp;
}

// n is three digit or not 
if(n>99 && n<1000){
    cout<<endl<<" Given number is three digit number";
}
if(n>99 and n<1000){
    cout<<endl<<" Given number is three digit number";
}

//divisible by 5 or 3
if(n%5==0 or n%3==0){
    cout<<endl<<" Given number is divisible by 5 or 3";
}
if(n%5==0 || n%3==0){
    cout<<endl<<" Given number is divisible by 5 or 3";
}

// take 3 number and print gratest of them 
int a ,b,c;
cout<<endl<<"Enter The 3  number";
cin>>a>>b>>c;
if(a>=b){
    if (c>a) cout<<endl<<c<<" is greatest";
    else cout<<endl<<a<<" is the greatest";
}
else{
    if (c>b) cout<<endl<<c<<" is greatest";
    else cout<<endl<<b<<" is the greatest";
}

    return 0;
}