#include "orderbook.hpp"

using namespace std;

order_key::order_key(uint64_t user_id, uint64_t price):user_id_(user_id),price_(price){
    time(&timestamp);
};

uint64_t order_key::get_price() const{
    return price_;
};

time_t order_key::get_timestamp() const{
    return timestamp;
};

bool order_key::buy_match(order_key& cmp){
    return price_ > cmp.get_price();
};

bool order_key::sell_match(order_key& cmp){
    return price_ < cmp.get_price();
};

bool buy_cmp::operator()(order_key& l, order_key& r) const{
    if(l.get_price() == r.get_price()){
        return l.get_timestamp() < r.get_timestamp();
    }
    else{
        return l.get_price() > r.get_price();
    }
};

bool sell_cmp::operator()(order_key& l, order_key& r) const{
    if(l.get_price() == r.get_price()){
        return l.get_timestamp() < r.get_timestamp();
    }
    else{
        return l.get_price() < r.get_price();
    }
};

order_amount::order_amount(uint64_t amount):amount_(amount){};

uint64_t order_amount::fulfill(uint64_t proposal){
    uint64_t anw = min(amount_, proposal);
    amount_ -= anw;
    return anw;
};