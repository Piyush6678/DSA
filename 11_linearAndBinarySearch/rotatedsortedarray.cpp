#include<iostream>
#include<vector>

using namespace std ;

int rotated(vector<int>nums,int x){
int n = nums.size();

if(n==1){
    if (nums[0]==x)return 0;
    else return -1;
}


int lo =1,hi=n-2,mid;
int pivot; 

//finding pivot smallest element 
while(lo<=hi){
    mid=lo/2+hi/2;
    if(mid==0)lo=mid+1;
    else if (mid==n-1)hi=mid-1;
    else if (nums[mid]<nums[mid+1] && nums[mid]<nums[mid-1]){pivot=mid;break;}
else if (nums[mid]<nums[mid+1] && nums[mid]>nums[mid-1]){
    if (nums[hi]<nums[mid])lo=mid+1;
    else hi=mid-1;
}
else {pivot=mid+1;break;}
 
}
if (pivot==-1){
    lo =0;hi =n-1;
while(lo<=hi){
    mid=lo/2 +hi/2;
    if (nums[mid]==x){
        return mid;
    }
    else if (nums[mid]<x) {
        lo=mid+1;
    }
    else hi =mid-1;
}
return -1;
}
//first 
if(x>=nums[0]&& x<nums[pivot-1]){
lo =0;hi =pivot-1;
while(lo<=hi){
    mid=lo/2 +hi/2;
    if (nums[mid]==x){
        return mid;
    }
    else if (nums[mid]<x) {
        lo=mid+1;
    }
    else hi =mid-1;
}}
else{
//second
lo =pivot;hi =n-1;
while(lo<=hi){
    mid=lo/2 +hi/2;
    if (nums[mid]==x){
        return mid;
    }
    else if (nums[mid]<x) {
        lo=mid+1;
    }
    else hi =mid-1;
}

}
return -1;
}

int main (){
    return 0;

}