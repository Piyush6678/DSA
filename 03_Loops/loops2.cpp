#include<iostream>
using namespace std;
int main(){
    //WAP to check if a number is prime or not
int n ,x=0;
cin>>n ;cout<<"enter the number \n";
    for (int i =2;i<=n/2;i++){
if (n % i== 0) x=1;
}
if(x){
    cout<<"Given number is not prime \n";

}
else{
    cout<<"Given number is prime \n";
}

// Write a progaram to count digit of a number 
cin>>n;
cout<<"enter the number ";
int count=0,a=n;
while(n>0){
    n/=10;
    count++;
}
if(a){
    cout<<count;
}
else{
    cout<<"1";
}
// Write a program to sum of digits of a number
cin >> n;
cout << "enter the number ";
int sum = 0;
while (n > 0) {
    sum += n % 10;
    n /= 10;
}
cout << "Sum of digits: " << sum << endl;


// Write a program to print reverse of a given number
cin >> n;
cout << "enter the number ";
int reverse = 0;
while (n > 0) {
    reverse = reverse * 10 + n % 10;
    n /= 10;
}
cout << "Reverse of the number: " << reverse << endl;
    return 0;
}