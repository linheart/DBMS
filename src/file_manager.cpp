#include "../include/file_manager.h"

#include <cassert>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <string>

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

    mkFile(path + name, curTable->name + "_pk" + ',' + saveData(*curTable));
    mkFile(path + curTable->name + "_pk_sequence", "0\n");
    mkFile(path + curTable->name + "_lock", "0\n");

    curTable = curTable->next;
  }
}

string saveData(const Table &table) {
  string str;
  Column *curColumn = table.columns;

  while (curColumn->next != nullptr) {
    str += curColumn->column + ',';
    curColumn = curColumn->next;
  }

  str += curColumn->column + '\n';

  return str;
}

void addLine(const Json &json, const Table &table) {
  string path = json.name + '/' + table.name + '/';
  ifstream pkFile(path + table.name + "_pk_sequence");
  assert(pkFile.is_open());
  string a;
  pkFile >> a;
  int pk = stoi(a) + 1;

  int i = ceil((double)pk / json.tuples_limit);

  ofstream file(path + table.name + "_pk_sequence");
  file << to_string(pk);

  string s = to_string(pk) + ',' + saveData(table);

  path += to_string(i) + ".csv";

  if (filesystem::exists(path)) {
    ofstream file(path, ios::app);
    file << s;
    file.close();
  } else {
    mkFile(path, s);
  }
}
