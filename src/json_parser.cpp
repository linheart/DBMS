#include "../include/json_parser.h"

#include <cassert>
#include <fstream>
#include <sstream>

Json JsonParser() {
  Json json;

  ifstream file("schema.json");
  assert(file);

  string line;

  while (getline(file, line)) {
    retrieveKeyValue(json, line, file);
  }

  file.close();
  return json;
}

void retrieveKeyValue(Json &json, const string &str, ifstream &file) {
  istringstream stream(str);
  string key;

  key = retrieveValue(stream);

  if (key == "name") {
    json.name = retrieveValue(stream);
  } else if (key == "tuples_limit") {
    json.tuples_limit = stringToInt(stream);
  } else if (key == "structure") {
    string line;
    while (getline(file, line) && line != "  }") {
      addTable(json, line);
    }
  }
}

void addTable(Json &json, const string &str) {
  Table *newTable = new Table;
  istringstream stream(str);
  string key = retrieveValue(stream);
  string val;

  newTable->name = key;
  do {
    val = retrieveValue(stream);
    if (val[0]) {
      addColumn(*newTable, val);
    }
  } while (val[0]);

  if (!json.structure) {
    json.structure = newTable;
  } else {
    Table *curTable = json.structure;
    while (curTable->next) {
      curTable = curTable->next;
    }
    curTable->next = newTable;
  }
}

void addColumn(Table &table, const string &column) {
  Column *newColumn = new Column(column);

  if (!table.columns) {
    table.columns = newColumn;
  } else {
    Column *curColumn = table.columns;
    while (curColumn->next) {
      curColumn = curColumn->next;
    }
    curColumn->next = newColumn;
  }
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
