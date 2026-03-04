# Orderbook test task


## Description
This project is an example of an orderbook trading component (UAH-USD) implemented in C++. The main
algorithm supports basic operations (like inserting a new order and automatic matching).
It can easily be extended or modifyed due to a class structure that separetes different logical parts into 
different classes and encapsulates critical attributes to avoid misusage.  
Also, the project includes simple command line interface to interact with the underlying algorithm. 


## Main logic
Implementation relies on stl::map data structure (which is based on a red-black tree). Therefore, an orderbook reqires O(n) memory and implements an order insertion in O(log n) time and matching in O(1) amortized time. While logarithmic-time 
insertion is costly, it is necessary to store orders in an ordered structure and avoid expensive matching. Also, imposing an
order on the objects allows to implement additional operations (like removing an order, changing an existing order, etc) in logarithmic time complexity as well, and top/bottom reading oprations in O(1) time.


## CLI
CLI was designed to add a way to interact with an orderbook. It supports only 1 command:
'Order{user_id int64, amount int64, price int64, side sell|buy};' and as output prints 
all transactions that were made in the next format: 'BalanceChange{user_id int64, value int64, currency UAH|USD}'.
Internally, CLI uses a regex-based parsing. That makes it fast, but quite sensitive. Time complexity is 
O(1) due to restriction on the command length.


## Compiling
Project is self-sufficient and designed to be built with CMake. C++ 20+ and CMake 3.10 are reqired.


## Usage
Executable outputs a command signature along with the considerations about the command usage in the console.
Currently an orderbook is stored in RAM, so it can only be used to model an actual orderbook work.