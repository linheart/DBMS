#ifndef UTILITY_H // UTILITY_H
#define UTILITY_H

#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std;

struct ColumnValue {
  string column;
  ColumnValue *next;
};

struct TableValue {
  string name;
  ColumnValue *columns;
  TableValue *next;
};

struct JsonValue {
  string name;
  int tuples_limit;
  TableValue *structure;
};

#endif // UTILITY_H
