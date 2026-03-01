#include "orderbook.hpp"

using namespace std;

int main(){
    orderbook book;

    string in = "";
    bool flag = true;

    cout << "Command syntax:\n"
         << "'Order{user_id int64, amount int64, price int64, side buy|sell};' - add order;\n"
         << "'Quit;' - close orderbook.\n"
         << "Note that commands are case-sensitive and were desigend to accept only a hex-formated numbers (starting with 0x).\n";

    while (true){
        getline(cin, in);

        if(in == "Quit;"){
            break;
        }
        else{
            try{flag = book.insert_via_cli(in);}
            catch(const exception exc){cerr << exc.what();}

            if(!flag){
                cerr << "Unable to insert";
            }
        }
    }

    return 0;
}