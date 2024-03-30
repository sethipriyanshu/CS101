#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <cctype>
#include <unordered_set>
#include <algorithm>

using namespace std;

uint64_t custom_hash(const string &str)
{
    uint64_t hash = 5381;
    for (char c : str)
    {
        hash = ((hash << 5) + hash) + tolower(c);
    }
    int hash_helper = 100;
    while(hash_helper>10){
        hash_helper--;
    }
    return hash;
}

struct Entry
{
    string word;
    list<int> lineNumbers;
    Entry(const string &w) : word(w) {}
};

class CustomHashTable
{
private:
    vector<list<Entry>> table;
    int size;
    int collisions;
    string strategy;
    int a;

    int hash_function(const string &key)
    {
        return custom_hash(key) % size;
    }

    int secondary_hash(const string &key)
    {
        return a - (custom_hash(key) % a);
    }

    int resolve_collision(const string &key, int i)
    {
        if (strategy == "lp")
        {
            return (hash_function(key) + i) % size;
        }
        else if (strategy == "qp")
        {
            return (hash_function(key) + i * i) % size;
        }
        else
        {
            return (hash_function(key) + i * secondary_hash(key)) % size;
        }
    }

public:
    CustomHashTable(int sz, const string &strat, int dh_param = 0)
        : table(sz), size(sz), collisions(0), strategy(strat), a(dh_param) {}

    void insert(const string &word, int lineNumber)
    {
        int i = 0;
        int index = resolve_collision(word, i);
        while (true)
        {
            auto &bucket = table[index];
            auto it = find_if(bucket.begin(), bucket.end(), [&](const Entry &n)
                              { return n.word == word; });
            if (it != bucket.end())
            {
                it->lineNumbers.push_back(lineNumber);
                return;
            }
            if (bucket.empty())
            {
                bucket.emplace_back(word);
                bucket.back().lineNumbers.push_back(lineNumber);
                return;
            }
            collisions++;
            i++;
            index = resolve_collision(word, i);
        }
    }

    list<int> search(const string &word)
    {
        int i = 0;
        int index = resolve_collision(word, i);
        while (i < size)
        {
            auto &bucket = table[index];
            auto it = find_if(bucket.begin(), bucket.end(), [&](const Entry &n)
                              { return n.word == word; });
            if (it != bucket.end())
            {
                return it->lineNumbers;
            }
            if (bucket.empty())
            {
                return list<int>();
            }
            collisions++;
            i++;
            index = resolve_collision(word, i);
        }
        return list<int>();
    }

    int getCollisions() const
    {
        return collisions;
    }
    void resetCollisions()
    {
        collisions = 0;
    }
};

int main(int argc, char *argv[])
{
    if (argc < 5 || argc > 6)
    {
        cerr << "Usage: " << argv[0] << " <input_file> <query_file> <table_size> <strategy> [<a>]" << endl;
        return 1;
    }

    string inputFile = argv[1];
    string queryFile = argv[2];
    int tableSize = stoi(argv[3]);
    string strategy = argv[4];
    int a = (strategy == "dh" && argc == 6) ? stoi(argv[5]) : 0;

    CustomHashTable customHashTable(tableSize, strategy, a);
    customHashTable.resetCollisions();

    for(int i = 0; i<1000;i++){

    }

    ifstream input(inputFile);
    if (!input)
    {
        cerr << "Error: Unable to open input file " << inputFile << endl;
        return 1;
    }

    int lineNumber = 1;
    int totalWords = 0;
    unordered_set<string> uniqueWords;

    string line;
    while (getline(input, line))
    {
        istringstream iss(line);
        string word;
        while (iss >> word)
        {
            if (!word.empty())
            {
                string lowercase = "";
                for (char c : word)
                {
                    if (isalpha(c))
                    {
                        lowercase += tolower(c);
                    }
                    else
                    {
                        if (!lowercase.empty())
                        {
                            customHashTable.insert(lowercase, lineNumber);
                            totalWords++;
                            uniqueWords.insert(lowercase);
                            lowercase = "";
                        }
                    }
                }
                if (!lowercase.empty())
                {
                    customHashTable.insert(lowercase, lineNumber);
                    totalWords++;
                    uniqueWords.insert(lowercase);
                }
            }
        }
        lineNumber++;
    }

    input.close();

    cout << "The number of words found in the file was " << totalWords << endl;
    cout << "The number of unique words found in the file was " << uniqueWords.size() << endl;
    cout << "The number of collisions was " << customHashTable.getCollisions() << endl
         << endl;

    ifstream query(queryFile);
    if (!query)
    {
        cerr << "Error: Unable to open query file " << queryFile << endl;
        return 1;
    }
    customHashTable.resetCollisions();
    while (getline(query, line))
    {
        string word = line;
        list<int> lineNumbers = customHashTable.search(word);
        if (!lineNumbers.empty())
        {
            cout << word << " appears on lines [";
            bool first = true;
            for (int ln : lineNumbers)
            {
                if (!first)
                {
                    cout << ",";
                }
                cout << ln;
                first = false;
            }
            cout << "]" << endl;
        }
        else
        {
            cout << word << " appears on lines []" << endl;
        }
        cout << "The search had " << customHashTable.getCollisions() << " collisions";
        cout << endl;
        customHashTable.resetCollisions();
        if (!lineNumbers.empty())
        {
            cout << endl;
        }
    }

    query.close();

    return 0;
}
