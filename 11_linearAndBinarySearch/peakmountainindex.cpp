#include<iostream>
#include<vector>
using namespace std;
int peak (vector<int> nums){
    int n=nums.size();
    int lo=1,hi=n-2,mid;
    while(lo<=hi){
        mid=lo/2 +hi/2;
        if(nums[mid]>nums[mid+1] && nums[mid]>nums[mid-1] ){return mid;

        }
        else if(nums[mid]>nums[mid-1])lo=mid+1;
        else hi=mid-1;
    }
    return -1;
    
}
int main(){
    return 0;
}
