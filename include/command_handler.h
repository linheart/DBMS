#ifndef COMMAND_HANDLER_H // COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "utility.h"
#include <sstream>

void split(string &str, string &left, string &right);
void printColumnNames(Json *json);
void printTables(Table *table, const string &name, const string &curLine);
void check(const string &tName, const string &cName, Json &json);
void fillingTable(Table &table, const string &name);
void mergeTable(Json &first, Json &second);
void filter(Json &json, Json &finalConfig, istringstream &stream);
void menu(Json &json, const string &str);
void printT(Table *table);

#endif // COMMAND_HANDLER_H
