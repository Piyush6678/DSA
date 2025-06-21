//lc 633
#include<iostream>
#include<math.h>
using namespace std;
bool ifperfectsquare(int n ){
    int root = sqrt(n);
    if (root*root==n) return true;
    return false;
}
bool judgeSquareSum(int c ){
    int x=0,y=c;
    while(x<=y){
        if(ifperfectsquare(x) && ifperfectsquare(y)){
            return true;
        }
    else if (!ifperfectsquare(y)){
        y=(int)sqrt(y)*(int)sqrt(y);
        x=c-y;
    }
    else{
x=((int)sqrt(x)+1)*((int)sqrt(x)+1);
        y=c-x;
    }
    }
     return false;
}
int main(){
    return 0;
}