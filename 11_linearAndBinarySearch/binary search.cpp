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

int firstOccurence (vector<int>nums ,int x ){
    int n =nums.size();
    int lo=0,hi=n-1,mid;
    while(lo<=hi){
        int mid=lo/2+hi/2;
        if(nums[mid]==x){
            if(nums[mid-1]==x)hi=mid-1;
            else return mid ;
        }
        else if(nums[mid]<x)lo=mid+1;
        else hi=mid-1;
    }
    return -1;
}
// Sorted array of non negative distinct integers ,find the smallest missing non negative element

int smallestMissing(vector<int>nums){
    int n =nums.size(),lo=0;
int hi =n-1,mid,ans=-1; 

while(lo<hi){
mid=lo/2+hi/2;
 if(nums[mid]==mid)lo=mid+1;
 else {
    ans=mid;hi=mid-1;
 } 


}
return ans;

}


int sqrt(int x)
{
int lo =0,hi=x,mid;
while(lo<=hi){
    mid=lo/2+hi/2;
    if(mid*mid==hi)return mid;
    else if(mid*mid<hi)lo=mid+1;
    else hi=mid-1;

}
return hi ;
}



int main(){


    return 0;
}