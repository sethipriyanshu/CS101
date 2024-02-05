#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <iomanip>
#include <vector>
#include <map>
#include <cctype>
#include <cfloat>
#include <algorithm>
#include <iomanip>

using namespace std;

typedef vector<pair<string, string> > DataRow;

vector<string> split(const string &s, char delimiter)
{
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

string removeQuotes(const string &s)
{
    string result;
    for (char c : s)
    {
        if (c != '"')
        {
            result += c;
        }
    }
    return result;
}

pair<vector<string>, vector<DataRow> > readCsv(const string &filename)
{
    vector<string> headers;
    vector<DataRow> data;
    ifstream file(filename);
    string line;
    while (getline(file, line))
    {
        vector<string> values = split(line, ',');
        if (line.back() == ',')
        {
            values.push_back("");
        }
        if (headers.empty())
        {
            for (string &value : values)
            {
                headers.push_back(removeQuotes(value));
            }
        }
        else
        {
            DataRow row;
            for (size_t i = 0; i < headers.size() && i < values.size(); ++i)
            {
                row.push_back(make_pair(headers[i], removeQuotes(values[i])));
            }
            data.push_back(row);
        }
    }
    return make_pair(headers, data);
}

void printRow(const DataRow &row, int width)
{
    for (const auto &pair : row)
    {
        cout << setw(width) << pair.second;
    }
    cout << endl;
}

void printData(const vector<string> &headers, const vector<DataRow> &data, int width)
{
    if (!data.empty())
    {
        for (const auto &header : headers)
        {
            cout << setw(width) << header;
        }
        cout << endl;
        for (const auto &row : data)
        {
            printRow(row, width);
        }
    }
}
void printColumnNames(const DataRow &row)
{
    for (const auto &pair : row)
    {
        cout << pair.first << endl;
    }
}

void search(const vector<string> &headers, const vector<DataRow> &data, const string &columnName, const string &value, int width)
{
    bool found = false;
    bool c = false;
    bool columnExists = false;
    for (const DataRow &row : data)
    {
        
        for (const auto &pair : row)
        {
            if (pair.first == columnName)
            {
                columnExists = true;
                break;
            }
        }
        if (columnName == "*" || columnExists)
        {

            for (const auto &pair : row)
            {
                if ( (columnName == "*" || pair.first == columnName) && pair.second == value)
                {
                    if (!c)
                    {
                        c = true;
                        for (const auto &header : headers)
                        {
                            cout << setw(width) << header;
                        }
                        cout << endl;
                    }
                    printRow(row, width);
                    found = true;
                    break;
                }
            }
        }
    }
    if (!found)
    {
        if (columnExists || columnName == "*") {
            cout << "No results" << endl;
        }
        else
        {
            cout << "Invalid column \"" << columnName << "\"" << endl;
        }
    }
}

void calculate(const vector<DataRow> &data, const string &columnName, const string &operation)
{
    double minVal = DBL_MAX;
    double maxVal = DBL_MIN;
    double sum = 0.0;
    double value = 0.0;
    int count = 0;
    bool ct = false;
    bool columnExists = false;
    for (const DataRow &row : data)
    {
        
        for (const auto &pair : row)
        {
            if (pair.first == columnName)
            {
                columnExists = true;
                break;
            }
        }
        if (columnExists)
        {

            for (const auto &pair : row)
            {
                if (pair.first == columnName)
                {
                    try
                    {
                        value = stod(pair.second);
                    }
                    catch (const invalid_argument &)
                    {
                        ct = true;
                    }
                }
            }
            if (ct)
            {
                ct = false;
                continue;
            }
            minVal = min(minVal, value);
            maxVal = max(maxVal, value);
            sum += value;
            ++count;
        }
    }
    if (count > 0)
    {
        if (operation == "min")
        {
            cout << "The min for col \"" << columnName << "\" = " << minVal << endl;
        }
        else if (operation == "max")
        {
            cout << "The max for col \"" << columnName << "\" = " << maxVal << endl;
        }
        else if (operation == "avg")
        {
            cout << "The avg for col \"" << columnName << "\" = " << sum / count << endl;
        }
    }
    else if (columnExists)
    {
        cout << "The " << operation << " for col \"" << columnName << "\" = N/A" << endl;
    }
    else
    {
        cout << "Invalid column \"" << columnName << "\" " << endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: ./a database.csv #col_width" << endl;
        return 1;
    }

    string filename = argv[1];
    int width = stoi(argv[2]);

    pair<vector<string>, vector<DataRow> > dr = readCsv(filename);
    vector<string> headers = dr.first;
    vector<DataRow> data = dr.second;
    if (data.empty())
    {
        cout << "Failed to open \"" << filename << "\"" << endl;
        return 1;
    }

    string command;
    cout << "Enter a command or \"help\" for a command list:" << endl;
    while (cin >> command)
{
    if (command == "quit")
    {
        break;
    }
    else if (command == "help")
    {
        cout << "command list:\n\tprint\n\tcols\n\tsearch *|col_name val\n\tmin col_name\n\tmax col_name\n\tavg col_name" << endl;
    }
    else if (command == "print")
    {
        printData(headers, data, width);
    }
    else if (command == "cols")
    {
        printColumnNames(data[0]);
    }
    else if (command == "search")
    {
        string columnName;
        string value;
        string input;
        getline(cin, input);
        istringstream iss(input);
        int quoteC = count(input.begin(), input.end(), '\"');
        if (quoteC == 4)
        {
            getline(iss, columnName, '\"');
            getline(iss, columnName, '\"');
            getline(iss, value, '\"');
            getline(iss, value, '\"');
        }
        else if (quoteC == 2 && input[1] == '\"')
        {
            getline(iss, columnName, '\"');
            getline(iss, columnName, '\"');
            iss >> value;
        }
        else if (quoteC == 2 && input[1] != '\"')
        {
            iss >> columnName;
            getline(iss, value, '\"');
            getline(iss, value, '\"');
        }
        else
        {
            iss >> columnName >> value;
        }
        search(headers, data, removeQuotes(columnName), removeQuotes(value), width);
    }
    else if (command == "min" || command == "max" || command == "avg")
    {
        string columnName;
        string input;
        getline(cin, input);
        istringstream iss(input);
        if (count(input.begin(), input.end(), '\"') >= 2)
        {
            getline(iss, columnName, '\"');
            getline(iss, columnName, '\"');
        }
        else
        {
            iss >> columnName;
        }
        calculate(data, removeQuotes(columnName), command);
    }
    else
    {
        cout << "Invalid command" << endl;
        cin.ignore(100000, '\n');
    }
    cout << "\nEnter a command or \"help\" for a command list:" << endl;
}

    return 0;
}