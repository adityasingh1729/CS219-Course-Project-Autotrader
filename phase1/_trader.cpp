#include "receiver.h"
// #include "iostream"
// #include "string"
#include "vector"
#include "map"
using namespace std;

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
    split_result.push_back(word);

    return split_result;
}

bool accept_trade(int old_price, int curr_price, string direction) {
    if((direction == "b" && old_price < curr_price) || (direction == "s" && old_price > curr_price)) {
        return true;
    }

    return false;
}

int main() {

    Receiver rcv;
    sleep(5);
    string message = rcv.readIML();

    message.pop_back();
    message.pop_back();

    vector<string> orders = split_sentence(message, '#');

    map<string, int> price_buffers;

    vector<string> responces;

    for(string order : orders) {
        vector<string> words = split_sentence(order, ' ');
        vector<string> stocks;
        vector<string> qttys;

        int i = 0;
        for(; i < words.size() - 1 - 2; i += 2) {
            stocks[i] = words[i];
            qttys[i] = stoi(words[i+1]);
        }
        int price = stoi(words[i]);
        string direction = words[i+1];

        // single stock order
        if(words.size() == 4) {
            if(price_buffers.find(stocks[0]) == price_buffers.end() || 
                accept_trade(price_buffers[stocks[0]], price, direction)) {
                string dir = (direction == "s") ? "b" : "s";
                string responce = stocks[0] + " " + to_string(price) + " " + dir;
                price_buffers[stocks[0]] = price;
                responces.push_back(responce);
            } else {
                responces.push_back("No trade");
            }
        } 
        // multi-stock order
        else {
            
        }
    }

    message = "";
    for(auto responce : responces) {
        message += responce + "\n";
    }

    cout << message;


    return 0;
}
