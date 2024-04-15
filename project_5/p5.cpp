#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <unordered_map>
using namespace std;

struct TreeNode {
    int value;
    TreeNode *left, *right;

    TreeNode(int value) : value(value), left(nullptr), right(nullptr) {}
};

class HuffmanCoder {
public:
    TreeNode* establishTree(const vector<int>& inorder, const vector<int>& levelorder);
    string decodeText(const string& encoded, TreeNode* root);
     void readTraversalData(const string& filename, vector<int>& traversal);
private:
    TreeNode* establishTreeUtil(const vector<int>& inorder, int inStart, int inEnd,
                        unordered_map<int, int>& levelOrderMap);
    int findIndex(const vector<int>& arr, int start, int end, int value);
    
};

void HuffmanCoder::readTraversalData(const string& filename, vector<int>& traversal) {
    ifstream file(filename);
    string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            std::istringstream iss(line);
            int num;
            while (iss >> num) {
                traversal.push_back(num);
            }
        }
        file.close();
    } else {
        cerr << "Unable to open file: " << filename << endl;
    }
}

int HuffmanCoder::findIndex(const vector<int>& arr, int start, int end, int value) {
    for (int i = start; i <= end; i++) {
        if (arr[i] == value) {
            return i;
        }
    }
    return -1;
}

TreeNode* HuffmanCoder::establishTreeUtil(const vector<int>& inorder, int inStart, int inEnd,
                                unordered_map<int, int>& levelOrderMap) {
    if (inStart > inEnd) {
        return nullptr;
    }

    int minValueIndex = inStart;
    for (int i = inStart + 1; i <= inEnd; i++) {
        if (levelOrderMap[inorder[i]] < levelOrderMap[inorder[minValueIndex]]) {
            minValueIndex = i;
        }
    }

    TreeNode* root = new TreeNode(inorder[minValueIndex]);

    if (inStart == inEnd) {
        return root;
    }

    root->left = establishTreeUtil(inorder, inStart, minValueIndex - 1, levelOrderMap);
    root->right = establishTreeUtil(inorder, minValueIndex + 1, inEnd, levelOrderMap);

    return root;
}

TreeNode* HuffmanCoder::establishTree(const vector<int>& inorder, const vector<int>& levelorder) {
    unordered_map<int, int> levelOrderMap;
    for (size_t i = 0; i < levelorder.size(); i++) {
        levelOrderMap[levelorder[i]] = i;
    }
    return establishTreeUtil(inorder, 0, inorder.size() - 1, levelOrderMap);
}

string HuffmanCoder::decodeText(const std::string& encoded, TreeNode* root) {
    string decoded = "";
    TreeNode* current = root;
    for (char bit : encoded) {
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }
        if (!current->left && !current->right) {
            decoded += char(current->value);
            current = root;
        }
    }
    return decoded;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <inorder file> <levelorder file> <encoded text file>" << std::endl;
        return 1;
    }

    vector<int> inorder, levelorder;
    HuffmanCoder coder;
    coder.readTraversalData(argv[1], inorder);
    coder.readTraversalData(argv[2], levelorder);

    TreeNode* root = coder.establishTree(inorder, levelorder);

    string encodedText, line;
    ifstream encodedFile(argv[3]);
    while (getline(encodedFile, line)) {
        encodedText += line;
    }
    encodedFile.close();

    cout << coder.decodeText(encodedText, root);

    return 0;
}
