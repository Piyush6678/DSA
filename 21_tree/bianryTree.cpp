#include<iostream> 
#include<queue>
#include<stack>
#include<vector>
using namespace std;
class Node{
public:
int val;
Node* left;
Node* right;
Node(int val) 
{
this->val=val;
left=NULL;
right=NULL;
}
};


// bfs using queue

void bfslOrderQueue(Node* root){
    queue<Node*>q;
    q.push(root);
    while(q.size()>0){
        Node* temp= q.front();
        cout<<temp->val<<"->";
        q.pop();
        if(temp->left)q.push(temp->left);
        if(temp->right)q.push(temp->right);
    }
    cout<<endl; 
}


//construct a tree from level order traversal from bfs

Node* constructATree (int arr[],int n ){
queue<Node*>tree;
Node* root = new Node(arr[0]);
tree.push(root);
int i =1;int j =2;
while(tree.size()>0  && i<n){
    Node*temp=tree.front();
    tree.pop();
    Node* l;
    Node* r;
    if(arr[i]!=-1){
        l=new Node(arr[i]);

    }
    else{
        l=NULL;
    }
    if(j!=n &&  arr[j]!=-1){
        r=new Node(arr[j]);

    }
    else{
        r=NULL;
    }
    temp->left=l;temp->right=r;
    if(l)tree.push(l);
    if(r)tree.push(r);
    i+=2;
    j+=2;
}

return root;

}

// pre order as iterative
void peroderIterative(Node* root){
    stack<Node*> st;
  if(root )  st.push(root);
    while(st.size()>0){
        Node* temp=st.top();
        st.pop();
        cout<<temp->val<<"->";
        if(temp->right!=NULL)st.push(temp->right);
        if(temp->left!=NULL)st.push(temp->left);
    }
}
// post order as iterative
void postIterative(Node* root){
    stack<Node*> st;
    vector<int>ans;
    stack<Node*> helper;
  if(root)  st.push(root);
    while(st.size()>0){
        Node* temp=st.top();
        st.pop();
       ans.push_back(temp->val);
        if(temp->left!=NULL)st.push(temp->left);
        if(temp->right!=NULL)st.push(temp->right);
    }
for(int i =ans.size()-1;i>-1;i--){
    cout<<ans[i]<<"->";
}

}
// in order as iterative
void inOrderIterative(Node* root){
    stack<Node*> st;
    vector<int>ans;
    Node* temp=root;
   

    while(st.size()>0  || temp ){
      if(temp){
        st.push(temp);
        temp=temp->left;
      }else{
        Node* temp2 =st.top();
        st.pop();
        ans.push_back(temp2->val);
        temp=temp2->right;
      }
    }
for(int i =0-1;i<ans.size();i++){
    cout<<ans[i]<<"->";
}
}

//Boundary traversal
void leftBoundary(Node* root){
if(!root)return;
if(!root->left && !root->right)return;
cout<<root->val<<"->";
 leftBoundary( root->left);
 if(!root->left) leftBoundary( root->right);
}
void bottomBoundary(Node* root){
    if(!root)return ;
    if(!root->left && !root->right)   {cout<<root->val<<"->"; return;}


 bottomBoundary( root->left);
 bottomBoundary( root->right);
}
void rightBoundary(Node* root){
    if(!root)return;
if(!root->left && !root->right)return;
rightBoundary( root->right);
if(!root->right) rightBoundary( root->left);
cout<<root->val<<"->";
}

void boundary(Node* root){
leftBoundary(root);
bottomBoundary(root);
rightBoundary(root->right);

}
int main(){
int arr[]={1,2,3,4,5,-1,6,-1,-1,7,8,9};
int n =sizeof(arr)/sizeof(arr[0]);
Node* root=constructATree(arr,n);






}