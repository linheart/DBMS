#ifndef FILE_MANAGER_H // FILE_MANAGER_H
#define FILE_MANAGER_H

#include "utility.h"

using namespace std;

void CreateDirectories(const JsonValue &json);
void CreateFiles(const JsonValue &json);
string SaveData(const TableValue &table);

#endif // FILE_MANAGER_H
