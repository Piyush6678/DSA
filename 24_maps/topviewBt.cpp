#include <iostream>
#include <queue>
#include <unordered_map>
using namespace std;
class TreeNode{
    public:
int val;
TreeNode* left;
TreeNode* right;
};

void topView(TreeNode* root){
unordered_map<int,int>m;
queue< pair < TreeNode*, int > > q;
pair<TreeNode*,int> r;
r.first=root;
r.second=0;
q.push(r);
m[0]=root->val;
while(q.size()){
TreeNode* temp=(q.front()).first;
int level=(q.front()).second;
q.pop();
if(m.find(level)==m.end())m[level]=temp->val;
if(temp->left){
    pair<TreeNode*,int> p;
p.first=root;
p.second=level-1;
q.push(p);
}
if(temp->right){
    pair<TreeNode*,int> p2;
p2.first=root;
p2.second=level+1;
q.push(p2);
}

}
int minLevel=INT16_MIN;
int maxLevel=INT16_MAX;
 for(auto x:m){
    int level=x.first;
    minLevel=min(minLevel,level);
    maxLevel=max(maxLevel,level);
 }
 for(int i =minLevel;i<=maxLevel;i++){
    cout<<m[i]<<" ";
 }cout<<endl;
}
int main() {
    
    return 0;
}