#include "stock_entries.cpp"
#include "iostream"

int main() {
  StockEntries AAPL;

  stock_entry entry1 {
      "aaa", // broker_name
      100,   // price
      10,    // quantity;
      0,     // time_of_entry;
      -1     // life_remaining;
  };

  stock_entry entry2 {
      "baa", // broker_name
      100,   // price
      10,    // quantity;
      0,     // time_of_entry;
      -1     // life_remaining;
  };

  stock_entry entry3 {
      "caa", // broker_name
      100,   // price
      10,    // quantity;
      0,     // time_of_entry;
      -1     // life_remaining;
  };

  AAPL.insert(entry1, 1);
  AAPL.insert(entry2, 1);
  AAPL.insert(entry3, 1);

  AAPL.remove(AAPL.root);

  Node* curr_node = AAPL.root;
  while(curr_node) {
    cout << curr_node->data.time_of_entry << " " << curr_node->data.broker_name << " " << "$" << curr_node->data.price << " #" << curr_node->data.quantity << " " << curr_node->data.life_remaining << " --- ";
    curr_node = curr_node->next_node;
  }
  cout << endl;
}