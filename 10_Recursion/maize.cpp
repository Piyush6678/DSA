#include<iostream>
using namespace std;

int maize( int i ,int j   ){

    if (i==1||j==1)return 1;
    return maize(i,j-1)+maize(i-1,j);
}
void printpath( int i ,int j ,string s  ){
if (i==0||j==0)return ;
    if (i==1 && j==1){
cout<<s<<endl;
    };
    
printpath(i,j-1,s+"D") ;printpath(i-1,j ,s+"R");
}



int main(){
    int now=maize(4,4);
    cout<<now;
    return 0; 
}