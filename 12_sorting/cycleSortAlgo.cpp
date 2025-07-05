#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;
int main(){
   // TC= O(n) SC = O(1)
int arr[5]={4,5,3,1,2};
int i =0;
while(i<5){
    if(arr[i]==i+1)  i++ ;    
    else swap(arr[i],arr[arr[i]-1])  ; 
  
    
}

    return 0;
}