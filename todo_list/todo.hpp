#ifndef TODO_HPP
#define TODO_HPP

#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <chrono>
#include <set>
#include <map>

#include <stdio.h>

static char todofile[] = "_todo.txt";

//
// [TODO> go to shopping ]
//

class todo_list {
private:
    std::set<std::string> todo_set_;

    void write_to_file();
    void parse_file(char* f, long len);
public:

    todo_list() = default;
    ~todo_list() { write_to_file(); }

    void add_todo(std::string& name) { todo_set_.insert(name); }
    void del_todo(std::string& name) { todo_set_.erase(name); }

    void init();
    void show();
};

void todo_list::init()
{
    int fd = open("_todo.txt", O_RDONLY);

    if (fd < 0)
    {
        throw std::runtime_error("can't open '_todo.txt'");
    }

    struct stat s{};
    int r = stat("_todo.txt", &s);

    if (r < 0)
    {
        throw  std::runtime_error("can't read stat from '_todo.txt'");
    }

    char buffer[s.st_size];
    auto len = read(fd, buffer, s.st_size);

    parse_file(buffer, len);

    close(fd);
}

void todo_list::show()
{
    int j = 0;
    for (auto &i : todo_set_)
    {
        std::cout << "[TODO" << j << "> " << i << " ]"  <<  std::endl;
    }
}

void todo_list::parse_file(char *f, long len)
{
    int p = 0;

    std::string temp;

    while (p < len)
    {
        switch (f[p]) {
            case '[':   // begin
                break;
            case ']':   // end
                todo_set_.insert(std::move(temp));
                break;
            default:
                temp.push_back(f[p]);
                break;
        }
        p++;
    }

}

void todo_list::write_to_file()
{
    std::string s;
    for (auto& i : todo_set_)
    {
        s.push_back('[');
        s.append(i);
        s.push_back(']');
    }
    FILE* f = fopen("_todo.txt", "w");
    if (f == nullptr)
        throw std::runtime_error("write_to_file fail! Can't open '_todo.txt'");
    fprintf(f, "%s",s.c_str());
    fclose(f);
}




#endif