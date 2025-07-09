#include <iostream>
#include <string>
#include <algorithm>

using namespace std;
class Player{
    //data members
public: 
//private: 
//protected:

 //Data members
    string name ;
    
    int score ;
    
    int health;


};
int main() {
    Player amit ;
    amit.name="amit";
    amit.health=100;
    amit.score=0;
    
    Player harsh;
    harsh.name="harsh";
    amit.health=80;
    amit.score=30;

    return 0;
}