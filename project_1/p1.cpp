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

void ColFunction(string filename){  
    ifstream inFile;
    inFile.open(filename);
    if(!inFile){
        cout << "Error: could not open file" << endl;
        return;
    }
    string line;
    getline(inFile, line);
    stringstream ss(line);
    string item;
    while (getline(ss, item, ',')) {
        cout << item << endl;
    }
    inFile.close();

}


void SearchRowFunction(string filename, string col_name, string val, int columnWidth){
    ifstream inFile;
    inFile.open(filename);
    if(!inFile){
        cout << "Error: could not open file" << endl;
        return;
    }
    string line;
    getline(inFile, line);
    stringstream ss(line);
    string item;
    int col_index = -1;
    int i = 0;
    if (col_name != "*") {
        while (getline(ss, item, ',')) {
            if (item == col_name) {
                col_index = i;
            }
            cout << setw(columnWidth) << right << item;
            i++;
        }
        cout << endl;
        if (col_index == -1) {
            cout << "Column not found" << endl;
            return;
        }
    } else {
        while (getline(ss, item, ',')) {
            cout << setw(columnWidth) << right << item;
        }
        cout << endl;
    }
    bool found = false;
    while(getline(inFile, line)){
        stringstream ss(line);
        string item;
        int i = 0;
        bool match = false;
        while (getline(ss, item, ',')) {
            if ((col_name == "*" && item == val) || (i == col_index && item == val)) {
                match = true;
                break;
            }
            i++;
        }
        if (match) {
            found = true;
            stringstream ss(line);
            while (getline(ss, item, ',')) {
                cout << setw(columnWidth) << right << item;
            }
            cout << endl;
        }
    }
    if (!found) {
        cout << "No results" << endl;
    }
    inFile.close();
}


void MinFunction(string filename, string col_name){
    ifstream inFile;
    inFile.open(filename);
    if(!inFile){
        cout << "Error: could not open file" << endl;
        return;
    }
    string line;
    getline(inFile, line);
    stringstream ss(line);
    string item;
    int col_index = -1;
    int i = 0;
    while (getline(ss, item, ',')) {
        if (item == col_name) {
            col_index = i;
        }
        i++;
    }
    if (col_index == -1) {
        cout << "Column not found" << endl;
        return;
    }
    double min = 0;
    bool first = true;
    while(getline(inFile, line)){
        stringstream ss(line);
        string item;
        int i = 0;
        while (getline(ss, item, ',')) {
            if (i == col_index) {
                double val = stod(item);
                if (first || val < min) {
                    min = val;
                    first = false;
                }
            }
            i++;
        }
    }
    cout<<"The min for col "<< "\""<<col_name <<"\"" <<" = " << min << endl;
    inFile.close();
}

void MaxFunction(string filename, string col_name){
    ifstream inFile;
    inFile.open(filename);
    if(!inFile){
        cout << "Error: could not open file" << endl;
        return;
    }
    string line;
    getline(inFile, line);
    stringstream ss(line);
    string item;
    int col_index = -1;
    int i = 0;
    while (getline(ss, item, ',')) {
        if (item == col_name) {
            col_index = i;
        }
        i++;
    }
    if (col_index == -1) {
        cout << "Column not found" << endl;
        return;
    }
    double max = 0;
    bool first = true;
    while(getline(inFile, line)){
        stringstream ss(line);
        string item;
        int i = 0;
        while (getline(ss, item, ',')) {
            if (i == col_index) {
                double val = stod(item);
                if (first || val > max) {
                    max = val;
                    first = false;
                }
            }
            i++;
        }
    }
    cout<<"The max for col "<<"\""<<col_name <<"\"" <<" = " << max << endl;
    inFile.close();
}

void AvgFunction(string filename, string col_name){
    ifstream inFile;
    inFile.open(filename);
    if(!inFile){
        cout << "Error: could not open file" << endl;
        return;
    }
    string line;
    getline(inFile, line);
    stringstream ss(line);
    string item;
    int col_index = -1;
    int i = 0;
    while (getline(ss, item, ',')) {
        if (item == col_name) {
            col_index = i;
        }
        i++;
    }
    if (col_index == -1) {
        cout << "Column not found" << endl;
        return;
    }
    double sum = 0;
    int count = 0;
    while(getline(inFile, line)){
        stringstream ss(line);
        string item;
        int i = 0;
        while (getline(ss, item, ',')) {
            if (i == col_index) {
                sum += stod(item);
                count++;
            }
            i++;
        }
    }
    cout<<"The avg for col "<<"\""<<col_name <<"\"" <<" = " << sum/count << endl;
    inFile.close();
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
        
        if(userCmd == "help"){
            HelpFunction();
        }
        else if(userCmd == "print"){
            PrintFunction(filename, columnWidth);
        }
        else if(userCmd == "cols"){
            ColFunction(filename);
        }
        else if(userCmd == "search"){
            string col_name;
            string val;
            cin >> col_name >> val;
            SearchRowFunction(filename, col_name, val, columnWidth);
        }
        else if(userCmd == "min"){
            string col_name;
            cin >> col_name;
            MinFunction(filename, col_name);
        }
        else if(userCmd == "max"){
            string col_name;
            cin >> col_name;
            MaxFunction(filename, col_name);
        }
        else if(userCmd == "avg"){
            string col_name;
            cin >> col_name;
            AvgFunction(filename, col_name);
        }
        else if(userCmd == "quit"){
            cout << "Goodbye!" << endl;
        }
        else{
            cout << "Invalid command" << endl;
        }

    }
    

}


// issues:
// prints out column not found in search "First name" "Mac" (is read first name and mac differently as commands)
// prints the column names in search age 100 in addition to column not found message
// fix all error messages according to the project description
// error on avg empty and program stops running