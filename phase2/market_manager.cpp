#include "utils.cpp"
using namespace std;

void update_buffers(buffer_dict& sell_buffer, buffer_dict& buy_buffer) {
  for(auto& bucket : sell_buffer.table) {
    for(auto& pair : bucket) {
      auto curr_ptr = pair.value.root;

      while(curr_ptr) {
        if(curr_ptr->data.life_remaining == 0) {
          pair.value.remove(curr_ptr);
          
          curr_ptr = curr_ptr->next_node;
          continue;
        }

        if(curr_ptr->data.life_remaining != -1) {
          curr_ptr->data.life_remaining--;
        }

        curr_ptr = curr_ptr->next_node;
      }

      delete curr_ptr;
    }
  }

  for(auto& bucket : buy_buffer.table) {
    for(auto& pair : bucket) {
      auto curr_ptr = pair.value.root;

      while(curr_ptr) {
        if(curr_ptr->data.life_remaining == 0) {
          pair.value.remove(curr_ptr);

          curr_ptr = curr_ptr->next_node;
          continue;
        }

        if(curr_ptr->data.life_remaining != -1) {
          curr_ptr->data.life_remaining--;
        }

        curr_ptr = curr_ptr->next_node;
      }

      delete curr_ptr;
    }
  }
}

void market_manager(buffer_dict& sell_buffer, buffer_dict& buy_buffer, vector<successful_exchange>& exchanges) {
  Node* sell_pointer;
  Node* buy_pointer;

  for(auto& bucket : sell_buffer.table) {
    for(auto& pair : bucket) {
      if(!buy_buffer.contains(pair.key)) {
        continue;
      }

      sell_pointer = pair.value.root;
      buy_pointer  = buy_buffer.at(pair.key).root;

      while(sell_pointer && buy_pointer) {
        // order is accepted
        if(sell_pointer->data.price <= buy_pointer->data.price) {
          int price;
          int qtty = min(sell_pointer->data.quantity, buy_pointer->data.quantity);
          if(sell_pointer->data.time_of_entry <= buy_pointer->data.time_of_entry) {
            price = sell_pointer->data.price;
          } else {
            price = buy_pointer->data.price;
          }

          successful_exchange exchange;
          exchange.stock_name = pair.key;
          exchange.seller_name = sell_pointer->data.broker_name;
          exchange.buyer_name = buy_pointer->data.broker_name;
          exchange.price_per_share = price;
          exchange.num_shares = qtty;
          
          sell_pointer->data.quantity -= qtty;
          buy_pointer->data.quantity -= qtty;

          exchanges.push_back(exchange);

          if(sell_pointer->data.quantity == 0) {
            auto temp = sell_pointer->next_node;
            pair.value.remove(sell_pointer);
            sell_pointer = temp;
          } 
          else {
            auto temp = buy_pointer->next_node;
            buy_buffer[exchange.stock_name].remove(buy_pointer);
            buy_pointer = temp;
          }
        } 
        // order is declined
        else {
          // seller is selling at a price higher than the buyer is willing to buy
          // the subsequent buyers are gonna buy at a lower price and sellers are gonna charge higher
          break;
          // what i want is that this goes to the next stock
        }
      }

    }
  }
}