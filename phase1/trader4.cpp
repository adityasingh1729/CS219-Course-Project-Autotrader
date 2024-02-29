#include "receiver.h"
// #include "iostream"
// #include "string"
#include <cmath>
#include "vector"
#include "map"
#include "algorithm"
#include "iostream"
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
            if(x != '\n') {
                word = word + x;
            }
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

// Function to convert number to binary
int conv_bin(int num) 
{
    int result = 0;
    int mul = 1;
    int rem;
    while (num > 0) {
        result += mul*(num % 2);
        mul *= 10;
        num /= 2;
    }
    return result;
}

// Function to check number of sets in the proposed subset
int subsetCardinality(int num)
{
    int result = 0;
    while (num > 0) {
        if (num % 10 == 1) result++;
        num /= 10;
    }
    return result;
}

int main() {
    Receiver rcv;
    sleep(5);
    string message = rcv.readIML();

    //string message = "X 1 Y -1 10 b#\nZ -1 Y 1 -15 b#";

    /*
    X 1 Y -1 10 b#
    Z -1 Y 1 -15 b#
    X 1 Y -1 20 b#
    Z -1 Y 1 -10 b#
    Z 1 X -1 15 b#
    Z 1 X -1 20 b#
    Z 1 X -1 10 b#
    */

    // message.pop_back();
    // message.pop_back();

    vector<string> orders = split_sentence(message, '#');
    orders.pop_back();

    map<string, int> price_buffers;

    vector<string> responces;

    int arbitFinder = 3;
    int arbitFound = 0;
    vector<map<string, int> > bundles {};
    map<int, int > arbitrage_set {};

    for(string order : orders) {
        vector<string> words = split_sentence(order, ' ');
        vector<string> stocks;
        vector<int> qttys;

        int i = 0;
        for(; i < words.size() - 1 - 2; i += 2) {
            stocks.push_back(words[i]);

            qttys.push_back(stoi(words[i+1]));
        }
        
        int price = 0;
        string direction = "";

        if(i == 0) {
            stocks.push_back(words[i]);
            price = stoi(words[i+1]);
            direction = words[i+2];
        } else {
            price = stoi(words[i]);
            direction = words[i+1];
        }

        // single stock order
        if(words.size() == 3) {
            if(price_buffers.find(stocks[0]) == price_buffers.end() || 
                accept_trade(price_buffers[stocks[0]], price, direction)) {
                string dir = (direction == "s") ? "b" : "s";
                string responce = stocks[0] + " " + to_string(price) + " " + dir;
                price_buffers[stocks[0]] = price;
                responces.push_back(responce);
                map<string, int> bundle {};
                bundle[stocks[0]] = stoi(words[1]);
                bundle["price"] = price;
                bundles.push_back(bundle);
            } else {
                responces.push_back("No trade");
            }
        } 
        // multi-stock order
        else if (words.size() > 3) {
            arbitFinder = 0;
            map<string, int> bundle {};
            for (int j = 0; j < stocks.size(); j++) {
                bundle[stocks[j]] = qttys[j];
            }
            if (direction == "s") price *= -1;
            bundle["price"] = price;
            bundles.push_back(bundle);

            // Checking for arbitrage:
            // Finding the subsets of lines using binary numbers
            if (bundles.size() > 1) {
                int num_subsets = pow(2, bundles.size());
                for (int j = 0; j < num_subsets; j++) {
                    // Taking a subset
                    int subsetIndex = conv_bin(j);
                    int temp = subsetIndex;
                    if (subsetCardinality(subsetIndex) > 1) {
                        vector<map<string, int> > checkSet;
                        int counter = 0;
                        while (subsetIndex > 0) {
                            if (subsetIndex % 10 == 1) {
                                checkSet.push_back(bundles[counter]);
                            }
                            subsetIndex /= 10;
                            counter++; 
                        }
                        subsetIndex = temp;
                        vector<string> checkedStocks {};
                        int netPrice = 0;
                        arbitFinder = 1; // Result = 0 => arbitrage not possible, Result = 1 => arbitrage possible
                        for (int k = 0; k < checkSet.size(); k++) {
                            int netQuantity = 0;
                            for (auto stock : checkSet[k]) {
                                if (stock.first != "price") {
                                    auto it = find(checkedStocks.begin(), checkedStocks.end(), stock.first);
                                    if (it == checkedStocks.end()) {
                                        int checkIterator = k;
                                        while (checkIterator < checkSet.size()) {
                                            netQuantity += checkSet[checkIterator][stock.first];
                                            checkIterator++;
                                        }
                                        if (netQuantity != 0) {
                                            k = checkSet.size();
                                            arbitFinder = 0;
                                            break;
                                        }
                                        checkedStocks.push_back(stock.first);
                                    }
                                }
                            }
                        }
                        if (arbitFinder == 1) {
                            arbitFound = 1;
                            for (auto stock : checkSet) {
                            netPrice += stock["price"];
                            }
                            if (netPrice > 0) {
                                //std::cout << "Arbitrage Possible, with price " << netPrice << endl;
                                arbitrage_set[subsetIndex] = netPrice;
                            }
                        }
                    }
                }
            }
        }
        // cout << "arbitFinder = " << arbitFinder << endl;
        if (arbitFinder == 0) {
            cout << "No Trade" << endl;
        }
    }
    /// Finding the best arbitrage possibility
    if (arbitFound == 1) {
        int max_set = 0;
        int maxPrice = 0;
        for (auto set : arbitrage_set) {
            if (set.second > maxPrice) {
                max_set = set.first;
                maxPrice = set.second;
            }
        }
        int counter2 = 0;
        vector<int> printSet {};
        while (max_set > 0) {
            if (max_set % 10 == 1) {
                printSet.push_back(counter2);
            }
            max_set /= 10;
            counter2++;
        }
        cout << "Exited while loop" << endl;
        for (int i = printSet.size() - 1; i >= 0; i--) {
            cout << orders[printSet[i]] << endl;
        }
        cout << maxPrice << endl;
    }

    message = "";
    for(auto responce : responces) {
        message += responce + "\n";
    }

    std::cout << message;
    return 0;
}
