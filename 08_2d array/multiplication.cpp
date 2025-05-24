#include<iostream>
using namespace std;
int main(){
    int r=4,n=3,c=4;
    int arr[r][n]={1,1,1,2,2,3,3,5,5,4,4,2};
    int brr[n][c]={2,2,2,2,5,5,5,5,7,7,7,7};
int res[r][c];
for (int i =0;i<r;i++){
    for (int j=0;j<c;j++){
       res[j][i]=0; 
        for (int k =0;k<n;k++){
res[j][i]+=(arr[r][k]*brr[k][c]);
        }
    }
}


    return 0;
}