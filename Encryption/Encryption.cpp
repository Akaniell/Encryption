#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <unordered_map>
#include <array>
#include <algorithm>
#include <string>

using namespace std;

void algorithmLZ77(const string& inputUser, int bufferSize, int dictionarySize) {
	vector<vector<string>> outputModel;
	int indexChar = 0;
    while (indexChar < inputUser.length()) {
       	int bestLength = 0;
       	int bestOffset = 0;
       	string nextChar = "";
       	int maxOffset = min(indexChar, dictionarySize);
       	int maxLength = min(static_cast<int>(inputUser.length()) - indexChar, bufferSize);
       	for (int offset = 1; offset <= maxOffset; offset++) {
           	for (int length = 1; length <= maxLength; length++) {
               	string stringWindow = inputUser.substr(indexChar - offset, length);
               	string stringBuffer = inputUser.substr(indexChar, length);
               	if (stringWindow == stringBuffer && length > bestLength) {
                   	bestLength = length;
                   	bestOffset = (offset - dictionarySize) * (-1);
                   	if (indexChar + length < inputUser.length()) {
                       	nextChar = inputUser.substr(indexChar + length, 1);
                   	}
               	}
           	}
       	}
       	if (indexChar == inputUser.length() - 1) {
           	outputModel.push_back({inputUser.substr(max(0, indexChar - dictionarySize), indexChar), inputUser.substr(indexChar, min(indexChar + bufferSize, static_cast<int>(inputUser.length()))),"<0,0," + inputUser.substr(indexChar, 1) + ">"});
       	} else {
           	outputModel.push_back({inputUser.substr(max(0, indexChar - dictionarySize), indexChar),inputUser.substr(indexChar, min(indexChar + bufferSize, static_cast<int>(inputUser.length()))),"<" + to_string(bestOffset) + "," + to_string(bestLength) + "," +(bestLength > 0 ? nextChar : inputUser.substr(indexChar, 1)) + ">"});
       	}
       	indexChar += bestLength + 1;
    }
    for (size_t i = 0; i < outputModel.size(); i++) {
    	string& str = outputModel[i][1];
    	if (str.length() > bufferSize) {
        	str = str.substr(0, bufferSize);
    	} else if (str.length() < bufferSize) {
        	int padding = bufferSize - str.length();
        	str += string(padding, ' ');
    	}
    	string& leng = outputModel[i][0];
    	if (leng.length() > dictionarySize) {
        	leng = leng.substr(0, dictionarySize);
    	} else if (leng.length() < dictionarySize) {
        	int padding = dictionarySize - leng.length();
        	leng = string(padding, ' ') + leng; 
    	}
    	cout<<setw(dictionarySize)<<right<<outputModel[i][0]<<"  "<<outputModel[i][1]<<"  "<<outputModel[i][2]<<'\n';
	}
}

