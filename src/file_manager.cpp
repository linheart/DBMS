#include "../include/file_manager.h"

#include <cassert>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <string>

using namespace std;

bool isTableFree(const string &schemaName, const string &tableName) {
  string path = schemaName + '/' + tableName + '/' + tableName + "_lock";
  ifstream file(path);
  string lock;
  file >> lock;
  file.close();

  return lock == "0";
}

void lockTable(const string &schemaName, const string &tableName) {
  string path = schemaName + '/' + tableName + '/' + tableName + "_lock";
  ofstream file(path);
  file << "1";
  file.close();
}

void unlockTable(const string &schemaName, const string &tableName) {
  string path = schemaName + '/' + tableName + '/' + tableName + "_lock";
  ofstream file(path);
  file << "0";
  file.close();
}

string saveData(Array &table) {
  string str;
  size_t size = table.size();

  for (size_t i = 0; i < size - 1; i++) {
    str += table[i] + ',';
  }
  str += table[size - 1] + '\n';

  return str;
}

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

void createFiles(HT &table, Array &names) {
  string name = "1.csv";
  string path;

  string scheme = table.name;
  mkDir(scheme);

  size_t size = names.size();
  for (size_t i = 0; i < size; i++) {
    path = scheme + '/' + names[i] + '/';
    mkDir(path);

    mkFile(path + "1.csv", names[i] + "_pk," + saveData(table[names[i]]));
    mkFile(path + names[i] + "_pk_sequence", "0\n");
    mkFile(path + names[i] + "_lock", "0\n");
  }
}
