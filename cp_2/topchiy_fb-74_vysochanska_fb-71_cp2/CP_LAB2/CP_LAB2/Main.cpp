#include<iostream>
#include<string>
#include<map>
#include<set>
#include<fstream>
#include"windows.h"
using namespace std;

map<char, int> CharIntMap = { {'�', 0}, {'�', 1}, {'�', 2}, {'�', 3}, {'�', 4}, {'�', 5}, {'�', 6}, {'�', 7}, {'�', 8},
							 {'�', 9}, {'�', 10}, {'�', 11}, {'�', 12}, {'�', 13}, {'�', 14}, {'�', 15}, {'�', 16},
							 {'�', 17}, {'�', 18}, {'�', 19}, {'�', 20}, {'�', 21}, {'�', 22}, {'�', 23}, {'�', 24},
							 {'�', 25}, {'�', 26}, {'�', 27}, {'�', 28}, {'�', 29}, {'�', 30}, {'�', 31} };

map<int, char> IntCharMap = { {0, 'a'}, {1, '�'}, {2, '�'}, {3, '�'}, {4, '�'}, {5, '�'}, {6, '�'}, {7, '�'}, {8, '�'},
							 {9, '�'}, {10, '�'}, {11, '�'}, {12, '�'}, {13, '�'}, {14, '�'}, {15, '�'}, {16, '�'},
							 {17, '�'}, {18, '�'}, {19, '�'}, {20, '�'}, {21, '�'}, {22, '�'}, {23, '�'}, {24, '�'},
							 {25, '�'}, {26, '�'}, {27, '�'}, {28, '�'}, {29, '�'}, {30, '�'}, {31, '�'} };

map<int, char> CharFreqMap = { {0, '�'}, {1, '�'}, {2, '�'}, {3, '�'}, {4, '�'}, {5, '�'}, {6, '�'}, {7, '�'}, {8, '�'},
							  {9, '�'}, {10, '�'}, {11, '�'}, {12, '�'}, {13, '�'}, {14, '�'}, {15, '�'}, {16, '�'},
							  {17, '�'}, {18, '�'}, {19, '�'}, {20, '�'}, {21, '�'}, {22, '�'}, {23, '�'}, {24, '�'},
							  {25, '�'}, {26, '�'}, {27, '�'}, {28, '�'}, {29, '�'}, {30, '�'}, {31, '�'} };


set<char> Alphabet = { '�', '�', '�', '�', '�', '�', '�',
							'�', '�', '�', '�', '�', '�', '�', '�', '�',
							'�', '�', '�', '�', '�', '�', '�', '�', '�',
							'�', '�', '�', '�', '�', '�', '�', '�' };

map<int, string> Keys = { {2, "��"}, {3, "���"}, {4, "����"}, {5, "�����"}, {19, "�������������������"} };

void FilterText(string, set<char>); //Text filter function

void Encrypt(string, string, string, map<char, int>, map<int, char>); //File Encryption function

double FileCI(string); //Counting coincidence index for text from file

double GetStringCI(string); //Get coincidence index of string

double SubStringCI(map<int, string>);	//Counting coincidence index for substrings of text

void SubString(string, map<int, string>&, int); //Get substrings of file function

char GetMaxFreqLetter(string); //Get the most frequent letter from string

void GetKey(string, int); //Get Key function

void Decrypt(string, string);
int main() {
	setlocale(LC_ALL, "rus");
	//FilterText("..\\..\\RawText.txt", Alphabet);
	/*for (auto it = Keys.cbegin(); it != Keys.cend(); it++) {
		Encrypt("..\\..\\FilteredText.txt", "", it->second, CharIntMap, IntCharMap);
	}*/
	//Encrypt("..\\..\\FilteredText.txt", "..\\..\\EncryptedText.txt", "��������", CharIntMap, IntCharMap);
	cout << "CoincidenceIndex of Raw Text: " << FileCI("..\\..\\FilteredText.txt") << endl;
	for (auto it = Keys.cbegin(); it != Keys.cend(); it++) {
		cout << it->first<< " " << it->second << "	: " << FileCI("..\\..\\EncryptedTextKey" + to_string(it->first) + ".txt");
		cout << endl;
	}
	map<int, string> SubStringMap;
	for (int i = 2; i < 31; i++) {
		SubString("..\\..\\EncryptedTextVariant18.txt", SubStringMap, i);
		cout << "SubStrings CI for r = " << i << " : " << SubStringCI(SubStringMap) << endl;
		SubStringMap.clear();
	}
	GetKey("..\\..\\EncryptedTextVariant18.txt", 15);

	Decrypt("..\\..\\EncryptedTextVariant18.txt", "���������������");
	system("pause");
	return 0;
}




