#include <iostream>
#include <cctype>  // For tolower() and isalpha()
#include <unordered_map>  // For character frequency map
using namespace std;

bool isPalindrome(string str) {
    string cleanStr = "";
    for (char ch : str) {
        if (isalpha(ch)) {
            cleanStr += tolower(ch);
        }
    }
    int left = 0, right = cleanStr.length() - 1;
    while (left < right) {
        if (cleanStr[left] != cleanStr[right]) return false;
        left++;
        right--;
    }
    return true;
}
void countCharacterFrequency(string str) {
    unordered_map<char, int> freq;
    for (char ch : str) {
        if (isalpha(ch)) {  // Only consider letters
            char lowerCh = tolower(ch);
            freq[lowerCh]++;
        }
    }
    cout << "Character frequencies:" << endl;
    for (auto pair : freq) {
        cout << pair.first << ": " << pair.second << endl;
    }
}
string replaceVowels(string str) {
    string result = str;
    string vowels = "aeiouAEIOU";

    for (char &ch : result) {
        if (vowels.find(ch) != string::npos) {
            ch = '*';
        }
    }
    return result;
}
int main() {
    string str;
    cout << "Enter a string: ";
    getline(cin, str);
    if (isPalindrome(str))
        cout << "The string is a palindrome." << endl;
    else
        cout << "The string is not a palindrome." << endl;

    countCharacterFrequency(str);
    string modifiedStr = replaceVowels(str);
    cout << "String after replacing vowels: " << modifiedStr << endl;

    return 0;
}