#include <iostream>
#include <math.h>
using namespace std;
void ques1(int arr[],int n,int k ){
//maximum sum subarray of size k
 int maxSum=INT16_MIN,idx;
int prevSum=0;
for (int i =0;i<k;i++){
    prevSum+=arr[i];
    
}
maxSum=prevSum;
int i =1,j=k;
while(j<n){
    prevSum-=arr[i-1];
    prevSum+=arr[j];
    if(maxSum<prevSum){

        maxSum=prevSum;
    idx=i;
    }
    i++;j++;
    
}
cout<<idx<<"  "<<maxSum;


}
int main() {
    
    return 0;
}