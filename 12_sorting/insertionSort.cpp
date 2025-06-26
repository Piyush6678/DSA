#include <iostream>
using namespace std;
void insertionSort(int arr[],int n ){

    //TC best O(n) avg/worst O(n^2)
    //Stable Sort 
    for (int i = 1; i < n; i++) {
        
        int j = i;

        
        while (j >= 1 && arr[j] > arr[j-1]) {
swap(arr[j],arr[j-1]);  // shift to right
            j--;
        }

        }
}
int main() {
    
    return 0;
}