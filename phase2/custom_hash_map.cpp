#include "vector"
#include "iostream"

template <typename K, typename V>
struct KeyValuePair {
    K key;
    V value;
};

template <typename K, typename V>
class CustomHashMap {
private:
    size_t hashFunction(const K& key) {
        // Simple hash function for strings
        size_t hash = 0;
        for (char c : key) {
            hash = hash * 31 + c;
        }
        return hash % table.size();
    }
public:
    std::vector<std::vector<KeyValuePair<K, V>>> table;
    size_t size;
    CustomHashMap(size_t size = 50) : table(size), size(0) {}

    void insert(const K& key, const V& value) {
        size_t index = hashFunction(key);
        for (auto& pair : table[index]) {
            if (pair.key == key) {
                pair.value = value;
                return;
            }
        }
        table[index].push_back({key, value});
        size++;
    }

    V& operator[](const K& key) {
        size_t index = hashFunction(key);
        for (auto& pair : table[index]) {
            if (pair.key == key) {
                return pair.value;
            }
        }
        table[index].push_back({key, V()});
        size++;
        return table[index].back().value;
    }

    V& at(const K& key) {
        size_t index = hashFunction(key);
        for (auto& pair : table[index]) {
            if (pair.key == key) {
                return pair.value;
            }
        }
        throw std::out_of_range("Key not found");
    }

    bool contains(const K& key) {
        size_t index = hashFunction(key);
        for (const auto& pair : table[index]) {
            if (pair.key == key) {
                return true;
            }
        }
        return false;
    }
};