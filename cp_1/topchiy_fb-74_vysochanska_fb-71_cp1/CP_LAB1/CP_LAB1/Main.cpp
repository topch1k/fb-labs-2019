#include <iostream>
#include <map>
#include <fstream>
#include <Windows.h>
#include <string>
#include <io.h>
#include <set>
#include <cctype>
#include <ctime>
#include <iomanip>
using namespace std;

void initMapMono(map<char, double>& Letters, string buffer, set<char> Alphabet, int& LetterAmount) {
	char ch;
	for (int i = 0; i < buffer.length(); i++) {
		if (Alphabet.count(buffer[i])) {

			if (Letters.count(char(tolower(buffer[i])))) {
				Letters[char(tolower(buffer[i]))] += 1;
				LetterAmount++;
			}
			else {
				Letters.emplace(char(tolower(buffer[i])), 1);
				LetterAmount++;
			}
		}
		else continue;
	}
};

void initMapBigram(map<string, double>& BigramMap, string buffer, set <char>Alphabet, int &BigramAmount, bool Flag) {
	string Bigram;
	for (int i = 0; i < buffer.length(); i += 1 + Flag) {  //В зависимости от значения Flag изменяется шаг
		if (Alphabet.count(buffer[i]) && Alphabet.count(buffer[i + 1])) { // Если Flag = 0 - биграммы пересекаются 
			Bigram.push_back(tolower(buffer[i]));
			Bigram.push_back(tolower(buffer[i + 1]));						  // Flag = 1 - биграммы не пересекаются 
			if (BigramMap.count(Bigram)) {
				BigramMap[Bigram] += 1;
				Bigram.clear();
				BigramAmount++;
			}
			else {
				BigramMap.emplace(Bigram, 1);
				Bigram.clear();
				BigramAmount++;
			}
		}
		else continue;
	
	}
}
void Filter(string FilePath, set<char> Alphabet, string Destination) {
	ifstream fin(FilePath);
	ofstream fout(Destination);
	string buffer;
	if (fin.is_open()) {
		while (fin.peek() != EOF) {
			getline(fin, buffer);
			fin.seekg(fin.tellg());
			for (int i = 0; i < buffer.length(); i++) {
				if (Alphabet.count(buffer[i])) {
					fout << buffer[i];
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

void ShowMap(map<char, double>& Letters, int LetterAmount) {
	for (auto it = Letters.cbegin(); it != Letters.cend(); it++) {
		cout << "[ " << (*it).first << " : " << (*it).second / LetterAmount << " ]\n";
	}
}
void ShowMap(map<string, double>& Letters, int BigramAmount) {
	double Sum = 0;
	for (auto it = Letters.cbegin(); it != Letters.cend(); it++) {
		Sum += (*it).second;
		//cout << "[ " << (*it).first << " : " << (*it).second  / BigramAmount << " ]\n";
		cout <<  "" << (*it).first << " : " << setprecision(3) << (*it).second / BigramAmount <<  "\n";
		
	}
}

double BigramEntropy(map<string, double> BigramMap, int BigramAmount) {
	double Entropy = 0;
	for(auto it = BigramMap.cbegin(); it != BigramMap.cend(); it++) {
		Entropy += (-((*it).second / BigramAmount) * log2(((*it).second) / BigramAmount));
	}
	return Entropy / 2;
}

double MonogramEntropy(map<char, double> Map, int LetterAmount) {
	double Entropy = 0;
	for (auto it = Map.cbegin(); it != Map.cend(); it++) {
		Entropy += (-((*it).second / LetterAmount) * log2(((*it).second) / LetterAmount));
	}
	return Entropy;
}


int main() {
	unsigned int start_time = clock();
	setlocale(LC_ALL, "rus");
	set<char> Alphabet = { 'а', 'А', 'б', 'Б', 'в', 'В', 'г', 'Г', 'д', 'Д', 'е', 'Е', 'ё', 'Ё',
	 'ж', 'Ж', 'з', 'З', 'и', 'И', 'й', 'Й', 'к', 'К', 'л', 'Л', 'м', 'М', 'н', 'Н', 'о', 'О',
	 'п', 'П', 'р', 'Р', 'с', 'С', 'т', 'Т', 'у', 'У', 'ф', 'Ф', 'х', 'Х', 'ц', 'Ц', 'ч', 'Ч',
	 'ш', 'Ш', 'щ', 'Щ', 'ъ', 'Ъ', 'ы', 'Ы', 'ь', 'Ь', 'э', 'Э', 'ю', 'Ю', 'я', 'Я' };

	Alphabet.insert(' '); //Adding space to set 

	if (0) {
		if (Alphabet.count(' ')) {
			string Destination = "..\\..\\FilteredTextWithSpace.txt";
			Filter("..\\..\\Crime and Punishment.txt", Alphabet, Destination);

		}
		else {
			string Destination = "..\\..\\FilteredTextNoSpace.txt";
			Filter("..\\..\\Crime and Punishment.txt", Alphabet, Destination);
		}
	}
	/*-----------------------------------------------------*/
	/*------------- Работа с монограммами -----------------*/
	if (0) {
		ifstream fin("..\\..\\Crime and Punishment.txt");
		//ifstream fin("..\\..\\Document.txt");
		int LetterAmount = 0;
		map<char, double> Letters;
		if (fin.is_open()) {
			string buffer;
			while (fin.peek() != EOF) {	//Просматриваем, но не считываем последующий символ
				getline(fin, buffer);	//Чтение файла построчно
				//cout << buffer << endl;	
				fin.seekg(fin.tellg());	// Перемещение каретки на позицию следующую после конца строки
				initMapMono(Letters, buffer, Alphabet, LetterAmount);
				buffer.clear();
			}

		}
		else {
			cout << "Error. File isn't opened" << endl;
		}
		ShowMap(Letters, LetterAmount);
		cout << "LetterAmount: " << LetterAmount << endl;
		//cout << Count(Letters) << endl;
	double MonoEntropy;
	MonoEntropy = MonogramEntropy(Letters, LetterAmount);

	cout << "Monogram Entropy: " << MonoEntropy << endl;
	cout << "Redudancy: " << 1 -  MonoEntropy / log2(Alphabet.size()/2) << endl;
	cout << "*********************************************\n";
	}
	
	/*------------------ Работа с биграммами ------------- */
	if (1) {
		map<string, double> BigramMap;
		ifstream fin2;
		if (Alphabet.count(' ')) {
			 fin2.open("..\\..\\FilteredTextWithSpace.txt");
		}
		else {
			fin2.open("..\\..\\FilteredTextNoSpace.txt");
		}
		int BigramAmount = 0;
		if (fin2.is_open()) {
			string buffer;
			while (fin2.peek() != EOF) {	//Просматриваем, но не считываем последующий символ
				getline(fin2, buffer);	//Чтение файла построчно
				//cout << buffer << endl;	
				fin2.seekg(fin2.tellg());	// Перемещение каретки на позицию следующую после конца строки
				//5-й параметр Flag; при Flag = 0 - подсчет биграмм с пересечениями
				//						Flag = 1 - без пересечений
				//cout << buffer << endl;
				initMapBigram(BigramMap, buffer, Alphabet, BigramAmount, 0);	//No space - 4.13; With Space - 3.96 : Intersection
				buffer.clear();													//No space - 4.13; With Space - 3.96 : Without Intersection

			}
		}
		else {
			cout << "Error. File isn't opened" << endl;
		}
		fin2.close();
		ShowMap(BigramMap, BigramAmount);
		double BiEntropy;
		BiEntropy = BigramEntropy(BigramMap, BigramAmount);
		cout << "Bigram Entropy: " << BiEntropy << endl;
		cout << "Redudancy: " << 1 - BiEntropy / log2(Alphabet.size() / 2) << endl;
	}
	unsigned int end_time = clock();
	double search_time = (end_time - start_time) / 1000;
	cout << "Performing time: " << search_time << endl << endl;
	float f = 3.8765426;
	cout << setprecision(4) << f;
	system("pause");
}