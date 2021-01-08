//14. Контейнер: двунаправленный стек.
//Итераторы: вывода и двунаправленный.
//Сортировка: быстрая сортировка.

#include <iostream>
#include <stdexcept>
#include <string>
#include <deque>
#include <fstream>

namespace ns {

    class Show {
        int id;
        std::string name;
        std::string review;
    public:
        Show(int id, std::string name) : id{ id }, name{ name }, review{""} {}
        Show(int id, std::string name, std::string review): id{ id }, name{ name }, review{ review } { }
        ~Show() = default;
        
        std::string get_name() {
            return name;
        }
        
        std::string get_review() {
            return review;
        }
        
        friend std::ostream & operator<<(std::ostream & stream, Show show) {
            stream << "show " << show.name;
            if (!show.review.empty())
                stream << " review: " << show.review << std::endl;
            else stream << std::endl;
            return stream;
        }
        
        bool operator==(Show show) {
            return show.name == name && show.review == review;
        }
        
        bool operator!=(Show show) {
            return !(*this == show);
        }
        
        bool operator>(Show show) {
            return strcmp(show.name.c_str(), name.c_str()) > 0;
        }
        
        bool operator<(Show show) {
            return strcmp(show.name.c_str(), name.c_str()) < 0;
        }
    };

    template<typename T>
    class Stack {
    private:
        bool use_dublicates = true;
        std::deque<T> container;
    public:
        typedef typename std::deque<T>::iterator iterator;
        
        Stack() { }
        Stack(bool use_dublicates): use_dublicates{ use_dublicates } { }
        
        void push(T data) {
            if (!use_dublicates) {
                iterator dup = find(data);
                if (dup != end()) return;
            }
            
            container.push_front(data);
        }
        
        T pop() {
            if (container.empty())
                throw std::logic_error("stack is empty");
            T data = container.front();
            container.pop_front();
            return data;
        }
        
        T peek() {
            if (container.empty()) {
                throw std::logic_error("stack is empty. peek failed");
            }
            return container.front();
        }
        
        T remove(iterator iter) {
            if (iter == end())
                throw std::logic_error("item not in stack");
            T data = *iter;
            container.erase(iter);
            return data;
        }
        
        iterator find(T element) {
            if (container.empty()) {
                return end();
            }
            
            for (iterator i = begin(); i < end(); i++)
                if (*i == element)
                    return i;
            return end();
        }
        
        iterator begin() {
            return container.begin();
        }
        
        iterator end() {
            return container.end();
        }
        
        int size() const {
            return container.size();
        }
    };

    int read_int() {
        while (true) {
            std::string buffer;
            std::getline(std::cin, buffer);
            try {
                return std::stoi(buffer);
            } catch (...) {
                std::cout << "not a number!" << std::endl;
                continue;
            }
        }
    }

    std::string read_string() {
        while (true) {
            std::string buffer;
            std::getline(std::cin, buffer);
            if (buffer.size() > 0) {
                return buffer;
            }
        }
    }

    Show read_show(int id) {
        std::cout << "enter show name: ";
        std::string name = read_string();
        std::cout << "enter show review (empty if none): ";
        std::string review;
        std::getline(std::cin, review);
        return Show(id, name, review);
    }

    template<typename T>
    void print_stack(Stack<T> &stack) {
        std::cout << "stack items: " << std::endl;
        for (Stack<Show>::iterator i = stack.begin(); i < stack.end(); i++) {
            std::cout << " " << *i << "\n";
        }
    }

    template<typename T>
    using iterator = typename Stack<T>::iterator;
    
    template<typename T>
    iterator<T> find_if(iterator<T> begin, iterator<T> end, std::function<bool(T&)> func) {
        for (iterator<T> i = begin; i < end; i++) {
            if (func(*i)) {
                return i;
            }
        }
        return end;
    }
    
    template<typename T>
    void remove_if(Stack<T> &stack, iterator<T> begin, iterator<T> end, std::function<bool(T&)> func) {
        for (iterator<T> i = begin; i < end; i++) {
            if (func(*i)) {
                stack.remove(i);
            }
        }
    }
    
