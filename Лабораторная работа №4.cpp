#include "iostream"
#include "vector"
#include "math.h"
#include "algorithm"
#include "set"
#include "string"
#include "map"
#include "iomanip"

using namespace std;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                               Построение подполугруппы                                                                              */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int findNumber(char word, char* el, int N) { //Поиск номера буквы в элементах полугруппы
	for (int i = 0; i < N; i++)
		if (el[i] == word)
			return i;
}


bool associativity(char** mas, int N, char* el) { //Проверка свойства ассоциативности
	for (int a = 0; a < N; a++)
		for (int b = a; b < N; b++)
			for (int c = 0; c < N; c++)
				if (mas[findNumber(mas[a][b], el, N)][c] != mas[a][findNumber(mas[b][c], el, N)])
					return false;
	return true;
}


vector <char> obedSets(vector <char> A, vector <char> B) { //Объединяем 2 множества
	vector <char> res;
	res.insert(res.end(), A.begin(), A.end());
	res.insert(res.end(), B.begin(), B.end());
	sort(res.begin(), res.end());

	for (int i = 1; i < res.size(); i++)
		if (res[i] == res[i - 1]) {
			res.erase(res.begin() + i);
			i--;
		}

	return res;
}


void underhalfgroup() { //Находим подполугруппу
	int N;
	cout << "Введите количество элементов в полугруппе: ";
	cin >> N;
	char* halfgroup = new char[N];
	cout << "Введите полугруппу: ";
	for (int i = 0; i < N; i++)
		cin >> halfgroup[i];

	char** matrix = new char* [N];
	cout << "\nВведите таблицу Кэли: \n";
	for (int i = 0; i < N; i++) {
		matrix[i] = new char[N];
		for (int j = 0; j < N; j++)
			cin >> matrix[i][j];
	}

	if (!associativity(matrix, N, halfgroup)) {
		cout << "\nНе выполняется ассоциативность полугруппы! \n";
		return;
	}

	int M;
	cout << "\nВведите количество элементов подмножества: ";
	cin >> M;
	vector <char> underset;
	cout << "Введите подмножество: ";
	for (int i = 0; i < M; i++) {
		char x;
		cin >> x;
		underset.push_back(x);
	}
	sort(underset.begin(), underset.end());

	vector <char> underset_;
	vector <char> undersetNew;
	while (true) {
		underset_.resize(0);
		undersetNew.resize(0);

		int size = underset.size();
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				underset_.push_back(matrix[findNumber(underset[i], halfgroup, N)][findNumber(underset[j], halfgroup, N)]);
		undersetNew = obedSets(underset, underset_);

		if (underset == undersetNew)
			break;
		else
			underset = undersetNew;
	}

	cout << "\nПодполугруппа: ";
	for (int i = 0; i < underset.size(); i++)
		cout << underset[i] << " ";
	cout << endl;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                               Построение полугруппы матриц                                                                          */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
vector <vector <int>> multMatrix(vector <vector <int>> A, vector <vector <int>> B, int N) { //Перемножаем булевы матрицы
	vector <vector <int>> res(N);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			int el = 0;
			for (int a = 0; a < N; a++)
				el += A[i][a] * B[a][j];

			if (el > 0)		res[i].push_back(1);
			else	res[i].push_back(0);
		}
	}

	return res;
}


