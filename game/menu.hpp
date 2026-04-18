#include <functional>
#include "../io/io.hpp"
#include "../utils/utils.hpp"
#include "../utils/modulo_counter.hpp"

struct menu_item
{
    std::function<std::string(void)> name;
    std::function<void(void)> action;
};

template <typename T>
concept CounterLike = requires(T x, size_t size)
{
    T(size); //Initializable from size_t
    --x; //Decrementable
    ++x; //Incrementable
    { x } -> std::convertible_to<size_t>; //Convertible to size_t
};

template <CounterLike Counter>
class menu
{
  public:
    menu(io& io, std::vector<menu_item> items, std::string header = "") : io_(io),
                                                                          items_(std::move(items)),
                                                                          selection_(items.size()),
                                                                          header_(std::move(header)) { }
    
    void loop(bool& exit)
    {
        while (!exit) {
            std::vector<std::string> item_names;
            for (auto&& [name_callback, _]: items_)
                item_names.emplace_back(with_result_of(name_callback));
            io_.clear_screen();
            io_.print_menu(item_names, static_cast<size_t>(selection_), header_);
            auto key = io_.get_key();
            switch (key.type) {
                case EKEY::UPARROW: {
                    --selection_;
                    break;
                }
                case EKEY::DOWNARROW: {
                    ++selection_;
                    break;
                }
                case EKEY::ENTER: {
                    items_[static_cast<size_t>(selection_)].action();
                    break;
                }
                default:break;
            }
        }
        io_.clear_screen();
    }
  
  private:
    io& io_;
    std::vector<menu_item> items_;
    Counter selection_;
    std::string header_;
};


