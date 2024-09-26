#ifndef JSON_PARSER_H // JSON_PARSER_H
#define JSON_PARSER_H

#include <fstream>
#include <iostream>

using namespace std;

using text_it = string::iterator;

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

JsonValue JsonParse();
void ReadFile(string &output);
JsonValue ParseJsonHelper(text_it &it);
string RertrieveValue(text_it &it);
void RetrieveTables(text_it &it, JsonValue &json_value);
void AddTableValue(JsonValue &json_value, const string &name);
void AddColumnValue(TableValue &table, string column);
void SkipSymbols(text_it &it);
int StringToInt(string &val);
void FreeJsonValue(JsonValue &json_value);
void PrintJsonValue(const JsonValue &json_value);

#endif // JSON_PARSER_H
