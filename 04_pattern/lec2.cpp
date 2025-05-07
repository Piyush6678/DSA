#include<iostream>
using namespace std;
int main(){
//Star Pattern 
    
    //Rhombus
    int r1=4,c1=4;
    for (int i =1;i<=r1;i++){
        for (int j =1;j<=r1-i;j++){
            cout<<" ";

        }
        for (int k =1; k<=c1;k++){
            cout<<"X";

        }
        cout<<endl;

    }
    
    //Star Pyramid
     int r2=4;
     for (int i =1;i<=r2;i++){
        //Spaces
        for(int j=1;j<=r2-1;j++){
            cout<<" ";
        }
        for (int k =1;k<=(2*i -1);k++){
            cout<<"X";
        }cout<<endl;

     }


 




//Number Pattern 
    // Number Pyramid Palindrome
    //          1
    //        1 2 1
    //      1 2 3 2 1
    //    1 2 3 4 3 2 1
    int r3 = 4;
    for (int i = 1; i <= r3; i++) {
        // Spaces
        for (int j = 1; j <= r3 - i; j++) {
            cout << " ";
        }
        // Increasing numbers
        for (int k = 1; k <= i; k++) {
            cout << k;
        }
        // Decreasing numbers
        for (int k = i - 1; k >= 1; k--) {
            cout << k;
        }
        cout << endl;
    }

    //Diamond
    int n =4,nsp=n-1,nst=1;
    for (int i =1;i<2*n - 1;i++){
        //Spaces 
        for(int j =1;j<=nsp;j++){
            cout<<" ";
        }
        if(i<=n-1) nsp--;
        else nsp++;
        
        //Stars
        for (int i =1;i<=nst;i++){
cout<<"X";
        }
        if(i<=n-1)nst+=2;
        else nst-=2;
        cout<<endl;
    }

    //Number Spiral 
    int N =4,A=0,B=0;
    for (int i =0;i<(2*N)-1;i++){
B=0;
        for (int j =0;j<(2*N)-1;j++){
if(A<=B){
    cout<<N-A<<" ";
}
else cout<<N-B<<" ";
if(j<N-1)B++;
else B--;
        }cout<<endl;
        if(i<N-1)A++;
        else A--;
    }



//Alphabet Pattern
    return 0;
}