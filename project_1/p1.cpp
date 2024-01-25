#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <iomanip>

using namespace std;

void HelpFunction(){
    cout << "command list:" << endl;
    cout << setw(13) << right << "print"<<endl;
    cout << setw(12) << right << "cols"<<endl;
    cout << setw(29) << right << "search *|col_name val"<<endl;
    cout << setw(20) << right << "min col_name"<<endl;
    cout << setw(20) << right << "max col_name"<<endl;
    cout << setw(20) << right << "avg col_name"<<endl;
}






int main(int argc, char* argv[]){

    if(argc !=2 ){
        cout << "Usage: program_name.exe columnwidth"<< endl;
        return 1;
    }

    string userCmd = "";
    HelpFunction();
    
    while(userCmd != "quit"){
        cout << "Enter a command or \"help\" for a command list:" << endl;
        cin >> userCmd;
        

    }
    

}