void FilterText(string FilePath, set<char>Alphabet) {
	ifstream fin(FilePath);
	ofstream fout("..\\..\\FilteredText.txt");
	string buffer;
	if (fin.is_open()) {
		while (fin.peek() != EOF) {
			getline(fin, buffer);
			fin.seekg(fin.tellg());
			//cout << buffer << endl;
			for (int i = 0; i < buffer.length(); i++) {
				if (Alphabet.count(buffer[i]) || Alphabet.count(char(tolower(buffer[i])))) {
					//fout << buffer[i];
					if (buffer[i] == '�' || buffer[i] == '�') {
						fout << '�';
					}
					else {
						fout << char(tolower(buffer[i]));

					};
				}
				else {
					continue;
				}
			}
			buffer.clear();
		}
	}
	fin.close();
	fout.close();
}

void Encrypt(string FilePath, string DestinationPath, string Key, map<char, int> CharIntMap, map<int, char> IntCharMap) {
	//ifstream fin(FilePath);
	ifstream fin;
	fin.open(FilePath);
	ofstream fout;
	if (DestinationPath.size() == 0) {
		fout.open("..\\..\\EncryptedTextKey" + to_string(Key.length()) + ".txt");
	}
	else {
		fout.open(DestinationPath);
	}
	string buffer;
	int EncNumber;
	char EncChar;
	for (int i = 0; i < Key.length(); i++) {
		if (Key[i] == '�' || Key[i] == '�') {
			Key[i] = '�';
		}
		else Key[i] = char(tolower(Key[i]));
	}
	if (fin.is_open()) {
		while (fin.peek() != EOF) {
			getline(fin, buffer);
			fin.seekg(fin.tellg());
			for (int i = 0; i < buffer.length(); i++) {
				EncNumber = (CharIntMap.find(buffer[i])->second + CharIntMap.find(Key[i % Key.length()])->second) % CharIntMap.size();
				EncChar = IntCharMap.find(EncNumber)->second;
				fout << EncChar;
			}
			buffer.clear();
		}
	}
	else cout << "ifstream Error\n";

}

double FileCI(string FilePath) {
	ifstream fin(FilePath);
	map<char, double> LettersMap;
	string buffer;
	double LettersAmount = 0;
	if (fin.is_open()) {
		while (fin.peek() != EOF) {
			getline(fin, buffer);
			fin.seekg(fin.tellg());
			//cout << buffer << endl;
			for (int i = 0; i < buffer.length(); i++) {
				if (LettersMap.count(buffer[i])) {
					LettersMap.find(buffer[i])->second++;
					LettersAmount++;
				}
				else {
					LettersMap.emplace(buffer[i], 1);
					LettersAmount++;
				}
			}
			buffer.clear();
		}
	}
	else cout << "ifstream Error\n";
	double CoincidenceIndex = 0;
	for (auto it = LettersMap.cbegin(); it != LettersMap.cend(); it++) {
		//cout << it->first << "	" << it->second << " " << it->second - 1 << endl;
		CoincidenceIndex += (it->second * (it->second - 1));
		//cout << CoincidenceIndex << endl;
	}
	CoincidenceIndex *= 1 / (LettersAmount * (LettersAmount - 1));
	fin.close();
	return CoincidenceIndex;
}

void SubString(string FilePath, map<int, string> & SubStringMap, int KeyLength) {
	ifstream fin(FilePath);
	string buffer;
	if (fin.is_open()) {
		while (fin.peek() != EOF) {
			getline(fin, buffer);
			fin.seekg(fin.tellg());
			for (int i = 0; i < buffer.length(); i++) {
				if (SubStringMap.count(i % KeyLength)) {
					SubStringMap.at(i % KeyLength) += buffer[i];
				}
				else {
					string s(1, buffer[i]);
					SubStringMap.emplace(i % KeyLength, s);
				}
			}
		}
		buffer.clear();
		/*for (auto it = SubStringMap.cbegin(); it != SubStringMap.cend(); it++) {
			cout << it->first << "  " << it->second << endl;
		}*/
	}
	else cout << "istream Error" << endl;
}

