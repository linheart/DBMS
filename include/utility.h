#ifndef UTILITY_H // UTILITY_H
#define UTILITY_H

#include <iostream>

using namespace std;

struct Column {
  string column;
  Column *next;
  unsigned num;

  Column(const string &str) : column(str), next(nullptr) {}
  ~Column() { delete next; }

  Column *find(const string &target) {
    Column *curColumn = this;
    while (curColumn && curColumn->column != target) {
      curColumn = curColumn->next;
    }

    return curColumn;
  }
};

struct Table {
  string name;
  Column *columns;
  Table *next;

  Table() : columns(nullptr), next(nullptr) {}
  ~Table() {
    delete columns;
    delete next;
  }

  void addColumn(const string &val) {
    Column *newColumn = new Column(val);

    if (!columns) {
      this->columns = newColumn;
    } else {
      Column *curColumn = columns;
      while (curColumn->next) {
        curColumn = curColumn->next;
      }
      curColumn->next = newColumn;
    }
  }

  Table *find(const string &target) {
    Table *curTable = this;
    while (curTable && curTable->name != target) {
      curTable = curTable->next;
    }

    return curTable;
  }
};

struct Json {
  string name;
  int tuples_limit;
  Table *structure;

  Json() : structure(nullptr) {}
  ~Json() { delete structure; }

  void addTable(Table *table) {
    if (!structure) {
      structure = table;
    } else {
      Table *curTable = structure;
      while (curTable->next) {
        curTable = curTable->next;
      }
      curTable->next = table;
    }
  }
};

#endif // UTILITY_H
