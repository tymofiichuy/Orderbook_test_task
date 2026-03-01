#include "orderbook.hpp"

using namespace std;

order_key::order_key(uint64_t user_id, uint64_t price):user_id_(user_id),price_(price){
    time(&timestamp);
}

uint64_t order_key::get_user_id() const{
    return user_id_;
}

uint64_t order_key::get_price() const{
    return price_;
}

time_t order_key::get_timestamp() const{
    return timestamp;
}

bool order_key::buy_match(const order_key& cmp){
    return price_ >= cmp.get_price();
}

bool order_key::sell_match(const order_key& cmp){
    return price_ <= cmp.get_price();
}

bool buy_cmp::operator()(const order_key& l,const order_key& r) const{
    if(l.get_price() == r.get_price()){
        return l.get_timestamp() < r.get_timestamp();
    }
    else{
        return l.get_price() > r.get_price();
    }
}

bool sell_cmp::operator()(const order_key& l,const order_key& r) const{
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

// bool orderbook::insert_buy_order(order_key& key, order_amount& amount){
//     uint64_t anw;
//     while((!sell_list.empty())&&key.buy_match(sell_list.begin()->first)){
//         anw = sell_list.begin()->second.fulfill(amount.get_amount());
//         amount.fulfill(anw);

//         cli.print_changes(sell_list.begin()->first.get_user_id(), anw, true, false);
//         cli.print_changes(sell_list.begin()->first.get_user_id(), sell_list.begin()->first.get_price()*anw, false, true);

//         cli.print_changes(key.get_user_id(), anw, true, true);
//         cli.print_changes(key.get_user_id(), sell_list.begin()->first.get_price()*anw, false, false);

//         if(sell_list.begin()->second.get_amount() == 0){
//             sell_list.erase(sell_list.begin());
//         }            
//         if(amount.get_amount() == 0){
//             return true;
//         }
//     }
//     if(amount.get_amount() != 0){
//         return buy_list.emplace(key, amount).second;
//     }
//     return false;
// }

// bool orderbook::insert_sell_order(order_key& key, order_amount& amount){
//     uint64_t anw;
//     while((!buy_list.empty())&&key.sell_match(buy_list.begin()->first)){
//         anw = buy_list.begin()->second.fulfill(amount.get_amount());
//         amount.fulfill(anw);

//         cli.print_changes(buy_list.begin()->first.get_user_id(), anw, true, true);
//         cli.print_changes(buy_list.begin()->first.get_user_id(), buy_list.begin()->first.get_price()*anw, false, false);

//         cli.print_changes(key.get_user_id(), anw, true, false);
//         cli.print_changes(key.get_user_id(), buy_list.begin()->first.get_price()*anw, false, true);

//         if(buy_list.begin()->second.get_amount() == 0){
//             buy_list.erase(buy_list.begin());
//         }
//         if(amount.get_amount() == 0){
//             return true;
//         }
//     }
//     if(amount.get_amount() != 0){
//         return sell_list.emplace(key, amount).second;
//     }
//     return false;
// }

bool orderbook::insert_buy_order(pair<order_key, order_amount> md_pair){
    uint64_t anw;
    while((!sell_list.empty())&&md_pair.first.buy_match(sell_list.begin()->first)){
        anw = sell_list.begin()->second.fulfill(md_pair.second.get_amount());
        md_pair.second.fulfill(anw);

        cli.print_changes(sell_list.begin()->first.get_user_id(), anw, true, false);
        cli.print_changes(sell_list.begin()->first.get_user_id(), sell_list.begin()->first.get_price()*anw, false, true);

        cli.print_changes(md_pair.first.get_user_id(), anw, true, true);
        cli.print_changes(md_pair.first.get_user_id(), sell_list.begin()->first.get_price()*anw, false, false);

        if(sell_list.begin()->second.get_amount() == 0){
            sell_list.erase(sell_list.begin());
        }            
        if(md_pair.second.get_amount() == 0){
            return true;
        }
    }
    if(md_pair.second.get_amount() != 0){
        return buy_list.emplace(md_pair.first, md_pair.second).second;
    }
    return false;
}

bool orderbook::insert_sell_order(pair<order_key, order_amount> md_pair){
    uint64_t anw = 0;
    while((!buy_list.empty())&&md_pair.first.sell_match(buy_list.begin()->first)){
        anw = buy_list.begin()->second.fulfill(md_pair.second.get_amount());
        md_pair.second.fulfill(anw);

        cli.print_changes(buy_list.begin()->first.get_user_id(), anw, true, true);
        cli.print_changes(buy_list.begin()->first.get_user_id(), buy_list.begin()->first.get_price()*anw, false, false);

        cli.print_changes(md_pair.first.get_user_id(), anw, true, false);
        cli.print_changes(md_pair.first.get_user_id(), buy_list.begin()->first.get_price()*anw, false, true);

        if(buy_list.begin()->second.get_amount() == 0){
            buy_list.erase(buy_list.begin());
        }
        if(md_pair.second.get_amount() == 0){
            return true;
        }
    }
    if(md_pair.second.get_amount() != 0){
        return sell_list.emplace(md_pair.first, md_pair.second).second;
    }
    return false;
}

bool orderbook::insert_via_cli(std::string md_str){
    if(!cli.tokenize(md_str)){
        throw invalid_argument("Unable to read\n");
    };

    if(cli.read_side()){
        return insert_buy_order(cli.read_order_model());
    }
    else{
        return insert_sell_order(cli.read_order_model());
    }
};