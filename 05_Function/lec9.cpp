#include<iostream>
using namespace std;
void greet(){
    cout<<"Hello World!"<<endl;
}
void greet2(string name){
    cout<<"Hello "<<name<<"!"<<endl;
}
int sum(int a, int b){
    return a+b;
}
int mini(int a,int b ){
    return a<b?a:b;
}
int factorial(int x ){
    if (x==0|| x==1){
        return 1 ;

    }
    else {
        return x*factorial(x-1);    
    }
}
//Pascal Triangle
void printPascalsTriangle(int n) {

    for (int i = 0; i < n; i++) {
    int coefficient = 1;
    // Print spaces for alignment
    for (int space = 1; space <= n - i; space++)
    cout << " ";
    for (int j = 0; j <= i; j++) { 
    // Print the current coefficient
  
    cout << coefficient << " ";
    // Calculate the next coefficient using the formula C(n, k) = C(n, k-1) * (n-k)/(k+1)
    // Update the coefficient for the next iteratio
    coefficient = coefficient * (i - j) / (j + 1);
    }
    cout <<endl;
    }
    
    }
    // Function to swap two numbers

void swapNumbers(int &a, int &b) {

    int temp = a;
    
    a = b;
    
    b = temp;
    
    }
    int combination(int n, int r){
        int nCr=factorial(n)/(factorial(r)*factorial(n-r));
        return nCr;
    }
    
    
 int gcd(int a, int b) {
    // Euclidean algorithm

while (b != 0) {

int temp = b;

b = a % b;

a = temp;

}

return a;

}



int main(){
    



    return 0;

}