double GetStringCI(string String) {
	map<char, double> LettersMap;
	double LettersAmount = 0;
	for (int i = 0; i < String.length(); i++) {
		if (LettersMap.count(String[i])) {
			LettersMap.at(String[i])++;
			LettersAmount++;
		}
		else {
			LettersMap.emplace(String[i], 1);
			LettersAmount++;
		}
	}
	double CoincidenceIndex = 0;
	for (auto it = LettersMap.cbegin(); it != LettersMap.cend(); it++) {
		//cout << it->first << "	" << it->second << " " << it->second - 1 << endl;
		CoincidenceIndex += (it->second * (it->second - 1));
		//cout << CoincidenceIndex << endl;
	}
	CoincidenceIndex *= 1 / (LettersAmount * (LettersAmount - 1));
	return CoincidenceIndex;
}

double SubStringCI(map<int, string>SubStringMap) {
	double CoincidenceIndex = 0;
	for (auto it = SubStringMap.cbegin(); it != SubStringMap.cend(); it++) {
		CoincidenceIndex += GetStringCI(it->second);
		//cout << GetStringCI(it->second) << endl;
	}
	return CoincidenceIndex / SubStringMap.size();
}

char GetMaxFreqLetter(string String) {
	map<char, int> LettersMap;
	for (int i = 0; i < String.length(); i++) {
		if (LettersMap.count(String[i])) {
			LettersMap.at(String[i])++;
		}
		else {
			LettersMap.emplace(String[i], 1);
		}
	}
	char MaxFreqChar;
	int MaxFreq = 0;
	for (auto it = LettersMap.cbegin(); it != LettersMap.cend(); it++) {
		if (it->second >= MaxFreq) {
			MaxFreq = it->second;
			MaxFreqChar = it->first;
		}
		else continue;
	}
	return MaxFreqChar;
}

void GetKey(string FilePath, int KeyLength) {
	map<int, string> SubStringMap;
	SubString(FilePath, SubStringMap, KeyLength);
	string Key;
	char chKi;
	int iKi = 0;
	for (auto it = CharFreqMap.cbegin(); it != CharFreqMap.cend(); it++) {
		for (auto it2 = SubStringMap.cbegin(); it2 != SubStringMap.cend(); it2++) {
			iKi = 0;
			iKi = CharIntMap.find(GetMaxFreqLetter(it2->second))->second - CharIntMap.find(it->second)->second;
			cout << "Most frequ�ncy letter in " << it2->first << " " << GetMaxFreqLetter(it2->second) << endl;
			if (iKi < 0) {
				iKi += CharIntMap.size();
				iKi %= CharIntMap.size();
				chKi = IntCharMap.find(iKi)->second;
				Key.push_back(chKi);
			}
			else {
				iKi %= CharIntMap.size();
				chKi = IntCharMap.find(iKi)->second;
				Key.push_back(chKi);
			}
		}
		cout << Key << endl;
		Key.clear();
	}
}

void Decrypt(string FilePath, string Key) {
	ifstream fin(FilePath);
	ofstream fout("..\\..\\DecryptedTextKey" + to_string(Key.length()) + ".txt");
	for (int i = 0; i < Key.length(); i++) {
		Key[i] = char(tolower(Key[i]));
	}
	string buffer;
	char EncChar;
	if (fin.is_open()) {
		while (fin.peek() != EOF) {
			getline(fin, buffer);
			fin.seekg(fin.tellg());
			//cout << buffer << endl;
			for (int i = 0; i < buffer.length(); i++) {
				EncChar = IntCharMap.find((CharIntMap.find(buffer[i])->second + CharIntMap.size() - Key[i % Key.length()]) % CharIntMap.size())->second;
				fout << EncChar;
			}

		}
		buffer.clear();
	}
	else cout << "ifstream Error" << endl;
}

