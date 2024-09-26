#include "../include/json_parser.h"
/*#include <iostream>*/

using namespace std;

int main() {
  JsonValue json = JsonParse();

  PrintJsonValue(json);
  FreeJsonValue(json);

  return 0;
}
