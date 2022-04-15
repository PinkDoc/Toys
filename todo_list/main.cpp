#include "todo.hpp"
#include <string.h>

int main() {
    bool quit = false;
    todo_list l;
    l.init();
    while (!quit) {
        std::string cmd, args;
        std::cout << "> ";
        std::cin >> cmd;

        if (cmd == "quit") return 1;
        else if (cmd == "list")
        {
            l.show();
        }
        else if (cmd == "del")
        {
            std::cin >> args;
            l.del_todo(args);
        }
        else if (cmd == "add")
        {
            std::cin >> args;
            l.add_todo(args);
        }
        else
        {
            std::cout << "quit, list, del [args], add [args]"  << std::endl;
        }
    }

}
