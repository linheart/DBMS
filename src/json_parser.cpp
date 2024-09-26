#include "../include/json_parser.h"

JsonValue JsonParse() {
  string text;
  ReadFile(text);
  text_it it = text.begin();

  return ParseJsonHelper(it);
}

void ReadFile(string &output) {
  ifstream file("schema.json");
  string line;

  while (getline(file, line)) {
    output.append(line);
  }

  file.close();
}

JsonValue ParseJsonHelper(text_it &it) {
  string key;
  string value;

  JsonValue json_value;
  json_value.structure = nullptr;

  it++;

  do {
    SkipSymbols(it);
    if (*it == '\"') {
      key = RertrieveValue(it);

      SkipSymbols(it);

      if (key == "structure") {
        RetrieveTables(it, json_value);

      } else {
        value = RertrieveValue(it);

        if (key == "name") {
          json_value.name = value;
        } else if (key == "tuples_limit") {
          json_value.tuples_limit = StringToInt(value);
        }
      }
    }
  } while (*it != '}');

  return json_value;
}

string RertrieveValue(text_it &it) {
  string val;

  if (*it == '\"') {
    it++;
    while (*it != '\"') {
      val += *it;
      it++;
    }
    it++;
  } else {
    while (isdigit(*it)) {
      val += *it;
      it++;
    }
  }

  it++;
  return val;
}

void RetrieveTables(text_it &it, JsonValue &json_value) {
  SkipSymbols(++it);
  string name;
  string column;

  while (*it != '}') {
    name = RertrieveValue(it);
    AddTableValue(json_value, name);

    TableValue *CurTable = json_value.structure;

    while (CurTable->next != nullptr) {
      CurTable = CurTable->next;
    }

    it += 2;
    SkipSymbols(it);

    while (*it != ']') {
      column = RertrieveValue(it);
      AddColumnValue(*CurTable, column);
      SkipSymbols(it);
    };

    it += 2;
    SkipSymbols(it);
  }
}

void AddTableValue(JsonValue &json_value, const string &name) {
  TableValue *NewTable = new TableValue{name, nullptr, nullptr};

  if (json_value.structure == nullptr) {
    json_value.structure = NewTable;
  } else {
    TableValue *CurTable = json_value.structure;
    while (CurTable->next != nullptr) {
      CurTable = CurTable->next;
    }
    CurTable->next = NewTable;
  }
}

void AddColumnValue(TableValue &table, string column) {
  ColumnValue *NewColumn = new ColumnValue{column, nullptr};

  if (table.columns == nullptr) {
    table.columns = NewColumn;
  } else {
    ColumnValue *CurColumn = table.columns;
    while (CurColumn->next != nullptr) {
      CurColumn = CurColumn->next;
    }
    CurColumn->next = NewColumn;
  }
}

void SkipSymbols(text_it &it) {
  while (*it == ' ' || *it == '\n') {
    it++;
  }
}

int StringToInt(string &val) {
  int num = 0;
  for (auto ch : val) {
    num = num * 10 + ch - '0';
  }
  return num;
}

void FreeJsonValue(JsonValue &json_value) {
  TableValue *current_table = json_value.structure;
  while (current_table != nullptr) {
    ColumnValue *current_column = current_table->columns;
    while (current_column != nullptr) {
      ColumnValue *to_delete = current_column;
      current_column = current_column->next;
      delete to_delete;
    }

    TableValue *to_delete_table = current_table;
    current_table = current_table->next;
    delete to_delete_table;
  }
  json_value.structure = nullptr;
}

void PrintJsonValue(const JsonValue &json_value) {
  cout << "Name: " << json_value.name << endl;
  cout << "Tuples limit: " << json_value.tuples_limit << endl;

  TableValue *current_table = json_value.structure;
  while (current_table != nullptr) {
    cout << "Table: " << current_table->name << endl;

    ColumnValue *current_column = current_table->columns;
    while (current_column != nullptr) {
      cout << "  Column: " << current_column->column << endl;
      current_column = current_column->next;
    }
    current_table = current_table->next;
  }
}
