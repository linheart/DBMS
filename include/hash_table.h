#ifndef HASH_TABLE_H // HASH_TABLE_H
#define HASH_TABLE_H

#include <functional>
#include <iostream>

class Array {
public:
  Array(std::size_t initialCapacity = 10)
      : data(new std::string[initialCapacity]), capacity(initialCapacity),
        length(0) {}

  ~Array() { delete[] data; }

  std::string &operator[](std::size_t index) { return data[index]; }

  void append(const std::string &value) {
    if (length >= capacity) {
      resize(capacity * 2);
    }
    data[length++] = value;
  }

  std::size_t size() { return length; }

private:
  std::string *data;
  std::size_t capacity;
  std::size_t length;

  void resize(std::size_t newCapacity) {
    std::string *newData = new std::string[newCapacity];
    for (std::size_t i = 0; i < length; i++) {
      newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
  }
};

class HT {
public:
  std::string name;
  int tuples_limit;

  HT() : size(1000) { nodes = new Node *[size](); }

  ~HT() {
    for (size_t i = 0; i < size; ++i) {
      Node *current = nodes[i];
      while (current) {
        Node *next = current->next;
        delete current;
        current = next;
      }
    }
    delete[] nodes;
  }

  Array &operator[](const std::string &key) {
    size_t index = hashFunction(key);
    Node *current = nodes[index];

    while (current) {
      if (current->key == key) {
        return current->values;
      }
      current = current->next;
    }

    Node *newNode = new Node(key);
    newNode->next = nodes[index];
    nodes[index] = newNode;
    return newNode->values;
  }

private:
  struct Node {
    std::string key;
    Array values;
    Node *next;

    Node(const std::string &k) : key(k), next(nullptr) {}
  };

  Node **nodes;
  const size_t size;

  size_t hashFunction(const std::string &key) const {
    std::hash<std::string> hasher;
    return hasher(key) % size;
  }
};

#endif // HASH_TABLE_H
