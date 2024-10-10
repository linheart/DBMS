#ifndef UTILITY_H // UTILITY_H
#define UTILITY_H

#include <iostream>

using namespace std;

struct Column {
  string column;
  Column *next;

  Column(const string &str) : column(str), next(nullptr) {}
  ~Column() { delete next; }
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
};

struct Json {
  string name;
  int tuples_limit;
  Table *structure;

  Json() : structure(nullptr) {}
  ~Json() { delete structure; }
};

#endif // UTILITY_H
