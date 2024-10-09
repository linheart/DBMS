#ifndef UTILITY_H // UTILITY_H
#define UTILITY_H

#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std;

constexpr uint32_t FNV_Prime = 0x01000193;
constexpr uint32_t Seed = 0x811C9DC5;

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
