#include <iostream>
using namespace std;
void selestionSort (int arr[],int n ){
    //TC O(n^2)
//unstabel sort 
    for(int i =0;i<n;i++){
        int min=INT32_MAX,idx;
        for (int j =i;j<n-1;j++){
          if(arr[j]<min){
            min=arr[j];idx=j;
          }
          
        }
        if (arr[idx]!=arr[i])  swap(arr[i],arr[idx]);
    }
}


int main() {
    

    return 0;
}