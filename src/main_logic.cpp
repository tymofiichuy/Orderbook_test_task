#include "orderbook.hpp"

using namespace std;

order_key::order_key(uint64_t user_id, uint64_t price):user_id_(user_id),price_(price){
    time(&timestamp);
}

uint64_t order_key::get_price() const{
    return price_;
}

time_t order_key::get_timestamp() const{
    return timestamp;
}

bool order_key::buy_match(const order_key& cmp){
    return price_ > cmp.get_price();
}

bool order_key::sell_match(const order_key& cmp){
    return price_ < cmp.get_price();
}

bool buy_cmp::operator()(order_key& l, order_key& r) const{
    if(l.get_price() == r.get_price()){
        return l.get_timestamp() < r.get_timestamp();
    }
    else{
        return l.get_price() > r.get_price();
    }
}

bool sell_cmp::operator()(order_key& l, order_key& r) const{
    if(l.get_price() == r.get_price()){
        return l.get_timestamp() < r.get_timestamp();
    }
    else{
        return l.get_price() < r.get_price();
    }
}

order_amount::order_amount(uint64_t amount):amount_(amount){};

uint64_t order_amount::get_amount() const{
    return amount_;
}

uint64_t order_amount::fulfill(uint64_t proposal){
    uint64_t anw = min(amount_, proposal);
    amount_ -= anw;
    return anw;
}

bool orderbook::insert_buy_order(order_key& key, order_amount& amount){
    uint64_t anw;
    while((!sell_list.empty())&&key.buy_match(sell_list.begin()->first)){
        anw = sell_list.begin()->second.fulfill(amount.get_amount());
        amount.fulfill(anw);

        if(sell_list.begin()->second.get_amount() == 0){
            sell_list.erase(sell_list.begin());
        }            
        if(amount.get_amount() == 0){
            return true;
        }
    }
    if(amount.get_amount() != 0){
        return buy_list.emplace(key, amount).second;
    }
    return false;
}

bool orderbook::insert_sell_order(order_key& key, order_amount& amount){
    uint64_t anw;
    while((!buy_list.empty())&&key.sell_match(buy_list.begin()->first)){
        anw = buy_list.begin()->second.fulfill(amount.get_amount());
        amount.fulfill(anw);

        if(buy_list.begin()->second.get_amount() == 0){
            buy_list.erase(buy_list.begin());
        }
        if(amount.get_amount() == 0){
            return true;
        }
    }
    if(amount.get_amount() != 0){
        return sell_list.emplace(key, amount).second;
    }
    return false;
}