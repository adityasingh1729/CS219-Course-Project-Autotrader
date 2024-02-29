#include "receiver.h"
// #include "iostream"
#include "string"
#include <cmath>
#include "vector"
#include "map"
#include "algorithm"
#include "iostream"
using namespace std;

int arbitFinder;
int arbitFound;
int order_num = 0;
int offset = 0;
int change = 0;
vector<map<string, int> > bundles {};
map<int, int> arbitrage_set {};

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

void arbitrageChecker(vector<map<string, int> > bundles) 
{
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
                        if (stock.first != "price" && stock.first != "order_num" && stock.first != "direction") {
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
                    // cout << "Arbitrage Found\n";
                    // arbitFound = 1;
                    for (auto stock : checkSet) {
                    netPrice += stock["price"];
                    }
                    // cout << "NetPrice = " << netPrice << endl;
                    if (netPrice > 0) {
                        arbitFound = 1;
                        //std::cout << "Arbitrage Possible, with price " << netPrice << endl;
                        arbitrage_set[subsetIndex] = netPrice;
                    }
                }
            }
        }
    }
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

    arbitFinder = 3;
    arbitFound = 3;
    bundles = {};

    for(string order : orders) {
        order_num++;
        // cout << "order_num = " << order_num << endl;
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
                bundle["order_num"] = order_num;
                if (direction == "s") {
                    bundle["direction"] = -1;
                } else if (direction == "b") {
                    bundle["direction"] = 1;
                }
                bundles.push_back(bundle);
            } else {
                responces.push_back("No trade");
            }
        } 
        // multi-stock order
        else if (words.size() > 3) {
            arbitFinder = 0;
            arbitFound = 0;
            change = 0;
            int fillin = 0;
            int found = 3;
            int j;
            for (j = 0; j < bundles.size(); j++) {
                map<string, int> bundle = bundles[j];
                if (bundles.size() != stocks.size()) {
                    break;
                }
                for (int i = 0; i < stocks.size(); i++) {
                    if (bundle.find(stocks[i]) == bundle.end()) {
                        found = 3;
                        break;
                    } else {
                        if (bundle[stocks[i]] != qttys[i]) {
                            found = 3;
                            break;
                        } else {
                            found = 1;
                        }
                    }
                }
                if (found == 1) {
                    // cout << "Find\n";
                    if (direction == "b") {
                        // cout << "bundle[direction] = " << bundle["direction"] << endl;
                        if (bundle["direction"] == 1) {
                            // cout << "price = " << price << " bundle[price] " << bundle["price"] << endl;
                            if (price > bundle["price"]) {
                                bundle["price"] = price;
                                bundles[j]["price"] = price;
                                bundles[j]["order_num"] = order_num;
                                change = 1;
                            }
                            // cout << "bundle[price] " << bundle["price"] <<  endl;
                            // cout << "bundles[bundles.size() - 1][price] = " << bundles[bundles.size() - 1]["price"] << endl;
                        } else {
                            fillin = 1;
                        }
                    } else {
                        if (bundle["direction"] == -1) {
                            if (price < bundle["price"]) {
                                bundle["price"] = price;
                                bundles[j]["price"] = price;
                                bundles[j]["price"] = order_num;
                                change = 1;
                            }
                        } else {
                            fillin = 1;
                        }
                    }
                    change = 1;
                }
            }
            if (fillin == 1) {
                // cout << "Fillin\n";
                for (int k = j; k < bundles.size() - 1; k++) {
                    bundles[k] = bundles[k + 1];
                }
            }

            if (change != 1) {
                // cout << "Hello\n";
                map<string, int> bundle {};
                for (int j = 0; j < stocks.size(); j++) {
                    if (direction == "s") bundle[stocks[j]] = -1*qttys[j];
                    else bundle[stocks[j]] = qttys[j];
                }
                if (direction == "s") price *= -1;
                bundle["price"] = price;
                bundle["order_num"] = order_num;
                if (direction == "s") {
                    bundle["direction"] = -1;
                } else if (direction == "b") {
                    bundle["direction"] = 1;
                }
                bundles.push_back(bundle);
            }

            // Checking for arbitrage:
            // Finding the subsets of lines using binary numbers

            // cout << "0.bundles.size() = " << bundles.size() << endl;
            arbitrageChecker(bundles);
        }
        // cout << "arbitFound = " << arbitFound << endl;
        if (arbitFound == 0) {
            // cout << "order_num = " << order_num << endl;
            cout << "No Trade" << endl;
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
            int temp = max_set;
            int counter2 = 0;
            vector<int> printSet {};
            while (max_set > 0) {
                if (max_set % 10 == 1) {
                    printSet.push_back(counter2);
                }
                max_set /= 10;
                counter2++;
            }
            max_set = temp;
            for (int i = printSet.size() - 1; i >= 0; i--) {
                //cout << "offset + printSet[i] = " << offset + printSet[i] << endl;
                string printString = "";
                printString = orders[bundles[printSet[i]].at("order_num") - 1];
                char lastCharacter = printString.back();
                printString.erase(printString.length() - 1);
                if (lastCharacter == 's') {
                    printString.append("b#");
                } else {
                    printString.append("s#");
                }
                cout << printString << endl;
                // cout << orders[bundles[printSet[i]].at("order_num") - 1] << endl;
            }
            cout << maxPrice << endl;
            arbitFound = 0;
            vector<map<string, int> > new_bundles {};
            int tracker = 0;
            // cout << "1.bundles.size() = " << bundles.size() << endl;
            while (tracker < bundles.size()) {
                if (max_set > 0) {
                    //cout << "max_set > 0\n";
                    if (max_set % 10 == 0) {
                        new_bundles.push_back(bundles[tracker]);
                        // cout << "Tracker = " << tracker << endl;
                    }
                    max_set /= 10;
                } else {
                    //cout << "max_set <= 0\n";
                    new_bundles.push_back(bundles[tracker]);
                    //cout << "Tracker = " << tracker << endl;
                }
                tracker++;
            }
            bundles = new_bundles;
            arbitrage_set.clear();
            // offset = order_num;
            // cout << "2.bundles.size() = " << bundles.size() << endl;
        }
    }

    message = "";
    for(auto responce : responces) {
        message += responce + "\n";
    }

    std::cout << message;
    return 0;
}
