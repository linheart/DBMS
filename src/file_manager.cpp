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
  Table *curTable = json.structure;

  mkDir(path);

  while (curTable != nullptr) {
    path = name + '/' + curTable->name;

    mkDir(path);

    curTable = curTable->next;
  }
}

void createFiles(const Json &json) {
  createDirectories(json);

  string name = "1.csv";
  string path;
  Table *curTable = json.structure;

  while (curTable != nullptr) {
    path = json.name + '/' + curTable->name + '/';

    mkFile(path + name, saveData(*curTable));
    mkFile(path + curTable->name + "_pk_sequence", "1\n");
    mkFile(path + curTable->name + "_lock", "0\n");

    curTable = curTable->next;
  }
}

string saveData(const Table &table) {
  string str = "";
  Column *curColumn = table.columns;

  while (curColumn->next != nullptr) {
    str += curColumn->column + ',';
    curColumn = curColumn->next;
  }

  str += curColumn->column + '\n';

  return str;
}
