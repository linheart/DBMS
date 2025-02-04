#include "../include/command_handler.h"
#include "../include/file_manager.h"
#include "../include/json_parser.h"

using namespace std;

int main() {
  HT table;
  Array names = JsonParser(table);

  createFiles(table, names);

  string line;
  while (line != "EXIT") {
    getline(cin, line);
    menu(table, line);
  }

  size_t size = names.size();
  for (size_t i = 0; i < size; i++) {
    lockTable(table.name, names[i]);
  }
  return 0;
}
