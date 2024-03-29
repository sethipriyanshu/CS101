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

typedef vector<pair<string, string>> DataPairVector;

vector<string> splitString(const string &str, char delimiter)
{
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);
    while (getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

string removeQuotesFromString(const string &str)
{
    string result;
    for (char c : str)
    {
        if (c != '"')
        {
            result += c;
        }
    }
    return result;
}

pair<vector<string>, vector<DataPairVector>> readCsvFile(const string &filename)
{
    vector<string> headers;
    vector<DataPairVector> data;
    ifstream file(filename);
    string line;
    while (getline(file, line))
    {
        vector<string> values = splitString(line, ',');
        if (line.back() == ',')
        {
            values.push_back("");
        }
        if (headers.empty())
        {
            for (string &value : values)
            {
                headers.push_back(removeQuotesFromString(value));
            }
        }
        else
        {
            DataPairVector row;
            for (size_t i = 0; i < headers.size() && i < values.size(); ++i)
            {
                row.push_back(make_pair(headers[i], removeQuotesFromString(values[i])));
            }
            data.push_back(row);
        }
    }
    return make_pair(headers, data);
}

void printDataRow(const DataPairVector &row, int width)
{
    for (const auto &pair : row)
    {
        cout << setw(width) << pair.second;
    }
    cout << endl;
}

void printDataInfo(const vector<string> &headers, const vector<DataPairVector> &data, int width)
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
            printDataRow(row, width);
        }
    }
}

void printColumnNamesFromDataRow(const DataPairVector &row)
{
    for (const auto &pair : row)
    {
        cout << pair.first << endl;
    }
}

void searchData(const vector<string> &headers, const vector<DataPairVector> &data, const string &columnName, const string &value, int width)
{
    bool found = false;
    bool c = false;
    bool columnExists = false;
    for (const DataPairVector &row : data)
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
                if ((columnName == "*" || pair.first == columnName) && pair.second == value)
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
                    printDataRow(row, width);
                    found = true;
                    break;
                }
            }
        }
    }
    if (!found)
    {
        if (columnExists || columnName == "*")
        {
            cout << "No results" << endl;
        }
        else
        {
            cout << "Invalid column \"" << columnName << "\"" << endl;
        }
    }
}

void performCalculation(const vector<DataPairVector> &data, const string &columnName, const string &operation)
{
    double minVal = DBL_MAX;
    double maxVal = DBL_MIN;
    double sum = 0.0;
    double val = 0.0;
    int count = 0;
    bool ct = false;
    bool columnExists = false;
    for (const DataPairVector &row : data)
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
                        val = stod(pair.second);
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
            minVal = min(minVal, val);
            maxVal = max(maxVal, val);
            sum += val;
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

    pair<vector<string>, vector<DataPairVector>> dataRows = readCsvFile(filename);
    vector<string> headers = dataRows.first;
    vector<DataPairVector> data = dataRows.second;
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
            printDataInfo(headers, data, width);
        }
        else if (command == "cols")
        {
            printColumnNamesFromDataRow(data[0]);
        }
        else if (command == "search")
        {
            string columnName;
            string value;
            string input;
            getline(cin, input);
            istringstream iss(input);
            int quoteCount = count(input.begin(), input.end(), '\"');
            if (quoteCount == 4)
            {
                getline(iss, columnName, '\"');
                getline(iss, columnName, '\"');
                getline(iss, value, '\"');
                getline(iss, value, '\"');
            }
            else if (quoteCount == 2 && input[1] == '\"')
            {
                getline(iss, columnName, '\"');
                getline(iss, columnName, '\"');
                iss >> value;
            }
            else if (quoteCount == 2 && input[1] != '\"')
            {
                iss >> columnName;
                getline(iss, value, '\"');
                getline(iss, value, '\"');
            }
            else
            {
                iss >> columnName >> value;
            }
            searchData(headers, data, removeQuotesFromString(columnName), removeQuotesFromString(value), width);
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
            performCalculation(data, removeQuotesFromString(columnName), command);
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
