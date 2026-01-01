#include <functional>
#include "../io/io.hpp"
#include "../utils/utils.hpp"

struct menu_item
{
    std::function<std::string(void)> name;
    std::function<void(void)> action;
};

class menu
{
  public:
    menu(io& io, std::vector<menu_item> items) : io_(io), items_(std::move(items))
    {
    
    }
    
    void loop(bool& exit)
    {
        std::vector<std::string> items_names;
        auto generate_items = [&]()
        {
            items_names.clear();
            for (auto&& [name_callback, _]: items_)
                items_names.emplace_back(with_result_of(name_callback));
        };
        
        while (!exit) {
            io_.clear_screen();
            generate_items();
            io_.print_menu(items_names,ptr);
            auto key = io_.get_key();
            switch (key.type) {
                case EKEY::UPARROW: {
                    decrement_ptr();
                    break;
                }
                case EKEY::DOWNARROW: {
                    increment_ptr();
                    break;
                }
                case EKEY::ENTER: {
                    items_[ptr].action();
                    break;
                }
                default:break;
            }
        }
        io_.clear_screen();
    }
  
  private:
    void increment_ptr()
    {
        if (++ptr == items_.size())
            ptr = 0;
    }
    
    void decrement_ptr()
    {
        if (--ptr == size_t(-1))
            ptr = items_.size() - 1;
    }
    
    io& io_;
    std::vector<menu_item> items_;
    size_t ptr = 0;
};
