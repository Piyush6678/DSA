#include<iostream>
using namespace std;
void swap(int *a,int* b){
    *a=*a+*b;
    *b=*a-*b;
    *a=*a-*b;
}
//function to move all negative numbers in array to beginnig and positives to end
void moveNegative(int arr[], int n){
    int i = 0, j = n - 1;
    while (i < j) {
        if (arr[i] > 0 && arr[j] < 0) {
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
        }
       if (arr[i] < 0) {
            i++;
        }
       if (arr[j] > 0) {
            j--;
        }
      
    }
}
int main(){
    // sorting arra of 0 and 1 
    int arr[]={
        0,1,0,1,0,1,0,1,0,0,1,0,1,1,0,0,1,0
    };
    int n = sizeof(arr)/sizeof(arr[0]);
    //two pass method
    int noz=0;
    for (int i =0;i<n;i++){
if (arr[i]==0)noz++;
    }
    for (int i =0;i<n;i++){
        if(i<noz)arr[i]=0;
        else arr[i]=1;
    }

    //two pointer method
    int i=0,j=n-1;
    while(i<j){
        if(arr[i]==1 && arr[j]==0){
            //swap
            
         
            arr[i]=0;
            arr[j]=1;
            i++;j--;
        }
        if(arr[i]==0){ i++;}
        if (arr[j]==1){j--;}
      
    
    }







// IMP 
// Dutch Flag Algorithm 
// The Dutch National Flag algorithm efficiently sorts an array containing only 0s, 1s, and 2s in a single pass (O(n) time, O(1) space). It was proposed by Edsger Dijkstra.
// sort the array of 0 1 and 2
int dutch[23]={0,1,2,0,1,1,2,2,0,1,2,1,2,0,2,0,0,0,1,2,1,0,2};
n=23; // size of array
int lo=0,mid=0,hi=n-1;

//dutch flag algorithm
while (mid<=hi){

    if(dutch[mid]==2){
        //swap mid and hi 
swap(&dutch[mid],& dutch[hi]);
hi--;
}
else if(dutch[mid]==1){mid++;}
else if(dutch[mid]==0){
    // swap lo and mid 
    swap(&dutch[mid],& dutch[lo]);
    lo++; mid++;
   }
}
    return 0;
}