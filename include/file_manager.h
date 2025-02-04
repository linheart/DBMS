#ifndef FILE_MANAGER_H // FILE_MANAGER_H
#define FILE_MANAGER_H

#include "hash_table.h"

bool isTableFree(const std::string &schemaName, const std::string &tableName);
void lockTable(const std::string &schemaName, const std::string &tableName);
void unlockTable(const std::string &schemaName, const std::string &tableName);
void createFiles(HT &table, Array &names);
void addLine(HT &table, const std::string &tName, const std::string &str);
void remLines(HT &table, const std::string &tName);

#endif // FILE_MANAGER_H
