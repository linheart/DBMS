#ifndef FILE_MANAGER_H // FILE_MANAGER_H
#define FILE_MANAGER_H

#include "utility.h"

void mkDir(const string &path);
void mkFile(const string &path, const string &data);
void createDirectories(const Json &json);
void createFiles(const Json &json);
string saveData(const Table &table);
void addLine(const Json &json, const Table &table);
void delLines(const Json &json);

#endif // FILE_MANAGER_H
