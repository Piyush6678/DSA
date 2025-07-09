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

    //members function
    void showHealth (){
        cout<<endl<<name<<"'s Health is "<< health;
    }
    void showScore (){
        cout<<endl<<name<<"'s Score is "<< score;
    }

};
int main() {
    Player amit ;
    amit.name="amit";
    amit.health=100;
    amit.score=0;
    
    Player harsh;
    harsh.name="harsh";
    harsh.health=80;
    harsh.score=30;

amit.showHealth();
amit.showScore();
    return 0;
}