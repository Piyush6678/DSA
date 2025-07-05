//Check if array can be partitione into continuous array of waul sum 
#include <iostream>
using namespace std;

int main() {
    int arr[]={1,2,3,4,5,15};
    int n =sizeof(arr)/sizeof(arr[0]);

for (int i =1; i<n ; i++){
    arr[i]+=arr[i-1];
    cout<<arr[i]<<endl;
     
}
for (int i =0;i<n-1;i++){
    if ((2*arr[i]-arr[n-1])==0) {cout<<i;break;}
    
}

    return 0;
}