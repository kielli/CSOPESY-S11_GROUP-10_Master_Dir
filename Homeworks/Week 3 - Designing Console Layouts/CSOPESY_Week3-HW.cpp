#include <iostream>

using namespace std;

class Screen{
    string name;
    public:
        Screen(string screenName){
            name = screenName;
        }
        void createScreen(){
            system("start cmd");
        }
};

