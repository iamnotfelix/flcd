#pragma once

#include <vector>
#include <list>
#include <cmath>
#include <string>
#include <utility>
#include <iostream>
#include <sstream>

using namespace std;

template <class T> 
class HashTable
{
private:
    int codeIndex;
    const int PRIME_CONST = 131;

    struct Node {
        T value;
        int code;
    };

    int hashFunction(string key) {
        return (key.length() * PRIME_CONST) % this->size;
       /* unsigned long hash = 0;
        for (int i = 0; i < key.length(); i++)
        {
            hash = (hash * PRIME_CONST) + key[i];
        }
        return hash % this->size;*/
    }

public:
    int size;
    vector<list<Node>> table;

    HashTable(int size);
    void insert(T value);
    pair<bool, int> search(T key);
    void displayHash();
};


template<class T>
HashTable<T>::HashTable(int size) {
    this->size = size;
    this->codeIndex = 0;

    this->table = vector<list<Node>>(size);
}

template<class T>
void HashTable<T>::insert(T value) {
    if (this->search(value).first) return;
 
    stringstream ss;
    ss << value;
    string key = ss.str();
    int hash = this->hashFunction(key);

    Node tmp;
    tmp.code = this->codeIndex++;
    tmp.value = value;

    this->table[hash].push_back(tmp);
}

template<class T>
pair<bool, int> HashTable<T>::search(T key) {
    stringstream ss;
    ss << key;
    string k = ss.str();
    int hash = this->hashFunction(k);

    if (this->table[hash].empty()) return { false, 0 };

    for (auto node : this->table[hash]) {
        if (node.value == key) return { true, node.code };
    }

    return { false, 0 };
}

template<class T>
void HashTable<T>::displayHash() {
    for (auto l : this->table) {
        for (auto node : l) {
            cout << node.value << " " << node.code << endl;
        }
    }
}
