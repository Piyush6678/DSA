#include<iostream>
using namespace std;
void PIP(int n ){
    if(n==0)return ;
    cout<<"PRE "<<n<<endl;
PIP(n-1);
cout<<"IN "<<n<<endl;
PIP(n-1);
    cout<<"POST "<<n<<endl;
}

int main(){
   PIP(3);
    return 0;

}