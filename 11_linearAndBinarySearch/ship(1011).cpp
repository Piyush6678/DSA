#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;


int shipCapacity(vector<int> &weights,int Days)
{
int n =weights.size();
//finding max element and sum 
int max=-1,sum=0;
for (int i=0;i<n;i++){
if(weights[i]>max)max=weights[i];
sum+=weights[i];
}

// 
int lo =max,cap=0,hi=sum,mid,days=0,minCap;


while(lo<=hi){
    mid=lo+(hi-lo)/2;
 days=0;
    for (int i =0;i<n;i++){
cap+=weights[i];

if(  (i==n-1) || ((cap + weights[i+1])>mid)){
    days++;cap=0;
}
}
if(days==Days){minCap= mid; hi=mid-1;} 
else if(days<Days)hi =mid-1;
else lo=mid+1;



    

}







}
int main(){
    return 0;
}