    void count_in_file(std::string filename, std::string outfilename, char c) {
        std::ifstream file;
        file.exceptions(std::ios::failbit | std::ios::badbit);

        std::ofstream outfile;
        outfile.exceptions(std::ios::failbit);
        
        try {
            std::cout << "opening file '" << filename << "'" << std::endl;
            file.open(filename, std::ios::in);
            
            std::cout << "opening out file '" << outfilename << "'" << std::endl;
            outfile.open(outfilename, std::ios::out);
        } catch(std::system_error error) {
            std::cout << "failed to open file: file doesn't exists" << std::endl;
            
            if (file.is_open())
                file.close();
            return;
        }
        
        file.exceptions(file.exceptions() & ~std::ios::failbit);
        outfile.exceptions(outfile.exceptions() & ~std::ios::failbit);
        
        outfile << "counting " << c << " in " << filename << std::endl;
        while (file) {
            std::string line;
            std::getline(file, line, '\n');
            
            std::cout << "next line: " << line << std::endl;
            size_t count = std::count(line.begin(), line.end(), c);
            std::cout << "count of " << c << ": " << count << std::endl;
        
            outfile << std::to_string(count) << ": " << line << std::endl;
        }
        
        std::cout << "closing files" << std::endl;
        file.close();
        outfile.close();
    }
    
}

bool no_review(ns::Show& show) {
    return show.get_review().empty();
}

void stack_main() {
    std::cout << "use dublicates in stack? ('y' to use): ";
    std::string choice = ns::read_string();
    
    ns::Stack<ns::Show> stack(choice[0] == 'y');
    
    std::cout << "enter count for the stack: ";
    int count = 0;
    while (true) {
        count = ns::read_int();
        if (count < 0) {
            std::cout << "count cannot be < 0" << std::endl;
        } else break;
    }
    
    std::cout << "items for the stack:" << std::endl;
    for (int i = 0; i < count; i++) {
        std::cout << "item " << (i + 1) << std::endl;
        ns::Show show = ns::read_show(i);
        stack.push(show);
        std::cout << std::endl;
    }
    
    ns::print_stack(stack);
    
    std::cout << "enter index to get item from the stack: ";
    int index = ns::read_int();
    
    try {
        ns::Show item = *(stack.begin() + index);
        std::cout << index << " item is " << item << std::endl;
    } catch (std::logic_error error) {
        std::cout << "error getting " << index << " item from the stack\n";
        std::cout << error.what() << std::endl;
    }
    
    std::cout << "enter number of items to remove from the top of the stack: ";
    index = ns::read_int();
    
    if (index >= 0) {
        for (int i = 0; i < index; i++) {
            try {
                stack.pop();
            } catch(std::logic_error) {
                std::cout << "bottom of the stack has been reached" << std::endl;
                break;
            }
        }
        std::cout << "removed " << index << " items" << std::endl;
    } else {
        std::cout << index << " is invalid number of items" << std::endl;
    }
    
    ns::print_stack(stack);

    std::cout << "removing shows with no review" << std::endl;
    ns::remove_if<ns::Show>(stack, stack.begin(), stack.end(), no_review);
    
    ns::print_stack(stack);
}

void file_main() {
    std::cout << "enter file name to count characters: ";
    std::string filename = ns::read_string();
    
    std::cout << "enter file name to store results: ";
    std::string outfilename = ns::read_string();
    
    std::cout << "enter character to count: ";
    std::string choice = ns::read_string();
    
    std::cout << "characrer '" << choice[0] << "' will be counted in " << filename << std::endl;
    
    ns::count_in_file(filename, outfilename, choice[0]);
}

int main(int argc, const char * argv[]) {
    std::cout << "select task: " << std::endl;
    std::cout << "[f]ile algorithm" << std::endl;
    std::cout << "[s]tack algorithm" << std::endl;
    
    std::string choice;
    while (true) {
        choice = ns::read_string();
        if (choice == "s") {
            stack_main();
            return 0;
        } else if (choice == "f") {
            file_main();
            return 0;
        } else std::cout << "invalid choice. please, select [s]tack or [f]ile\n";
    }
}

