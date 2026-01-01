#include "menu.hpp"

menu::menu(io& io, std::vector<menu_item> items, std::string header) : io_(io),
                                                                       items_(std::move(items)),
                                                                       header_(std::move(header))
{

}

void menu::loop(bool& exit)
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
        io_.print_menu(items_names, ptr_, header_);
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
                items_[ptr_].action();
                break;
            }
            default:break;
        }
    }
    io_.clear_screen();
}

void menu::increment_ptr()
{
    if (++ptr_ == items_.size())
        ptr_ = 0;
}

void menu::decrement_ptr()
{
    if (--ptr_ == size_t(-1))
        ptr_ = items_.size() - 1;
}
