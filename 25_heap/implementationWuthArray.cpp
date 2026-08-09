#include <iostream>
using namespace std;

class minHeap{
// if parent is at i left child at 2i and right child at 2i+1 
// if child is i parent at i/2
public:
    int arr[101] ; // use index 1 to 100 for 100 elments 
    int top;
    minHeap(){
        top=1;
    }
     
    int top(){
        if(top==1)return -100;
        return arr[1];
    }
    void push(int val ){
        arr[top]=val;
        int i =top;
        while(i>1){
            if(arr[i/2]>arr[i]){
                swap(arr[i/2],arr[i]);
                 i=i/2;
            }
            else break;
           

        }top++;

}
void pop(){
if(top==1)  { cout<<"heap is empty" ;return;} 

top--;
int i =1;

arr[i]=arr[top];
while(1){

    int l=2*i;
int r=2*i +1;
if( l>top-1  )break; 
if(r>top-1){
if(arr[i]>arr[l]){swap(arr[i],arr[l]);
i=l;} break;
}
if(arr[l]<arr[r] ){
if(arr[i]>arr[l]){swap(arr[i],arr[l]);
i=l;}else break;
}
else{
if(arr[i]>arr[r]){swap(arr[i],arr[r]);
i=r;}else break;
}


}

}};



int main() {
    
    return 0;
}