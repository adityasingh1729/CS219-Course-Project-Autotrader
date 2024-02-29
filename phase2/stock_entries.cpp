#include "vector"
#include "string"
using namespace std;

struct stock_entry {
    string broker_name;
    int price;
    int quantity;
    int time_of_entry;
    int life_remaining;
};

struct successful_exchange {
  string stock_name;
  string seller_name;
  string buyer_name;
  int num_shares;
  int price_per_share;
};

class Node {
public:
    stock_entry data;
    Node* next_node = nullptr;

    Node();
    Node(stock_entry value) : data(value) {};
};

/* TODO:
- [x] inserting new entries
- [x] deleting entries 
- [ ] updating life_remaining

*/
class StockEntries {
public:
    Node* root = nullptr;
    int size = 0;

    void insert(stock_entry& value, bool smallest_first) {
        Node* new_entry = new Node(value);

        if (root == nullptr) {
            new_entry->next_node = root;
            root = new_entry;
            size++;
            return;
        }

        Node* curr_node = root;
        Node* prev_node = nullptr;

        while (curr_node != nullptr) {
            // Determine the condition based on smallest_first
            bool shouldInsert = false;
            if (smallest_first == 0) {
                shouldInsert = (value.price > curr_node->data.price) ||
                            (value.price == curr_node->data.price &&
                            value.time_of_entry < curr_node->data.time_of_entry) ||
                            (value.price == curr_node->data.price &&
                            value.time_of_entry == curr_node->data.time_of_entry &&
                            value.broker_name < curr_node->data.broker_name);
            } else {
                shouldInsert = (value.price < curr_node->data.price) ||
                            (value.price == curr_node->data.price &&
                            value.time_of_entry < curr_node->data.time_of_entry) ||
                            (value.price == curr_node->data.price &&
                            value.time_of_entry == curr_node->data.time_of_entry &&
                            value.broker_name < curr_node->data.broker_name);
            }

            if (shouldInsert) {
                new_entry->next_node = curr_node;
                if (prev_node) {
                    prev_node->next_node = new_entry;
                } else {
                    root = new_entry; // Update root if inserting at the beginning
                }
                size++;
                return;
            }
            prev_node = curr_node;
            curr_node = curr_node->next_node;
        }

        prev_node->next_node = new_entry; // Insert at the end
        size++;
    }

    void remove(Node* node_to_delete) {
        if (root == nullptr || node_to_delete == nullptr) {
            return;
        }

        if (root == node_to_delete) {
            root = root->next_node;
            delete node_to_delete;
            size--;
            return;
        }

        Node* curr_node = root;
        Node* prev_node = nullptr;

        while (curr_node != nullptr) {
            if (curr_node == node_to_delete) {
                prev_node->next_node = curr_node->next_node;
                delete curr_node;
                size--;
                return;
            }
            prev_node = curr_node;
            curr_node = curr_node->next_node;
        }
    }
};
