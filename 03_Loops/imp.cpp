#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Enter a number: ";
    cin >> n;
//Factorial
    if (n < 0) {
        cout << "Factorial is not defined for negative numbers." << endl;
    } else {
        long long factorial = 1;
        for (int i = 1; i <= n; ++i) {
            factorial *= i;
        }
        cout << "Factorial of " << n << " is " << factorial << endl;
    }
    
    //Fibonacci
    cout << "Fibonacci number at position " << n << " is ";
    if (n == 0) {
        cout << 0 << endl;
    } else if (n == 1) {
        cout << 1 << endl;
    } else {
        int a = 0, b = 1, fib;
        for (int i = 2; i <= n; ++i) {
            fib = a + b;
            a = b;
            b = fib;
        }
        cout << fib << endl;
    }

    int a ,b;
    cout<<"enter the number ";
    cin>>a>>b;
if(b==0)cout<<"1";
else{
    for(int i=2;i<=b;i++){
        a*=a;
    }
}
    return 0;
}