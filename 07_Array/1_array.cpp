#include<iostream>
using namespace std;
int main(){
//Syntax and Declaration
int x[7]; // array x pf size 7
int y[5]={1,23,1,5,3}; // array y of size 5 
int z[]={1,2,3,4,5,6}; //automaticaly take sizes

// Q1Given an array of marks of students, if the
// mark of any student is less than 35 print its roll
// number. [roll number here refers to the index of the
// array.]

for(int i=0 ;i<7;i++){
    cin>>x[i];
}
for (int i =0;i<7;i++){
    if (x[i]<35) cout<<i;
}

//size and size of 
int s =sizeof(x)/sizeof(x[0]); // size of array x 

//Q2 sum of all elements in given array 
int sum=0 ;
for (int i =0;i<5;i++){
    sum+=y[i];

}
cout<<sum;


// Q3 Find the element x in the array . Take array and
// x as input.
int n,a;
cout<<"enter the size of array";
cin>>n;

int arr[n];
for (int i =0;i<n;i++){
cin>>arr[i];
}
cout<<"enter the target element";
cin>>a;
 
for(int i =0;i<n;i++)
{
    if (arr[i]==a) {cout<< i;
break;
    }}

//find max value
int max=-1;
for (int i =0 ;i<n;i++){
if (max<=arr[i])max=arr[i];
}










    return 0;

}