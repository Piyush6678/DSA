#include<iostream>
#include<vector>

using namespace std;
int binarysearch(vector<int> &nums,int trgt ){
//return index of target element if not found return -1
int n =nums.size();
int hi =n-1,lo=0,mid;
while(lo<hi ){
    mid=(lo/2)+(hi/2);
if (nums[mid]==trgt)return mid ;
else if (nums[mid]<trgt) lo=mid+1;
else hi =mid-1;
}
return -1;
}

//Lower Bound 
//given a sorted integer array and an integer x find the lower bound of x 

int lowerBound(vector<int>nums,int x){
    int n =nums.size();
    int lo =0,mid,hi=n-1;
    while(lo<hi ){
        mid=(lo/2)+(hi/2);
        // if(nums[mid]>=x) {
        //     if(nums[mid-1]<x)return mid-1;
        //     else hi =mid-1;
        // }
        // if(nums[mid]<x){
        //       if(nums[mid+1]>x)return mid;
        //       else lo=mid+1;
        // }
   if(nums[mid]==x) return mid-1;
        else     if (nums[mid]<x)lo=mid+1;
    else    if(nums[mid]>x) hi =mid-1;

    }
    return hi;


    
}


int main(){


    return 0;
}