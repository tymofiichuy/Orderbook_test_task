#pragma once

#include <iostream>
#include <map>
#include <ctime>
#include <string>
#include <regex>
#include <sstream>
#include <vector>
#include <utility>

class order_key{
private:
    uint64_t user_id_;
    uint64_t price_;
    std::time_t timestamp;
public:
    uint64_t get_price() const;
    std::time_t get_timestamp() const;

    bool buy_match(const order_key&);
    bool sell_match(const order_key&);

    order_key(uint64_t user_id, uint64_t price);
};

struct buy_cmp{
    bool operator()(const order_key& l,const order_key& r) const;
};

struct sell_cmp{
    bool operator()(const order_key& l,const order_key& r) const;
};

class order_amount{
private:
    uint64_t amount_;
public:
    uint64_t get_amount() const;

    uint64_t fulfill(uint64_t proposal);

    order_amount(uint64_t amount);
};

class orderbook{
private:
    std::map<order_key, order_amount, buy_cmp> buy_list;
    std::map<order_key, order_amount, sell_cmp> sell_list;
public:
    bool insert_buy_order(order_key&, order_amount&);
    bool insert_sell_order(order_key&, order_amount&);
};

class orderbook_cli{
private:
    //side = true if an order to insert is a buy order
    std::regex main_pattern{R"(^Order\{user_id 0x[a-fA-F0-9]{1,16}, amount 0x[a-fA-F0-9]{1,16}, price 0x[a-fA-F0-9]{1,16}, side (buy|sell)\}$)"};
    std::regex extraction_pattern{R"(0x[a-fA-F0-9]{1,16}|buy|sell)"};

    std::vector<std::string> container;
    
    bool init = false;
public:
    bool tokenize(std::string);

    bool read_side();
    std::pair<order_key, order_amount> read_order_model();
};