#include "market.h"
#include "market_manager.cpp"
#include "iostream"
#include "fstream"
 
market::market(int argc, char **argv) {}

void market::start()
{
  ifstream inputFile("output.txt");

  string line;
  int curr_timestamp = 0; 

  // each element is: (stock_name, broker, price, # stocks, life_remaining)
  buffer_dict sell_buffer;
  buffer_dict buy_buffer;
  vector<successful_exchange> exchanges;

  while (getline(inputFile, line)) {
    if(line == "TL") // start of the file
      continue;
    if(line == "!@") {// end of relevent data
      // update_buffers(sell_buffer, buy_buffer);
      market_manager(sell_buffer, buy_buffer, exchanges);
      break;
    }

    // breaks each line into individual words
    vector<string> words = split_sentence(line, ' ');

    int timestamp = stoi(words[0]);
    if(curr_timestamp != timestamp) {
      market_manager(sell_buffer, buy_buffer, exchanges);
      update_buffers(sell_buffer, buy_buffer);
      curr_timestamp = timestamp;
    }

    string stock_name = words[3];

    stock_entry value = {
      words[1],                 // broker_name
      stoi(words[4].substr(1)), // price
      stoi(words[5].substr(1)), // quantity
      timestamp,                // time_of_entry
      stoi(words[6])            // life_remaining
    };

    // inserting into buffers
    if(words[2] == "SELL") {
      sell_buffer[stock_name].insert(value, 1);
    }
    else if(words[2] == "BUY") {
      buy_buffer[stock_name].insert(value, 0); 
    }
  }

  print_exchanges(exchanges);
  eod(exchanges);
  
  inputFile.close();
}
