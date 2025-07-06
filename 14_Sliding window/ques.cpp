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

void ques2 (int arr[],int n,int k){
    //First negative numver in every window of size of size k 
    
int ans[n-k+1]; 
int prvIdx=-1;
for (int i =0;i<k;i++)
{
    if(arr[i]<0){
        prvIdx=i;
        ans[0]=arr[prvIdx];break;
    }
}
int i =1,j=k;


while(j<n){
if(prvIdx<i){
    
for (int m =i;m<=j;m++)
{
    if(arr[m]<0){
        prvIdx=m;
        break;
    }
}
}
ans[i]=arr[prvIdx];
i++;j++;

}


for (int l =0;l<=n-k;l++){
    cout<<ans[l]<<endl; // -3,-3,-7,-7,-7,-1,-2
}


}
int main() {

//QUES 
    int arr[]={7,1,2,5,8,4,9,3,6 };
    // ques1(arr,9,3);
      int arr2[]={2,-3,4,4,-7,-1,4,-2,6 };
    int n =9,k=4;
    ques2(arr2,n,k); 
   
    return 0;
}