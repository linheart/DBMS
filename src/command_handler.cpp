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
    while (curColumn && !isdigit(curColumn->column[0])) {
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

        while (curColumn && !isdigit(curColumn->column[0])) {
          unsigned j = 0;
          istringstream stream(line);
          while (getline(stream, tmp, ',') && j++ != curColumn->num)
            ;
          newLine += tmp + " ";
          curColumn = curColumn->next;
        }
        printTables(table->next, name, newLine);
      }
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
    ifstream file(path + char('0' + i) + ".csv");

    string line;
    if (i == 1) {
      getline(file, line);
    }
    i++;

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

void editTable(Json &json, Table &table, const string &cName,
               const string &target) {
  Table *curTable = json.structure->find(table.name);
  unsigned num = curTable->columns->find(cName)->num;
  string path = json.name + '/' + table.name + '/';
  int i = 1;
  while (filesystem::exists(path + char('0' + i) + ".csv")) {
    ifstream file(path + char('0' + i) + ".csv");

    string line;
    if (i == 1) {
      getline(file, line);
    }

    i++;

    while (getline(file, line)) {
      if (line[0]) {
        istringstream stream(line);
        string tmp;

        unsigned j = 1;
        getline(stream, tmp, ',');
        if (table.columns->find(tmp)) {
          string pk = tmp;

          while (getline(stream, tmp, ',') && j++ != num)
            ;

          if (tmp != target) {
            table.columns->remove(table.columns, pk);
          }
        }
      }
    }
    file.close();
  }
}

void copyTables(Json &src, Json &target) {
  Table *curTable = src.structure;
  while (curTable) {
    Table *newTable = new Table;
    newTable->name = curTable->name;
    Column *curColumn = curTable->columns;
    while (curColumn) {
      newTable->addColumn(curColumn->column);
      newTable->columns->find(curColumn->column)->num = curColumn->num;
      curColumn = curColumn->next;
    }
    target.addTable(newTable);
    curTable = curTable->next;
  }
}

void mergeTable(Json &first, Json &second, const string &tName) {
  Table *firstTable = first.structure->find(tName);
  Table *secondTable = second.structure->find(tName);

  Column *firstColumn = firstTable->columns;
  Column *secondColumn = secondTable->columns;
  while (secondColumn) {
    if (!firstColumn->find(secondColumn->column)) {
      firstTable->addColumn(secondColumn->column);
    }
    secondColumn = secondColumn->next;
  }
}

void filter(Json &json, Json &finalConfig, istringstream &stream) {
  string token;

  if (!(stream >> token)) {
    return;
  }

  string tName;
  string cName;
  split(token, tName, cName);

  check(tName, cName, json);

  assert(stream >> token && token == "=");
  assert(stream >> token);

  string val = token.substr(1, token.size() - 2);
  Table *table = finalConfig.structure->find(tName);

  if (stream >> token && token == "AND" || token != "OR") {
    editTable(json, *table, cName, val);
    filter(json, finalConfig, stream);
  } else if (token == "OR") {
    Json *tmp = new Json;
    copyTables(finalConfig, *tmp);
    editTable(json, *tmp->structure->find(tName), cName, val);
    filter(json, finalConfig, stream);
    mergeTable(finalConfig, *tmp, tName);

    delete tmp;
  }
}

string getVal(const string &token) {
  istringstream stream(token);
  string val;
  char ch;

  while (stream >> ch && ch != '\'')
    ;

  while (stream >> ch && ch != '\'') {
    val += ch;
  }
  return val;
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
      Table *tmp = json.structure->find(tName);
      table->columns->find(cName)->num = tmp->columns->find(cName)->num;
    }

    assert(finalConfig->structure && token == "FROM");

    while (stream >> token && token != "WHERE") {
      if (token[token.size() - 1] == ',') {
        token.pop_back();
      }

      assert(json.structure->find(token));
      assert(isTableFree(json.name, token));
      if (!finalConfig->structure->find(token)) {
        Table *newTable = new Table;
        newTable->name = token;
        finalConfig->addTable(newTable);
      }
      fillingTable(*finalConfig->structure->find(token), json.name);
    }

    if (token == "WHERE") {
      filter(json, *finalConfig, stream);
    }

    printColumnNames(finalConfig);
    printTables(finalConfig->structure, finalConfig->name, "");

    delete finalConfig;

  } else if (token == "INSERT") {
    assert(stream >> token && token == "INTO");
    assert(stream >> token);

    assert(isTableFree(json.name, token));
    Table *table = json.structure->find(token);
    assert(table);

    Table finalTable;
    finalTable.name = token;

    assert(stream >> token && token == "VALUES");

    Column *curColumn = table->columns;

    do {
      assert(stream >> token);
      finalTable.addColumn(getVal(token));
      curColumn = curColumn->next;
    } while (token.back() != ')' && curColumn);

    assert(!(stream >> token) && !curColumn);

    addLine(json, finalTable);

  } else if (token == "DELETE") {
    assert(stream >> token && token == "FROM");
    assert(stream >> token);
    assert(isTableFree(json.name, token));
    assert(json.structure->find(token));

    Json *finalConfig = new Json;
    finalConfig->name = json.name;

    Table *table = new Table;
    table->name = token;
    finalConfig->addTable(table);

    assert(stream >> token && token == "WHERE");

    fillingTable(*table, json.name);
    filter(json, *finalConfig, stream);

    delLines(*finalConfig);

    delete finalConfig;
  } else if (token == "FREE") {
    Table *curTable = json.structure;
    while (curTable) {
      unlockTable(json.name, curTable->name);
      curTable = curTable->next;
    }

  } else if (token != "EXIT") {
    cout << "Wrong query" << endl;
  }
}
