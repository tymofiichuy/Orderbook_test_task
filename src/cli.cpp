#include "orderbook.hpp"

using namespace std;

bool orderbook_cli::tokenize(std::string md_str){
    if(!regex_match(md_str, main_pattern)){
        init = false;
    }
    else{
        container.clear();

        for(smatch match; regex_search(md_str, match, extraction_pattern);){
            container.push_back(match.str());
            md_str = match.suffix();
        }

        init = true;        
    }

    return init;
}

pair<order_key, order_amount> orderbook_cli::read_order_model(){
    if(!init){
        throw runtime_error("Uninitialized");
    }
    return pair<order_key, order_amount>(order_key(stoll(container[0],0,16),stoll(container[1],0,16)), order_amount(stoll(container[2],0,16)));
};

bool orderbook_cli::read_side(){
    if(!init){
        throw runtime_error("Uninitialized");
    }
    return (container[3].compare("true")==0);
};

void orderbook_cli::print_changes(uint64_t user_id, uint64_t amount, bool base_currency, bool side){
    cout << R"(BalanceChange{user_id )" 
         << user_id 
         << R"(, value )" 
         << (side ? "" : "-")
         << amount 
         << R"(, currency )" 
         << (base_currency ? R"(UAH})" : R"(USD})");
}