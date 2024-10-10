#include "../include/file_manager.h"
#include <filesystem>
#include <fstream>

void createDirectories(const Json &json) {
  string name = json.name;
  string path = name;
  Table *CurrTable = json.structure;

  filesystem::create_directory(path);

  while (CurrTable != nullptr) {
    path = name + '/' + CurrTable->name;
    filesystem::create_directory(path);

    CurrTable = CurrTable->next;
  }
}

void createFiles(const Json &json) {
  createDirectories(json);

  string name = "1.csv";
  string path;
  Table *CurrTable = json.structure;

  while (CurrTable != nullptr) {
    path = json.name + '/' + CurrTable->name + '/';

    ofstream file(path + name);

    if (file.is_open()) {
      file << saveData(*CurrTable);
      file.close();
    } else {
      cerr << "Failed" << endl;
    }

    ofstream PkFile(path + CurrTable->name + "_pk_sequence");

    if (PkFile.is_open()) {
      PkFile << 1;
      PkFile.close();
    } else {
      cerr << "Failed";
    }

    ofstream LockFile(path + CurrTable->name + "_lock");

    if (LockFile.is_open()) {
      LockFile << 0;
      LockFile.close();
    } else {
      cerr << "Failed";
    }

    CurrTable = CurrTable->next;
  }
}

string saveData(const Table &table) {
  string str = "";
  Column *CurrColumn = table.columns;

  while (CurrColumn->next != nullptr) {
    str += CurrColumn->column + ',';
    CurrColumn = CurrColumn->next;
  }

  str += CurrColumn->column + '\n';

  return str;
}
