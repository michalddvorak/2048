#include <functional>
#include "../utils/ring.hpp"
#include "../io/io.hpp"

class menu
{
  public:
    menu(io& io, std::vector<std::pair<std::string, std::function<void(void)>>> items) : io_(io),
                                                                                         items_names_(
                                                                                                 extract_first(items)),
                                                                                         items_actions_(extract_second(
                                                                                                 items)) { }
    
    void loop(bool& exit)
    {
        while (!exit) {
            io_.clear_screen();
            io_.print_menu(items_names_);
            auto key = io_.get_key();
            switch (key.type) {
                case EKEY::UPARROW: {
                    --items_names_;
                    --items_actions_;
                    break;
                }
                case EKEY::DOWNARROW: {
                    ++items_names_;
                    ++items_actions_;
                    break;
                }
                case EKEY::ENTER: {
                    (*items_actions_)();
                    break;
                }
                default:break;
            }
        }
    }
  
  private:
    
    //TODO move this elsewhere...
    template <typename F, typename S>
    static std::vector<F> extract_first(const std::vector<std::pair<F, S>>& v)
    {
        std::vector<F> ret;
        for (auto& [f, s]: v)
            ret.push_back(f);
        return ret;
    }
    
    template <typename F, typename S>
    static std::vector<S> extract_second(const std::vector<std::pair<F, S>>& v)
    {
        std::vector<S> ret;
        for (auto& [f, s]: v)
            ret.push_back(s);
        return ret;
    }
    
    io& io_;
    ring<std::string> items_names_;
    ring<std::function<void(void)>> items_actions_;
};