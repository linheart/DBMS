#ifndef FILE_MANAGER_H // FILE_MANAGER_H
#define FILE_MANAGER_H

#include "utility.h"

void createDirectories(const Json &json);
void createFiles(const Json &json);
string saveData(const Table &table);

#endif // FILE_MANAGER_H
