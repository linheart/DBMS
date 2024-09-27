#include "../include/file_manager.h"

void CreateDirectories(const JsonValue &json) {
  string name = json.name;
  string path = name;
  TableValue *CurrTable = json.structure;

  filesystem::create_directory(path);

  while (CurrTable != nullptr) {
    path = name + '/' + CurrTable->name;
    filesystem::create_directory(path);

    CurrTable = CurrTable->next;
  }
}

void CreateFiles(const JsonValue &json) {
  CreateDirectories(json);

  string name = "1.csv";
  string path;
  TableValue *CurrTable = json.structure;

  while (CurrTable != nullptr) {
    path = json.name + '/' + CurrTable->name + '/' + name;

    ofstream file(path);

    if (file.is_open()) {
      file << SaveData(*CurrTable);
    } else {
      cerr << "Failed" << endl;
    }

    CurrTable = CurrTable->next;
  }
}

string SaveData(const TableValue &table) {
  string str = "";
  ColumnValue *CurrColumn = table.columns;

  while (CurrColumn->next != nullptr) {
    str += CurrColumn->column + ',';

    CurrColumn = CurrColumn->next;
  }

  str += CurrColumn->column + '\n';

  return str;
}
