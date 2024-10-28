#include "../include/file_manager.h"

#include <cassert>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

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

void delLines(const Json &json) {
  string path = json.name + '/' + json.structure->name + '/';
  int i = 1;
  Column *curColumn = json.structure->columns;

  while (filesystem::exists(path + char('0' + i) + ".csv")) {
    ifstream iFile(path + char('0' + i) + ".csv");
    ofstream tFile(path + "tmp.csv");

    string line;

    if (i == 1) {
      getline(iFile, line);
      tFile << line << endl;
    }

    while (getline(iFile, line)) {
      string pk;
      istringstream stream(line);
      getline(stream, pk, ',');

      if (curColumn->column == pk) {
        curColumn = curColumn->next;
      } else {
        tFile << line << endl;
      }
    }

    filesystem::remove(path + char('0' + i) + ".csv");
    if (!filesystem::is_empty(path + "tmp.csv")) {
      filesystem::rename(path + "tmp.csv", path + char('0' + i) + ".csv");
    } else {
      filesystem::remove(path + "tmp.csv");
    }
    i++;
  }
}
