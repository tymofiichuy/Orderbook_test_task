#include "orderbook.hpp"

using namespace std;

string orderbook_cli::to_hex_string(uint64_t num){
    return string("0x").append(format("{:x}",num));
}

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
        throw runtime_error("Uninitialized\n");
    }
    return pair<order_key, order_amount>(order_key(stoll(container[0],0,16),stoll(container[2],0,16)), order_amount(stoll(container[1],0,16)));
};

bool orderbook_cli::read_side(){
    if(!init){
        throw runtime_error("Uninitialized\n");
    }
    return (container[3].compare("buy")==0);
};

void orderbook_cli::print_changes(uint64_t user_id, uint64_t amount, bool base_currency, bool side){
    cout << R"(BalanceChange{user_id )" 
         << to_hex_string(user_id)  
         << R"(, value )" 
         << (side ? "+" : "-")
         << to_hex_string(amount) 
         << R"(, currency )" 
         << (base_currency ? R"(UAH};)" : R"(USD};)")
         << "\n";
}