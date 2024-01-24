#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <iomanip>

using namespace std;

void HelpFunction(){
    // todo
}






int main(int argc, char* argv[]){

    if(argc !=2 ){
        cout << "Usage: program_name.exe columnwidth"<< endl;
        return 1;
    }

    string userCmd = "";
    
    while(userCmd != "quit"){
        cout << "Enter a command or \"help\" for a command list:" << endl;
        cin >> userCmd;
        // todo
    }
    

}
