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

  /*Table *curTable = json.structure;*/
  /*while (curTable) {*/
  /*  lockTable(json.name, curTable->name);*/
  /*  curTable = curTable->next;*/
  /*}*/

  return 0;
}
