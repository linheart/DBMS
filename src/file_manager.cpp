#include "../include/file_manager.h"

#include <cassert>
#include <filesystem>
#include <fstream>

void mkDir(const string &path) {
  if (!filesystem::exists(path)) {
    filesystem::create_directory(path);
  }
}

void mkFile(const string &path, const string &data) {
  if (!filesystem::exists(path)) {
    ofstream file(path);
    assert(file.is_open());
    file << data;
    file.close();
  }
}

void createDirectories(const Json &json) {
  string name = json.name;
  string path = name;
  Table *CurrTable = json.structure;

  mkDir(path);

  while (CurrTable != nullptr) {
    path = name + '/' + CurrTable->name;

    mkDir(path);

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

    mkFile(path + name, saveData(*CurrTable));
    mkFile(path + CurrTable->name + "_pk_sequence", "1\n");
    mkFile(path + CurrTable->name + "_lock", "0\n");

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
