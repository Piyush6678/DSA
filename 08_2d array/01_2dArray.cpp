#include<iostream>
using namespace std;
int main(){
    //While initializing a 2D array it is necessary to mention the second (column) dimension, whereas the first dimension (row) is optional.
int arr[3][4]={{1,2,3},{4,5,6},{7,8,9},{10,11,12}};
//or
int brr[4][3]={1,2,3,4,5,6,7,8,9,10,11,12};

//write a program to store roll numbers and marks of student 
int crr[5][2];  // 5 row 2 column
int r=5,c=2;
for (int i =0;i<r;i++){
    for (int j =0;j<2;j++){
        if(j){
            cout <<"enter the marks of the student ";
            cin>>crr[i][j];
            
        }
        else {
            cout <<"enter the roll number of  the student ";
            cin>>crr[i][j];
        }
    }
}

    //largesst element 
    int max =-999;
    for (int i =0;i<3;i++){
        for (int j =0 ;j<4;j++){
if (arr[i][j]>max) max=arr[i][j];
        }
    }
    
    //sum of all elemnts
    
    int sum =0;
    for (int i =0;i<3;i++){
        for (int j =0 ;j<4;j++){
            sum+=arr[i][j];
        }
    }
    
    // transpose of a matrix
    //m1 using additional matrices
    for (int i =0;i<4;i++){
        for (int j =0 ;j<3;j++){
            brr[i][j]=arr[j][i]; 
        }
    }

// for square matrix 
int r=4;
int trr[4][4]={1,2,3,45,6,7,8,9,10,12,13,14,15,16};
for (int i =0;i<r;i++){
    for (int j =0;j<r;j++){
        if(i==j)break;
        int temp=trr[j][i];
        trr[j][i]=trr[i][j];
        trr[i][j]=temp;

    }
}

    return 0;
    

}