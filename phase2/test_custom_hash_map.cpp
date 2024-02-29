#include "custom_hash_map.cpp"

int main() {
    CustomHashMap<std::string, int> hashMap;
    
    hashMap.insert("one", 1);
    hashMap.insert("two", 2);
    hashMap.insert("three", 3);
    
    std::cout << "Size: " << hashMap.size << std::endl;  // Output the size, which should be 3
    
    std::cout << "Value for key 'two': " << hashMap["two"] << std::endl;  // Output the value for the key 'two'
    
    std::cout << "Value for key 'four': " << hashMap["four"] << std::endl;  // Output the value for the key 'four' (default-initialized to 0)
    
    std::cout << hashMap.contains("four") << std::endl;
    return 0;
}
