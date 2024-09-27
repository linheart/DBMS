#include "../include/file_manager.h"
#include "../include/json_parser.h"
#include "../include/utility.h"

int main() {
  JsonValue json = JsonParse();

  CreateFiles(json);

  FreeJsonValue(json);

  return 0;
}
