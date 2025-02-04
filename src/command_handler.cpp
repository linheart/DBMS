#include "../include/command_handler.h"
#include <filesystem>
#include <fstream>

using namespace std;

void split(string &str, string &left, string &right) {
  istringstream stream(str);
  char ch;

  while (stream >> ch && ch != '.') {
    left += ch;
  }

  while (stream >> ch && ch != ',') {
    right += ch;
  }
}

void printColumnNames(Array &names) {

  for (size_t i = 0; i < names.size(); i++) {
    string tName;
    string cName;
    split(names[i], tName, cName);
    cout << cName << '\t';
  }
  cout << endl;
}

void printTables(HT &table, Array &names, Array &lines, size_t index) {

  if (index == names.size()) {
    size_t size = lines.size();
    for (size_t i = 0; i < size; i++) {
      cout << lines[i] << '\t';
    }
    cout << endl;
    return;
  }

  string firstTable;
  string _;

  split(names[index], firstTable, _);

  Array columns;

  while (index + columns.size() < names.size()) {
    string tName;
    string cName;
    split(names[index + columns.size()], tName, cName);
    if (tName != firstTable) {
      break;
    }
    columns.append(cName);
  }

  size_t size = table[firstTable + columns[0]].size();
  for (size_t i = 0; i < size; i++) {
    Array nextLines = lines;
    for (size_t j = 0; j < columns.size(); j++) {
      nextLines.append(table[firstTable + columns[j]][i]);
    }
    printTables(table, names, nextLines, index + columns.size());
  }
}

void saveLine(HT &table, const string &tName, const string &line) {
  Array &names = table[tName];
  istringstream stream(line);
  string val;

  for (size_t i = 0; i < names.size(); i++) {
    getline(stream, val, ',');
    table[tName + names[i]].append(val);
  }
}

void editTable(HT &table, const string &tName, const string &cName,
               const string &target = "") {
  string path = table.name + '/' + table[tName].name + '/';
  for (int i = 1; filesystem::exists(path + char('0' + i) + ".csv"); i++) {
    ifstream file(path + char('0' + i) + ".csv");

    string line;
    if (i == 1) {
      getline(file, line);
    }

    while (getline(file, line)) {
      if (line[0]) {
        istringstream stream(line);
        string tmp;
        size_t colPos = table[tName].find(cName);

        for (size_t j = 0; j <= colPos; j++) {
          getline(stream, tmp, ',');
        }

        if (tmp == target || target == "") {
          saveLine(table, tName, line);
        }
      }
    }
    file.close();
  }
}

bool check(const string tName, const string cName, HT &table) {

  if (!tName[0] || !cName[0]) {
    cout << "Syntax error: Expected 'table1.column1'" << endl;
  } else if (table[tName].find(cName) == table[tName].size()) {
    cout << "Invalid error: There's no such table or column" << endl;
  } else if (!isTableFree(table.name, tName)) {
    cout << "This table is currently in use" << endl;
  } else {
    return true;
  }
  return false;
}

string extractWord(const string &str) {
  string result;
  for (char c : str) {
    if (isalnum(static_cast<unsigned char>(c))) {
      result += c;
    }
  }
  return result;
}

void copyLine(HT &src, HT &target, const string &tName, size_t pos) {
  Array &names = src[tName];

  size_t size = names.size();
  for (size_t i = 0; i < size; i++) {
    target[tName + names[i]].append(src[tName + names[i]][pos]);
  }
}

void removeLine(HT &table, const string &tName, size_t pos) {
  Array &names = table[tName];

  size_t size = names.size();
  for (size_t i = 0; i < size; i++) {
    table[tName + names[i]].remove(pos);
  }
}

void intersectionTables(HT &table1, HT &table2, const string tName1,
                        const string tName2) {
  string key1 = tName1 + table1[tName1][0];
  string key2 = tName2 + table2[tName2][0];

  for (size_t i = 0; i < table1[key1].size(); i++) {
    if (table2[key2].find(table1[key1][i]) == table2[key2].size()) {
      if (tName1 == tName2) {
        removeLine(table1, tName1, i);
      }
    } else if (tName1 != tName2) {
      copyLine(table2, table1, tName2, i);
    }
  }
}

void unionTables(HT &table1, HT &table2, const string tName1,
                 const string tName2) {
  string key1 = tName1 + table1[tName1][0];
  string key2 = tName2 + table2[tName2][0];

  for (size_t i = 0; i < table2[key2].size(); i++) {
    if (tName1 != tName2) {
      copyLine(table2, table1, tName2, i);
    } else if (table1[key1].find(table2[key2][i]) == table1[key1].size()) {
      copyLine(table2, table1, tName1, i);
    }
  }
}

