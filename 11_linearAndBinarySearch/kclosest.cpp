#include<iostream>
#include<algorithm>

#include<vector>
using namespace std;
vector<int> closest(vector<int>nums,int  k,int x){
//finding the element
int n =nums.size(),idx=-1;
int lo=0,hi=n-1,mid;
while(lo<=hi){
 mid = lo + (hi - lo) / 2;

    if (nums[mid]==x){idx=mid ;break;}
    else if (nums[mid]<x)lo=mid+1;
    else hi=mid-1;
}
vector<int> ans;

if(idx!=-1){
    lo =idx-1;//lowerbound
    hi=idx+1; //upperbound
ans.push_back(nums[idx]);//push back x 
}
else{
hi = lo;
lo = lo - 1;
}

   while (ans.size() < k) {
        if (lo >= 0 && (hi >= n || abs(x - nums[lo]) <= abs(x - nums[hi]))) {
            ans.push_back(nums[lo]);
            lo--;
        } else if (hi < n) {
            ans.push_back(nums[hi]);
            hi++;
        } else {
            break; // both pointers out of bounds
        }
    }


//sorting the ans
sort(ans.begin(),ans.end());

return ans;


}
int main(){

    return 0;
}
