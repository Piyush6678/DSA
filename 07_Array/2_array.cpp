#include<iostream>
#include<vector>
#include <algorithm>

using namespace std;
void display(int a[]){
    for (int i =0;i<5;i++){
        cout<<a[i];
    }
}
void change(int b[]){
b[0]=100;
}
int lastOccurence(vector<int> v ,int trgt ){
int idx=-1;
    for (int i =v.size()-1;i>=0;i--){
if (v[i] ==trgt ) {idx =i;break;}
}return idx;
}
void rotateArray(vector<int> & v,int k ){
    k= k % v.size();
    reverse(v.begin(),v.end());
    reverse(v.begin(),v.begin()+(k));
    reverse(v.begin()+k,v.end());
}


int main(){
    //Passing array to fucntion
    int arr[5]={1,2,3,4,5};
    display(arr);  // adress of first element /array get passed
    change(arr);  
    display(arr);

    // Array and pointer
    int *ptr=arr;
    int *ptr2=&arr[0];

    //VECTOR
    vector<int> v;
    v.push_back(6);
    // v[1]=3;
    v.push_back(1);
    v.push_back(4);
    v.push_back(2);
    cout<<v[0]; //6
    cout<<v[1]; //1
    cout<<v[2]; //4
    cout<<v[3]; //2
    cout<<v[4]; //garbage value
    v[4]=3;
    cout<<v[4]; //3
v.push_back(4);
v.push_back(5);

//last occurrence of x in the aray
lastOccurence(v,4);

// reverse a array 
//Method 1 make another array ofsame size and just cop[y the elements in reverse order
//Method 2 two pointer method 
vector<int> r={1,2,3,4};
int p1=0,p2=r.size()-1;
while(p1<p2){
    r[p1]=r[p1]+r[p2];
    r[p2]=r[p1]-r[p2];
    r[p1]=r[p1]-r[p2];
    p1++;p2--;
}

// Rotate the array
vector<int> v3 ={1,2,3,4,5,6,7,8}; 
rotateArray(v3,5);  // {4,5,6,7,8,1,2,3}

    return 0;
}