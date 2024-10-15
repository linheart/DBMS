#ifndef JSON_PARSER_H // JSON_PARSER_H
#define JSON_PARSER_H

#include "utility.h"

Json JsonParser();
void retrieveKeyValue(Json &json, const string &str, ifstream &file);
void addTable(Json &json, const string &str);
string retrieveValue(istringstream &stream);
int stringToInt(istringstream &stream);

#endif // JSON_PARSER_H
