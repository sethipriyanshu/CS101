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

void ComaFormatter(string filename){
 // helperfunction? potentially?
}


void PrintFunction(string filename, int columnWidth){
    ifstream inFile;
    inFile.open(filename);
    if(!inFile){
        cout << "Error: could not open file" << endl;
        return;
    }
    string line;
    while(getline(inFile, line)){
        stringstream ss(line);
        string item;
        while (getline(ss, item, ',')) {
            cout << setw(columnWidth) << right << item;
        }
        cout << endl;
    }
    inFile.close();
   
}

void ColFunction(){
    //todo
}




int main(int argc, char* argv[]){

    if(argc !=3 ){
        cout << "Usage: program_name.exe columnwidth"<< endl;
        return 1;
    }

    string userCmd = "";
    int columnWidth = stoi(argv[2]);
    string filename = argv[1];

    
    while(userCmd != "quit"){
        cout << "Enter a command or \"help\" for a command list:" << endl;
        cin >> userCmd;
        

    }
    

}
