#pragma once

#include <iostream>
#include <map>
#include <ctime>
// #include <regex>

class order_key{
private:
    uint64_t user_id_;
    uint64_t price_;
    std::time_t timestamp;
public:
    uint64_t get_price() const;
    std::time_t get_timestamp() const;

    bool buy_match(order_key&);
    bool sell_match(order_key&);

    order_key(uint64_t user_id, uint64_t price);
};

struct buy_cmp{
    bool operator()(order_key l, order_key r) const;
};

struct sell_cmp{
    bool operator()(order_key l, order_key r) const;
};

class order_amount{
private:
    uint64_t amount_;
public:
    uint64_t fulfill(uint64_t proposal);

    order_amount(uint64_t amount);
};

class orderbook{
private:
    std::map<order_key, order_amount, buy_cmp> buy_list;
    std::map<order_key, order_amount, sell_cmp> sell_list;
public:
    bool insert(order_key, order_amount);
};