#ifndef JSON_PARSER_H // JSON_PARSER_H
#define JSON_PARSER_H

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

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

Json JsonParser();
void retrieveKeyValue(Json &json, const string &str, ifstream &file);
void addTable(Json &json, const string &str);
void addColumn(Table &table, const string &column);
string retrieveValue(istringstream &stream);
int stringToInt(istringstream &stream);

#endif // JSON_PARSER_H
