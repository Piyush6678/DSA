#include <iostream>
using namespace std;



void bubbleSort (int arr[]){
    //Stable sort 
    // time complexity  (n(n-1))/2 == o(n^2)
// TC best Case=>O(1) avg/worstcase=O(n^@) 

    int n =sizeof(arr)/sizeof(arr[0]);
    bool flag=false;
    for (int i =0;i<n-1;i++){
        flag=false;
        for(int j =0;j<n-1-i;j++){
            if(arr[j]>arr[j+1]){
                //swap
                int temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;
                flag=true;
            }
        }
        if(!flag)break;
    }
}
int main() {
    
    return 0;
}