void filter(HT &table, istringstream &stream, const string token,
            Array &usedTables, string &curTName) {
  string curToken;
  if (!(stream >> curToken)) {
    if (token == "WHERE" || token == "AND" || token == "OR") {
      cout << "Syntax error: Expected condition" << endl;
    }
    return;
  }

  string tName;
  string cName;
  split(curToken, tName, cName);

  if (!check(tName, cName, table)) {
    return;
  } else if (!(stream >> curToken) || curToken != "=" ||
             !(stream >> curToken)) {
    cout << "Syntax error: Expected 'table1.column1 = val'" << endl;
    return;
  }

  string val = extractWord(curToken);
  if (!val[0]) {
    cout << "Syntax error: Expected value" << endl;
    return;
  }

  editTable(table, tName, cName, val);
  usedTables.append(tName);
  curTName = tName;

  if (token == "AND") {
    return;
  }

  if (stream >> curToken) {
    HT tmp(table);
    for (size_t i = 0; i < tmp[tName].size(); i++) {
      tmp[tName + tmp[tName][i]].clear();
    }
    filter(tmp, stream, curToken, usedTables, curTName);
    if (curToken == "AND") {
      intersectionTables(table, tmp, tName, curTName);
      if (stream >> curToken) {
        filter(table, stream, curToken, usedTables, tName);
      }
    } else if (curToken == "OR") {
      unionTables(table, tmp, tName, curTName);
    }
  }
}

void selectHandler(HT &table, istringstream &stream) {
  string token;
  HT finaltable(table);
  Array names;
  stream >> token;
  do {
    string tName;
    string cName;
    split(token, tName, cName);

    if (!check(tName, cName, table)) {
      return;
    }
    names.append(token);

  } while (stream >> token && token != "FROM");

  if (token != "FROM") {
    cout << "Syntax error: Expected 'FROM token'" << endl;
    return;
  }

  while (stream >> token && token != "WHERE") {
    token = extractWord(token);
    if (!finaltable[token].size()) {
      cout << "Invalid error: This table does not exist" << endl;
      return;
    }
  }

  Array usedTables;
  if (token == "WHERE") {
    string tmp;
    filter(finaltable, stream, token, usedTables, tmp);
  }

  size_t size = names.size();
  for (size_t i = 0; i < size; i++) {
    string tName;
    string cName;

    split(names[i], tName, cName);

    if (usedTables.find(tName) == usedTables.size()) {
      editTable(finaltable, tName, cName);
    }
  }

  printColumnNames(names);

  Array lines;
  printTables(finaltable, names, lines, 0);
}

void insertHandler(HT &table, istringstream &stream) {
  string token;
  if (!(stream >> token) || token != "INTO" || !(stream >> token)) {
    cout << "Syntax error: Expected 'INTO table'" << endl;
    return;
  } else if (!table[token].size()) {
    cout << "Invalid error: This table does not exist" << endl;
    return;
  } else if (!isTableFree(table.name, token)) {
    cout << "This table is currently in use" << endl;
    return;
  }

  string tName = token;

  if (!(stream >> token) || token != "VALUES") {
    cout << "Syntax error: Expected 'VALUES'" << endl;
    return;
  }

  HT finalTable(table);
  string line;
  size_t size = finalTable[tName].size();

  for (size_t i = 0; i < size; i++) {
    if (!(stream >> token)) {
      cout << "Syntax error: " << size << " values were expected" << endl;
      return;
    }

    line += extractWord(token);
    (i == size - 1) ? line += '\n' : line += ',';
  }

  addLine(table, tName, line);
}

void deleteHandler(HT &table, istringstream &stream) {
  string token;
  if (!(stream >> token) || token != "FROM" || !(stream >> token)) {
    cout << "Syntax error: Expected 'FROM table'" << endl;
    return;
  } else if (!table[token].size()) {
    cout << "Invalid error: This table does not exist" << endl;
    return;
  } else if (!isTableFree(table.name, token)) {
    cout << "This table is currently in use" << endl;
    return;
  }

  string tName = token;

  if (!(stream >> token) || token != "WHERE") {
    cout << "Syntax error: Expected 'WHERE'" << endl;
    return;
  }

  HT finaltable(table);
  Array usedTables;
  string tmp;
  filter(finaltable, stream, token, usedTables, tmp);

  remLines(finaltable, tName);
}

void menu(HT &table, const string &str) {
  istringstream stream(str);
  string token;

  stream >> token;

  if (token == "SELECT") {
    selectHandler(table, stream);
  } else if (token == "INSERT") {
    insertHandler(table, stream);
  } else if (token == "DELETE") {
    deleteHandler(table, stream);
  } else if (token == "FREE") {
    /*Table *curTable = json.structure;*/
    /*while (curTable) {*/
    /*  unlockTable(json.name, curTable->name);*/
    /*  curTable = curTable->next;*/
    /*}*/

  } else if (token != "EXIT") {
    cout << "Wrong query" << endl;
  }
}
