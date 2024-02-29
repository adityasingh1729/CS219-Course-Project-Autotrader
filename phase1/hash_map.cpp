#include "vector"
#include "iostream"

template <typename K, typename V>
struct KeyValuePair {
    K first; // key
    V second; // value
};

template <typename K, typename V>
class str_map {
public:
    size_t hashFunction(const K &first) {
        // Simple hash function for strings
        size_t hash = 0;

        for (char c : first) {
            hash = hash * 31 + c;
        }

        return hash % table.size();
    }

    std::vector<std::vector<KeyValuePair<K, V>>> table;
    std::vector<K> insertionOrder;
    // size_t size;
    size_t num_elems;

    str_map(size_t size = 50) : table(size), num_elems(0) {}

    void insert(const K &first, const V &second) {
        size_t index = hashFunction(first);
        for (auto &pair : table[index]) {
            if (pair.first == first) {
                pair.second = second;
                return;
            }
        }
        table[index].push_back({first, second});
        insertionOrder.push_back(first);
        num_elems++;
    }

    V &operator[](const K &first) {
        size_t index = hashFunction(first);
        for (auto &pair : table[index]) {
            if (pair.first == first) {
                return pair.second;
            }
        }
        table[index].push_back({first, V()});
        insertionOrder.push_back(first);
        num_elems++;
        return table[index].back().second;
    }

    V &at(const K &first) {
        size_t index = hashFunction(first);
        for (auto &pair : table[index]) {
            if (pair.first == first) {
                return pair.second;
            }
        }
        throw std::out_of_range("first not found");
    }

    bool contains(const K &first) {
        size_t index = hashFunction(first);
        for (const auto &pair : table[index]) {
            if (pair.first == first) {
                return true;
            }
        }
        return false;
    }

    void printInsertionOrder() {
        for (const auto &first : insertionOrder) {
            std::cout << first << " ";
        }
        std::cout << std::endl;
    }

    int size() {
      return num_elems;
    }

    void clear() {
      table.clear();
      num_elems = 0;
    }

    // Find method to check if a key exists in the map
    bool find(const K &first) {
        return contains(first);
    }

    // // End method to get an iterator pointing to the end of the map
    // typename std::vector<K>::iterator end() {
    //     return insertionOrder.end();
    // }
};


template <typename K, typename V>
class int_map {
public:
    size_t hashFunction(const K &first) {
        // Simple hash function for strings
        size_t hash = static_cast<size_t>(first);

        return hash % table.size();
    }

    std::vector<std::vector<KeyValuePair<K, V>>> table;
    std::vector<K> insertionOrder;
    // size_t size;
    size_t num_elems;

    int_map(size_t size = 50) : table(size), num_elems(0) {}

    void insert(const K &first, const V &second) {
        size_t index = hashFunction(first);
        for (auto &pair : table[index]) {
            if (pair.first == first) {
                pair.second = second;
                return;
            }
        }
        table[index].push_back({first, second});
        insertionOrder.push_back(first);
        num_elems++;
    }

    V &operator[](const K &first) {
        size_t index = hashFunction(first);
        for (auto &pair : table[index]) {
            if (pair.first == first) {
                return pair.second;
            }
        }
        table[index].push_back({first, V()});
        insertionOrder.push_back(first);
        num_elems++;
        return table[index].back().second;
    }

    V &at(const K &first) {
        size_t index = hashFunction(first);
        for (auto &pair : table[index]) {
            if (pair.first == first) {
                return pair.second;
            }
        }
        throw std::out_of_range("first not found");
    }

    bool contains(const K &first) {
        size_t index = hashFunction(first);
        for (const auto &pair : table[index]) {
            if (pair.first == first) {
                return true;
            }
        }
        return false;
    }

    void printInsertionOrder() {
        for (const auto &first : insertionOrder) {
            std::cout << first << " ";
        }
        std::cout << std::endl;
    }

    int size() {
      return num_elems;
    }

    void clear() {
      table.clear();
      num_elems = 0;
    }

    // Find method to check if a key exists in the map
    bool find(const K &first) {
        return contains(first);
    }

    // // End method to get an iterator pointing to the end of the map
    // typename std::vector<K>::iterator end() {
    //     return insertionOrder.end();
    // }
};


