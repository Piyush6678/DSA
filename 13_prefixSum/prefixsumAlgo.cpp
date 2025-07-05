#include <iostream>
using namespace std;

int main() {
    int arr[]={1,4,5,2,5,2,6};
    int n =sizeof(arr)/sizeof(arr[0]);
    int pre[n];
    int sum=0;
    for  (int i =0;i<n;i++){
        sum+=arr[i];
        pre[i]=sum;
    };
    //or 
    pre[0]=arr[0];
    for  (int i =1;i<n;i++){
        
        pre[i]=pre[i-1];
    };
    for  (int i =0;i<n;i++){
        
        cout<<pre[i]<<endl;
    };



    return 0;
}