void halfgroupMatrix() { //Полугруппа матриц
	int N;
	cout << "\nРазмер матриц: ";
	cin >> N;

	int M;
	cout << "Количество булевых матриц: ";
	cin >> M;

	set <vector <vector <int>>> sets;
	map <char, vector <vector <int>>> matrixIn;
	map <vector <vector <int>>, char> matrixOut;
	int wordIn = 0;
	for (int i = 0; i < M; i++) {
		vector <vector <int>> matrix(N);
		cout << "\nВведите матрицу " << char(65 + wordIn) << ": \n";
		for (int j = 0; j < N; j++)
			for (int k = 0; k < N; k++) {
				int x;
				cin >> x;
				matrix[j].push_back(x);
			}

		matrixIn.insert(make_pair(char(65 + wordIn), matrix));
		matrixOut.insert(make_pair(matrix, char(65 + wordIn++)));
		sets.insert(matrix);
	}

	set <vector <vector <int>>> halfgroup;
	halfgroup = sets;
	int wordOut = 0;
	while (true) {
		for (auto i = sets.begin(); i != sets.end(); i++)
			for (auto j = sets.begin(); j != sets.end(); j++) {
				vector <vector <int>> newMatrix = multMatrix(*i, *j, N);

				if (halfgroup.find(newMatrix) == halfgroup.end()) {
					matrixIn.insert(make_pair(char(65 + wordIn + wordOut), newMatrix));
					matrixOut.insert(make_pair(newMatrix, char(65 + wordIn + wordOut++)));
					halfgroup.insert(newMatrix);
				}
			}

		if (halfgroup.size() == sets.size()) {
			cout << "\nПолугруппа булевых матриц: \n";
			vector <vector <int>> res;
			wordOut = 0;

			for (vector <vector <int>> res : sets) {
				cout << matrixOut[res] << endl;
				for (int j = 0; j < N; j++) {
					for (int k = 0; k < N; k++)
						cout << res[j][k] << " ";
					cout << endl;
				}
				cout << "\n\n";
			}

			cout << endl << "Таблица Кэли: \n";
			cout << " ";
			for (int i = 0; i < matrixOut.size(); i++)
				cout << setw(2) << char(65 + i);
			cout << endl;
			for (int i = 0; i < matrixOut.size(); i++) {
				cout << char(65 + i) << setw(2);
				for (int j = 0; j < matrixOut.size(); j++)
					cout << setw(2) << matrixOut[multMatrix(matrixIn[char(65 + i)], matrixIn[char(65 + j)], N)];
				cout << endl;
			}

			return;
		}
		else
			sets = halfgroup;
	}
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                             Построение полугруппы по порождающему множеству и определяющим соотношениям                                             */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
set <string> setOfR;
map <string, string> R;
set <string> S_cur, S, Scopy;
int maxRel = 0;
string cur_word;
vector <vector <string>> cayley;


vector <string> split(string word) {
	vector <string> splitedWord;
	for (int i = 0; i < word.size(); i++)
		splitedWord.push_back(word.substr(i, 1));

	return(splitedWord);
}


bool comp(string A, string B) {
	return (A.size() < B.size()) || (A.size() == B.size() && A < B);
}


void newEl(string word, bool flagCayley = false, int row = 0, int size = -1) {
	if (setOfR.find(word) != setOfR.end() && !flagCayley)
		return;

	vector <string> word_ = split(word);

	int substr = 2;
	while (substr <= maxRel) {
		for (int i = word_.size() - 1; i >= 0; i--) {
			if (substr > word.size() || substr > i + 1)
				break;

			string part = "";
			for (int j = i; j > i - substr; j--)
				part = word_[j] + part;

			if (R.find(part) != R.end()) {
				cur_word = "";
				vector <string> part_ = split(R[part]);

				if (!flagCayley)
					for (int k = 0; k < i - substr; k++)
						cur_word = cur_word + word_[k];
				else
					for (int k = 0; k < i - substr + 1; k++)
						cur_word = cur_word + word_[k];

				cur_word = cur_word + R[part];

				for (int k = i + 1; k < word_.size(); k++)
					cur_word = cur_word + word_[k];

				if (cur_word.size() < word.size() && !flagCayley) {
					setOfR.insert(word);
					setOfR.insert(cur_word);
					return;
				}
				else if (setOfR.find(cur_word) != setOfR.end() && !flagCayley)
					return;
				else if (flagCayley && Scopy.find(cur_word) != Scopy.end()) {
					if (cayley[row].size() < size)
						cayley[row].push_back(cur_word);
					return;
				}
				else {
					int check = cayley[row].size();
					newEl(cur_word, flagCayley, row, size);
					if (flagCayley && cayley[row].size() > check)
						return;
				}
			}

			if (setOfR.find(word) != setOfR.end() && !flagCayley)
				return;

			if (flagCayley && Scopy.find(word) != Scopy.end()) {
				if (cayley[row].size() <= size)
					cayley[row].push_back(word);
				return;
			}
		}
		substr++;
	}

	if (cur_word.size() == word.size() && cur_word < word && setOfR.find(word) == setOfR.end() && setOfR.find(cur_word) == setOfR.end()) {
		S.insert(cur_word);
		setOfR.insert(word);
		setOfR.insert(cur_word);
	}
	else {
		S.insert(word);
		setOfR.insert(word);
		setOfR.insert(cur_word);
	}
}


void halfgroupRel() {
	int N;
	cout << "\nКоличество букв в множестве: ";
	cin >> N;

	set <string> A;
	cout << "Алфавит: ";
	for (int i = 0; i < N; i++) {
		string a;
		cin >> a;
		A.insert(a);
		S.insert(a);
		setOfR.insert(a);
	}

	int M;
	cout << "\nКоличество определяющих отношений: ";
	cin >> M;

	cout << "Введите определяющие отношения: \n";
	for (int i = 0; i < M; i++) {
		string r1, r2;
		cin >> r1 >> r2;

		if (r1.size() > maxRel)
			maxRel = r1.size();
		if (r2.size() > maxRel)
			maxRel = r2.size();

		if (r1.size() < r2.size())
			R[r2] = r1;
		else if (r1.size() > r2.size())
			R[r1] = r2;
		else {
			if (r1 < r2)
				R[r2] = r1;
			else
				R[r1] = r2;
		}
	}

	int count = 1;
	S_cur = A;
	while (!S_cur.empty()) {
		count++;

		for (string s : S_cur)
			for (string letter : A)
				newEl(s + letter);

		S_cur.clear();
		for (auto s : S)
			if (s.size() == count)
				S_cur.insert(s);
	}
	
	vector <string> halfgroup;
	for (string s : S)
		halfgroup.push_back(s);
	sort(halfgroup.begin(), halfgroup.end(), comp);
	int hSize = halfgroup.size();

	cout << "\nПолугруппа: {";
	for (int i = 0; i < hSize; i++) {
		if (i == hSize - 1)
			cout << halfgroup[i] << "} \n";
		else
			cout << halfgroup[i] << " ";
	}

	cayley.resize(hSize);
	Scopy = S;
	for (int i = 0; i < hSize; i++)
		for (int j = 0; j < hSize; j++)
			newEl(halfgroup[i] + halfgroup[j], true, i, hSize);

	cout << endl << "Таблица Кэли: \n";
	cout << "     ";
	for (int i = 0; i < hSize; i++)
		cout << setw(5) << halfgroup[i];
	cout << endl;
	for (int i = 0; i < hSize; i++) {
		cout << setw(5) << halfgroup[i] << setw(5);
		for (int j = 0; j < hSize; j++)
			cout << cayley[i][j] << setw(5);
		cout << endl;
	}

	cout << endl;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                                        Главная функция                                                                              */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
	setlocale(LC_ALL, "ru");
	for (;;) {
		cout << "1 - Построить подполугруппу по таблице Кэли \n2 - Построить полугруппу по порождающему множеству бинарных отношений \n";
		cout << "3 - Построить полугруппу по порождающему множеству и определяющим соотношениям \n4 - Выход \n";
		int x;
		cin >> x;
		switch (x) {
		case 1:
			underhalfgroup();
			cout << endl;
			break;
		case 2:
			halfgroupMatrix();
			cout << endl;
			break;
		case 3:
			halfgroupRel();
			cout << endl;
			break;
		case 4:
			return 0;
		default:
			cout << "Incorrect. Try again! \n";
		}
	}
}