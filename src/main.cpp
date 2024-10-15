#include "../include/command_handler.h"
#include "../include/file_manager.h"
#include "../include/json_parser.h"

int main() {
  Json json = JsonParser();

  createFiles(json);

  string line;
  /*while (line != "EXIT") {*/
  getline(cin, line);
  menu(json, line);
  /*}*/

  return 0;
}
