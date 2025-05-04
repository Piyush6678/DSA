#include<iostream>
using namespace std;
int main (){
// greet 5 times
for (int i =1;i<=5;i++){
    cout<<"good morning \n";
}

//Print numbers from 1 to 100
for(int i =1 ;i<=100;i++){
    cout<<i;
}

//Print all the even numbers from 1 to 100
for(int i =1 ;i<=100;i++){
 if (i%2==0)   cout<<i;
}

// Display the AP - 1, 3, 5, 7, 9... up to 'n' terms
int n;
cout << "Enter the number of terms for the AP: ";
cin >> n;
for (int i = 0; i < n; i++) {
    cout << (2 * i + 1) << " ";
}
cout << endl;

// Display the GP - 1, 2, 4, 8, 16, 32... up to 'n' terms
cout << "Enter the number of terms for the GP: ";
cin >> n;
int term = 1;
for (int i = 0; i < n; i++) {
    cout << term << " ";
    term *= 2;
}
cout << endl;
return 0;
}