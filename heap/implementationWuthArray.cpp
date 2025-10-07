#include <iostream>
using namespace std;

class minHeap{
// if parent is at i left child at 2i and right child at 2i+1 
// if child is i parent at i/2
    int arr[101] ; // use index 1 to 100 for 100 elments 
    int top=1;

    void push(int val ){
        arr[top]=val;
        int i =top;
        while(i>1){
            if(arr[i/2]>arr[i]){
                swap(arr[i/2],arr[i]);
            }
            i=i/2;

        }top++;

}
void pop(){

}

};



int main() {
    
    return 0;
}