void algorithmLZ78(string inputUser, int dictionarySize) {
    vector<string> dictionary;
    dictionary.push_back("");
    unordered_map<string, int> frequency;
    frequency[""] = 0;
    vector<array<string, 3>> outputModel;
    outputModel.push_back({"''", "", "0"});

    int i = 0;
    while (i < inputUser.length()) {
        string windowSymbols = string(1, inputUser[i]);
        int index = distance(dictionary.begin(), find(dictionary.begin(), dictionary.end(), windowSymbols));

        while (index != dictionary.size() && i < inputUser.length() - 1) {
            i++;
            windowSymbols += inputUser[i];
            index = distance(dictionary.begin(), find(dictionary.begin(), dictionary.end(), windowSymbols));
        }

        if (index == dictionary.size()) {
            string code = "<" + to_string(distance(dictionary.begin(), find(dictionary.begin(), dictionary.end(), windowSymbols.substr(0, windowSymbols.length() - 1)))) + ", '" + windowSymbols.back() + "'>";
            outputModel.push_back({ "'" + windowSymbols + "'", code, to_string(dictionary.size()) });
            if (dictionary.size() == dictionarySize) {
                auto lastUsed = min_element(frequency.begin(), frequency.end(),
                    [](const pair<const string, int>& p1, const pair<const string, int>& p2) { return p1.second < p2.second; });
                dictionary.erase(remove(dictionary.begin(), dictionary.end(), lastUsed->first), dictionary.end());
                frequency.erase(lastUsed->first);
            }
            dictionary.push_back(windowSymbols);
            frequency[windowSymbols] = 1;
        } else {
            frequency[windowSymbols]++;
        }
        i++;
    }
    string checkLastTwoSymbols = inputUser.length() > 1 ? inputUser.substr(inputUser.length() - 2) : "";
    string lastItemOutputModel = dictionary.back();
    if (lastItemOutputModel.find(checkLastTwoSymbols) == string::npos) {
        string lastSymbol = inputUser.substr(i - 1);
        string code = "<" + to_string(distance(dictionary.begin(), find(dictionary.begin(), dictionary.end(), ""))) + ", '" + lastSymbol + "'>";
        outputModel.push_back({ "'" + lastSymbol + "'", code, to_string(dictionary.size()) });
    }
    for (size_t i = 0; i < outputModel.size(); i++) {
    	cout<<setw(6)<< outputModel[i][0] << "  " <<setw(8)<< outputModel[i][1] << "  " << outputModel[i][2] << '\n';
	}
}

static void algorithmLZSS(string inputUser, const int bufferSize, const int dictionarySize) {
	vector<vector<string>> outputModel;
    string dictionary = "";
    string buffer = inputUser.substr(0, min(bufferSize, static_cast<int>(inputUser.length())));
    inputUser = inputUser.substr(buffer.length());
    while (!buffer.empty()) {
        int offset = 0;
        int length = 0;
        for (int i = 1; i <= buffer.length(); i++) {
            string subStr = buffer.substr(0, i);
            int position = dictionary.rfind(subStr);
            if (position != -1) {
                offset = dictionarySize - static_cast<int>(dictionary.length()) + position;
                length = subStr.length();
            } else {
                break;
            }
        }
        string code;
        int codeLength;
        if (length > 0) {
            code = "1<" + to_string(offset) + "," + to_string(length) + ">";
            codeLength = static_cast<int>(log2(dictionarySize)) + static_cast<int>(log2(bufferSize)) + 2;
        } else {
            code = "0'" + buffer.substr(0, 1) + "'";
            codeLength = 9;
        }
        outputModel.push_back({dictionary, buffer, code, to_string(codeLength)});
        int shiftSize = max(length, 1);
        dictionary += buffer.substr(0, shiftSize);
        if (dictionary.length() > dictionarySize) {
            dictionary = dictionary.substr(dictionary.length() - dictionarySize);
        }
        buffer = buffer.substr(shiftSize);
        if (buffer.length() < bufferSize && !inputUser.empty()) {
            int addSize = min(static_cast<int>(bufferSize - buffer.length()), static_cast<int>(inputUser.length()));
            buffer += inputUser.substr(0, addSize);
            inputUser = inputUser.substr(addSize);
        }
    }
    for (size_t i = 0; i < outputModel.size(); i++) {
    	cout<<setw(dictionarySize)<<right<< outputModel[i][0] << "  " <<setw(bufferSize)<<left<< outputModel[i][1] << "  " <<setw(7)<< outputModel[i][2] <<"  "<< outputModel[i][3]<< '\n';
	}
}

int main(){
	setlocale(LC_ALL,"Russian");
	string input;
	size_t dictionarySize, bufferSize;
	cout<<"Введите фразу: ";
    getline(cin, input);
	cout<<"Введите размер словаря (для всех методов): ";
    cin>>dictionarySize;
    cout<<"Введите размер буфера (для LZ77 и LZSS): ";
    cin>>bufferSize;
    cout<<"\nLZ77:\n";
	algorithmLZ77(input, bufferSize, dictionarySize);
	cout<<"\nLZ78:\n";
	algorithmLZ78(input, dictionarySize);
    cout<<"\nLZSS:\n";
	algorithmLZSS(input, bufferSize, dictionarySize);
	return 0;
}
