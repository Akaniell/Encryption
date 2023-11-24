#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct Token {
	int isLiteral;
	int code;
    int offset;
    int length;
    char nextChar;
};

vector<Token> compressLZ77(const string& input, const int& dictionarySize, const int& bufferSize) {
    vector<Token> compressedData;
    int inputSize = input.size();
    int currentIndex = 0;
    while (currentIndex < inputSize) {
        Token token;
        token.offset = 0;
        token.length = 0;
        token.nextChar = input[currentIndex + token.length];
        for (int i = std::max(0, currentIndex - dictionarySize); i < currentIndex; ++i) {
            int j = 0;
            while (j < bufferSize && currentIndex + j < inputSize &&
                   input[i + j] == input[currentIndex + j]) {
                ++j;
            }
            if (j > token.length) {
                token.offset = currentIndex - i;
                token.length = j;
                token.nextChar = (currentIndex + j < inputSize) ? input[currentIndex + j] : '\0';
            }
        }
        compressedData.push_back(token);
        currentIndex += token.length + 1;
    }
    return compressedData;
}

void compressLZ78(const string& input) {
    unordered_map<string, int> dictionary; 
    int nextCode = 1; 
    string currentPhrase;
    for (char c : input) {
        currentPhrase += c;
        if (dictionary.find(currentPhrase) == dictionary.end()) {
            cout<<"("<<dictionary[currentPhrase.substr(0, currentPhrase.length() - 1)]<<",'"<< c <<"') ";
            dictionary[currentPhrase] = nextCode++;
            currentPhrase = "";
        }
    }
    if (!currentPhrase.empty()) {
        cout<<"("<< dictionary[currentPhrase]<<"' ')";
    }
}

vector<Token> compressLZSS(const string& input, const int& dictionarySize, const int& bufferSize) {
    vector<Token> compressedData;
    int inputSize = input.size();
    int currentIndex = 0;
    int threshold = dictionarySize;
    while (currentIndex < inputSize) {
        Token token;
        token.isLiteral = 0;
        token.offset = 0;
        token.length = 0;
        token.nextChar = input[currentIndex];
        for (int i = std::max(0, currentIndex - dictionarySize); i < currentIndex; ++i) {
            int j = 0;
            while (j < bufferSize && currentIndex + j < inputSize &&
                   input[i + j] == input[currentIndex + j]) {
                ++j;
            }
            if (j > token.length) {
                token.isLiteral = 1;
                token.offset = currentIndex - i;
                token.length = j;
                token.nextChar = (currentIndex + j < inputSize) ? input[currentIndex + j] : '\0';
            }
        }
        if (token.isLiteral || token.length >= threshold) {
            compressedData.push_back(token);
        } else {
            Token literalToken;
            literalToken.isLiteral = 1;
            literalToken.offset = 0;
            literalToken.length = 0;
            literalToken.nextChar = token.nextChar;
            compressedData.push_back(literalToken);
        }
        currentIndex += max(1, token.length);
    }
    return compressedData;
}

vector<pair<int, char>> compressLZW(const string& input) {
    unordered_map<string, int> dictionary;
    vector<pair<int, char>> compressedData;
    int nextCode = 256;
    string currentPhrase;
    for (char c : input) {
        currentPhrase += c;
        if (dictionary.find(currentPhrase) == dictionary.end()) {
            compressedData.push_back({dictionary[currentPhrase.substr(0, currentPhrase.length() - 1)], c});
            dictionary[currentPhrase] = nextCode++;
            currentPhrase = c;
        }
    }
    compressedData.push_back({dictionary[currentPhrase], '\0'});
    return compressedData;
}

int main() {
    setlocale(LC_ALL,"Russian");
    string input, tempInput;
    size_t dictionarySize, bufferSize, threshold;
    cout<<"Введите фразу: ";
    getline(cin, input);
    tempInput=input;
    cout<<"Введите размер словаря: ";
    cin>>dictionarySize;
    cout<<"Введите размер буфера: ";
    cin>>bufferSize;
    cout<<"\nLZ77: ";
    vector<Token> compressedDataLZ77 = compressLZ77(input, dictionarySize, bufferSize);
    for (const Token& token : compressedDataLZ77) {
        cout<<"("<<token.offset<<","<<token.length<<",'"<<token.nextChar<<"') ";
    }
    cout<<"\nLZ78: ";
    compressLZ78(input);
    cout<<"\nLZSS: ";
    vector<Token> compressedDataLZSS = compressLZSS(input, dictionarySize, bufferSize);
    for (const Token& token : compressedDataLZSS) {
        if (token.isLiteral) {
            cout<<"("<<token.offset<<","<<token.length<<",'"<<token.nextChar<<"') ";
        } else {
            cout<<"("<<token.nextChar<<") ";
        }
    }
    cout<<"\nLZW: (0-255, ASCII+)";
    vector<pair<int, char>> compressedDataLZW = compressLZW(input);
    for (const auto& entry : compressedDataLZW) {
        cout<<"("<<entry.first<<",'"<<entry.second<<"') ";
    }
    return 0;
}
