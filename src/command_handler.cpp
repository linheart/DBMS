#include "../include/command_handler.h"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <sstream>

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

void printColumnNames(Json *json) {
  Table *curTable = json->structure;
  while (curTable) {
    Column *curColumn = curTable->columns;
    while (!isdigit(curColumn->column[0])) {
      cout << curColumn->column << ' ';
      curColumn = curColumn->next;
    }
    curTable = curTable->next;
  }
  cout << endl;
}

void printTables(Table *table, const string &name, const string &curLine) {
  if (!table) {
    cout << curLine << endl;
    return;
  }

  string path = name + '/' + table->name + '/';
  int i = 1;

  while (filesystem::exists(path + char('0' + i) + ".csv")) {
    ifstream file(path + char('0' + i) + ".csv");
    string line;

    if (i++ == 1) {
      getline(file, line);
    }

    while (getline(file, line)) {
      string newLine = curLine;

      istringstream stream(line);
      string tmp;

      getline(stream, tmp, ',');

      if (table->columns->find(tmp)) {
        Column *curColumn = table->columns;
        unsigned j = 1;

        while (getline(stream, tmp, ',')) {
          if (j == curColumn->num) {
            newLine += tmp + " ";
            curColumn = curColumn->next;
          }
          j++;
        }
      }

      printTables(table->next, name, newLine);
    }
  }
}

void check(const string &tName, const string &cName, Json &json) {
  assert(tName[0] && cName[0]);

  Table *table = json.structure->find(tName);
  assert(table);

  Column *column = table->columns->find(cName);
  assert(column);
}

void fillingTable(Table &table, const string &name) {
  string path = name + '/' + table.name + '/';
  int i = 1;
  while (filesystem::exists(path + char('0' + i) + ".csv")) {
    ifstream file(path + char('0' + i++) + ".csv");

    string line;
    getline(file, line);

    while (getline(file, line)) {
      if (line[0]) {
        istringstream stream(line);
        string tmp;
        getline(stream, tmp, ',');
        table.addColumn(tmp);
      }
    }
    file.close();
  }
}

unsigned getColumnNum(Table *table, const string &cName) {
  unsigned i = 1;
  Column *curColumn = table->columns;

  while (curColumn && curColumn->column != cName) {
    i++;
    curColumn = curColumn->next;
  }

  return i;
}

void menu(Json &json, const string &str) {
  istringstream stream(str);
  string token;

  stream >> token;

  if (token == "SELECT") {
    Json *finalConfig = new Json;
    finalConfig->name = json.name;
    while (stream >> token && token != "FROM") {
      string tName;
      string cName;
      split(token, tName, cName);

      check(tName, cName, json);

      Table *table = finalConfig->structure->find(tName);

      if (!table) {
        table = new Table;
        table->name = tName;
        finalConfig->addTable(table);
      }

      table->addColumn(cName);
      table->columns->find(cName)->num =
          getColumnNum(json.structure->find(tName), cName);
    }

    assert(token == "FROM");

    while (stream >> token && token != "WHERE") {
      if (token[token.size() - 1] == ',') {
        token.pop_back();
      }
      assert(json.structure->find(token));
      fillingTable(*finalConfig->structure->find(token), json.name);
    }

    printColumnNames(finalConfig);
    printTables(finalConfig->structure, finalConfig->name, "");

    while (stream >> token) {
      if (token == "AND") {
      } else if (token == "OR") {

      } else {
      }
    }

    delete finalConfig;

  } else if (token == "INSERT") {

  } else if (token == "DELETE") {
  } else {
    cout << "Wrong query" << endl;
  }
}
