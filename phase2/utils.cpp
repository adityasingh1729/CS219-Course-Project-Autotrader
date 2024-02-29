#include "vector"
#include "string"
#include "custom_hash_map.cpp"
#include "stock_entries.cpp"
using namespace std;

typedef CustomHashMap<string, StockEntries> buffer_dict;

// works perfectly
vector<string> split_sentence(string str, char delem) {
    vector<string> split_result;

    string word = "";
    for (auto x : str)  {
        if (x == delem) {
            split_result.push_back(word);
            word = "";
        }
        else {
            word = word + x;
        }
    }
    // word.pop_back();
    split_result.push_back(word);

    return split_result;
}

int min(int a, int b) {
  if(a > b) {
    return b;
  } else {
    return a;
  }
}


void print_exchanges(vector<successful_exchange>& exchanges) {
  for(auto& exchange : exchanges) {
    cout << exchange.buyer_name << " purchased " << 
    exchange.num_shares << " shares of " << exchange.stock_name << " from " << 
    exchange.seller_name << " for $" << exchange.price_per_share << "/share\n";
  }
  cout << endl;
}

// EOD calculations
void eod(vector<successful_exchange>& exchanges) {
  int total_sum = 0;
  int num_trades = 0;
  int num_shares = 0;

  for(auto& exchange : exchanges) {
    total_sum += exchange.num_shares * exchange.price_per_share;
    num_shares += exchange.num_shares;
    num_trades++;
  }

  cout << "---End of Day---\n";
  cout << "Total Amount of Money Transferred: " << total_sum << endl;
  cout << "Number of Completed Trades: " << num_trades << endl;
  cout << "Number of Shares Traded: " << num_shares << endl;
}