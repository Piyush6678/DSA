#include<iostream>
using namespace std;
int main(){
    int arr[4][5]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    int minr=0,minc=0,maxr=3,maxc=4;
    while(minr<=maxr && maxc>=minc){

        //right
for(int i =minc;i<=maxc;i++){
    cout<<arr[minr][i];
}
minr++;
//Down
for(int i =minr;i<=maxr;i++){
    cout<<arr[i][maxc];
}
maxc--;

if (minr>maxr)break;
//Left
for(int i =maxc;i>=minc;i--){
    cout<<arr[maxr][i];
}
maxr--;
//up
for(int i =maxr;i>=minr;i--){
    cout<<arr[i][minc];
}
minc++;

    }

    return 0;
}