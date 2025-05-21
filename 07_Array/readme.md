# Leet Code Questions
- 1 Two sum 
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        vector<int> ans;
        for (int i=0;i<nums.size();i++){
for (int j=i+1;j<nums.size();j++){
    if (nums[i]+nums[j]==target) {
        ans.push_back(i);
        ans.push_back(j);
        return ans;
    }
}
        }
        return ans;
    }
};
- 2 merge two sorted array 
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int i =m-1,j=n-1,k=m+n-1;
        while(i>=0 &&j>=0){
            if(nums1[i]>nums2[j]){
                nums1[k]=nums1[i];
                i--;k--;
            }
            else{
                nums1[k]=nums2[j];
                k--;j--;
            }

        }
        while(j>=0){
            nums1[k]=nums2[j];
            j--;k--;
        }


    }
};
- 3 nextpermutation 31
- 4 traping rain water 42 