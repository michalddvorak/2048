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
    menu(io& io, std::vector<menu_item> items, std::string header="");
    
    void loop(bool& exit);
  
  private:
    void increment_ptr();
    
    void decrement_ptr();
    
    io& io_;
    std::vector<menu_item> items_;
    size_t ptr_ = 0;
    std::string header_;
};
