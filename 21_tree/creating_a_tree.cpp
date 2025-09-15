#include<iostream> 
#include<queue>
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


int main(){
int arr[]={1,2,3,4,5,-1,6,-1,-1,7,8,9};
int n =sizeof(arr)/sizeof(arr[0]);
Node* root=constructATree(arr,n);






}