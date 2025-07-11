#include <iostream>
using namespace std;
class A{
               public:
                   int a;  };
class B:  virtual public A{
      public:
        int b;  };
class C  :  virtual public A{           public:  int c;  };
class D :public B , public C {   
    //D has 2 instance of A
            public:
             int d;  
            void show(){
                // cout<<B::a;
                // cout<<C::a;
                  cout<<a;  
            }
            };
int main() {
    
    return 0;
}