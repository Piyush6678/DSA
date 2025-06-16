 //LeetCode 39
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;

void combination(  vector<vector<int>> &ans, vector<int> v,vector<int> c ,int target,int idx){ 
    if (target==0){
        ans.push_back(v);
        return;
    }
    if(target<0)return ;
    for(int i =idx;i<c.size();i++){
        v.push_back(c[i]);
        combination(ans,v,c,target-c[i],idx);
        v.pop_back();
    }

}


vector<vector<int>> combinationSum(vector<int> c,int trgt){
vector<vector<int>> ans ;
vector<int> v;
combination(ans,v,c,trgt,0);
return ans;
}
int main (){return 0;}