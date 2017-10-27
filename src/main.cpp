#include <iostream>
#include <sstream>
#include "bplustree.h"
#include "definitions.h"

std::string introMessage(size_t aOrder) {
    std::ostringstream oss;
    oss << "B+ Tree of Order " << aOrder << std::endl;
    oss << "C++ implementation by Amittai Aviram -- afa13@columbia.edu -- Version " << VERSION << std::endl;
    oss << "Following Silberschatz, Korth, Sidarshan, Database Concepts, 5th ed." << std::endl;
    oss << "To build a B+ tree of a different order, start again and enter the order" << std::endl;
    oss << "as an integer argument:  bpt <order>  " << std::endl;
    oss << "(" << MIN_ORDER << " <= order <= " << MAX_ORDER << ")." << std::endl;
    oss << "To start with input from a file of newline-delimited integers," << std::endl;
    oss << "start again and enter the order followed by the filename:" << std::endl;
    oss << "bpt <order> <inputfile> ." << std::endl << std::endl;
    return oss.str();
}

std::string usageMessage() {
    std::string message =
    "Enter any of the following commands after the prompt > :\n"
    "\ti <k>  -- Insert <k> (an integer, <k> >= 0) as both key and value).\n"
    "\tf <k>  -- Find the value under key <k>.\n"
    "\tp <k> -- Print the path from the root to key k and its associated value.\n"
    "\tr <k1> <k2> -- Print the keys and values found in the range [<k1>, <k2>]\n"
    "\td <k>  -- Delete key <k> and its associated value.\n"
    "\tx -- Destroy the whole tree.  Start again with an empty tree of the same order.\n"
    "\tt -- Print the B+ tree.\n"
    "\tl -- Print the keys of the leaves (bottom row of the tree).\n"
    "\tv -- Toggle output of pointer addresses (\"verbose\") in tree and leaves.\n"
    "\tq -- Quit. (Or use Ctl-D.)\n"
    "\t? -- Print this help message.\n\n";
    return message;
}

size_t getOrder(int argc, const char * argv[]) {
    if (argc > 1) {
        size_t order = 0;
        std::istringstream iss(argv[1]);
        if ((iss >> order) && iss.eof() &&
            order >= MIN_ORDER && order <= MAX_ORDER) {
            return order;
        } else {
            std::cerr << "Invalid order specification: "
            << argv[1] << std::endl;
            std::cerr << "Order must be an integer such that " << MIN_ORDER
            << " <= <order> <= " << MAX_ORDER << std::endl;
            std::cerr << "Proceeding with order " << DEFAULT_ORDER << std::endl;
        }
    }
    return DEFAULT_ORDER;
}



int main(int argc, const char * argv[]) {
    char instruction;
    int key = 0;
    bool quit = false;
    bool verbose = false;
    size_t order = getOrder(argc, argv);
    std::cout << introMessage(order);
    std::cout << usageMessage();
    BPlusTree tree(order);
    if (argc > 2) {
        tree.readInputFromFile(argv[2]);
        std::cout << "Input from file " << argv[2] << ":" << std::endl;
        tree.print();
    }
    while (!quit)
    {
        std::cout << "> ";
        std::cin >> instruction;
        switch (instruction) {
            case 'd':
                std::cin >> key;
                tree.remove( Key( key ) );
                tree.print(verbose);
                break;
            case 'i':
                std::cin >> key;
                if (key < 0) {
                    std::cout << usageMessage();
                }
                tree.insert( Key( key ), Value( key ) );
                tree.print(verbose);
                break;
            case 'f':
                std::cin >> key;
                tree.printValue( Key( key ) );
                break;
            case 'l':
                tree.printLeaves(verbose);
                break;
            case 'p':
                std::cin >> key;
                tree.printPathTo( Key( key ) , verbose);
                break;
            case 'q':
                quit = true;
                break;
            case 'r':
                int key2;
                std::cin >> key;
                std::cin >> key2;
                tree.printRange( Key( key ) , Key( key2 ) );
                break;
            case 't':
                tree.print(verbose);
                break;
            case 'v':
                verbose = !verbose;
                tree.print(verbose);
                break;
            case 'x':
                tree.destroyTree();
                tree.print();
                break;
            case '?':
                std::cout << usageMessage();
                break;
            default:
                std::cin.ignore(256, '\n');
                std::cout << usageMessage();
                break;
        }
    }
    return 0;
}
