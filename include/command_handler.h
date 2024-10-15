#ifndef COMMAND_HANDLER_H // COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "utility.h"

void printTables(Table *table, const string &name, const string &curLine);
void check(const string &tName, const string &cName, Json &json);
void fillingTable(Table &table, const string &name);
void menu(Json &json, const string &str);

#endif // COMMAND_HANDLER_H
