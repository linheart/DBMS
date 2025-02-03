#include "../include/json_parser.h"

#include <fstream>
#include <sstream>

using namespace std;

int stringToInt(istringstream &stream) {
  char ch;
  int num = 0;
  while (stream >> ch && !isdigit(ch))
    ;

  do {
    num = num * 10 + ch - '0';
  } while (stream >> ch && ch != ',');

  return num;
}

string retrieveValue(istringstream &stream) {
  string val;
  char ch;

  while (stream >> ch && ch != '\"')
    ;

  while (stream >> ch && ch != '\"') {
    val += ch;
  }
  return val;
}

void retrieveColumns(HT &table, istringstream &stream, const string &key) {
  table[key].append(key + "_pk");
  string col = retrieveValue(stream);
  while (col[0]) {
    table[key].append(col);
    col = retrieveValue(stream);
  }
}

Array JsonParser(HT &table) { // возвращает названия таблиц
  Array names;

  ifstream file("schema.json");

  if (file.is_open()) {
    string line;
    while (getline(file, line)) {
      istringstream stream(line);

      string key = retrieveValue(stream);
      if (key == "name") {
        table.name = retrieveValue(stream);
      } else if (key == "tuples_limit") {
        table.tuples_limit = stringToInt(stream);
      } else if (key[0] && key != "structure") {
        names.append(key);
        table[key].name = key;
        retrieveColumns(table, stream, key);
      }
    }
  }

  file.close();
  return names;
}
