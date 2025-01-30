#ifndef FILE_MANAGER_H // FILE_MANAGER_H
#define FILE_MANAGER_H

#include "hash_table.h"

void addLine(HT &table, Array &curTable);
bool isTableFree(const std::string &schemaName, const std::string &tableName);
void lockTable(const std::string &schemaName, const std::string &tableName);
void unlockTable(const std::string &schemaName, const std::string &tableName);
void createFiles(HT &table, Array &names);

#endif // FILE_MANAGER_H
