#include<iostream>
using namespace std;
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
    return 0;
}