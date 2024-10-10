#include "../include/file_manager.h"
#include "../include/json_parser.h"

int main() {
  Json json = JsonParser();

  createFiles(json);

  return 0;
}
