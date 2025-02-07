#include "../include/file_manager.h"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <string>

using namespace std;

bool isTableFree(const string &schemaName, const string &tableName) {
  string path = schemaName + '/' + tableName + '/' + tableName + "_lock";
  ifstream file(path);
  string lock;
  if (file.is_open()) {
    file >> lock;
    file.close();
  }
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
    if (file.is_open()) {
      file << data;
      file.close();
    }
  }
}

void createFiles(HT &table, Array &names) {
  string path;

  string scheme = table.name;
  mkDir(scheme);

  size_t size = names.size();
  for (size_t i = 0; i < size; i++) {
    path = scheme + '/' + names[i] + '/';
    mkDir(path);

    mkFile(path + "1.csv", saveData(table[names[i]]));
    mkFile(path + names[i] + "_pk_sequence", "0\n");
    mkFile(path + names[i] + "_lock", "0\n");
  }
}

void addLine(HT &table, const string &tName, const string &str) {
  string path = table.name + '/' + tName + '/';
  ifstream pkFile(path + tName + "_pk_sequence");

  if (pkFile.is_open()) {
    string pk;
    pkFile >> pk;

    pk = to_string(stoi(pk) + 1);
    ofstream file(path + tName + "_pk_sequence");
    file << pk;

    string line = pk + ',' + str;

    int i = ceil((double)(stoi(pk)) / table.tuples_limit);
    path += to_string(i) + ".csv";
    if (filesystem::exists(path)) {
      ofstream file(path, ios::app);
      file << line;
      file.close();
    } else {
      mkFile(path, line);
    }
  }
}

void remLines(HT &table, const string &tName) {
  string path = table.name + '/' + tName + '/';
  string key = tName + table[tName][0];
  for (int i = 1; filesystem::exists(path + char('0' + i) + ".csv"); i++) {
    ifstream file(path + char('0' + i) + ".csv");
    ofstream iFile(path + char('0' + i) + ".csv.tmp");
    string line;
    while (getline(file, line)) {
      if (line[0]) {
        istringstream stream(line);
        string tmp;
        getline(stream, tmp, ',');

        if (table[key].find(tmp) == table[key].size()) {
          iFile << line << endl;
        } else {
          iFile << endl;
        }
      }
    }
    filesystem::remove(path + char('0' + i) + ".csv");
    filesystem::rename(path + char('0' + i) + ".csv.tmp",
                       path + char('0' + i) + ".csv");
    file.close();
    iFile.close();
  }
}
