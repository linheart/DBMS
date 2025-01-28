#include "../include/json_parser.h"

#include <cassert>
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
  while (stream) {
    table[key].append(retrieveValue(stream));
  }
}

void JsonParser(HT &table) {
  ifstream file("schema.json");
  assert(file);

  string line;
  while (getline(file, line)) {
    istringstream stream(line);

    string key = retrieveValue(stream);
    if (key == "name") {
      table.name = retrieveValue(stream);
    } else if (key == "tuples_limit") {
      table.tuples_limit = stringToInt(stream);
    } else if (key[0] && key != "structure") {
      retrieveColumns(table, stream, key);
    }
  }

  file.close();
}
