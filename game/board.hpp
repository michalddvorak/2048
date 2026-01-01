#include <numeric>
#include "../utils/matrix.hpp"
#include "../utils/utils.hpp"


template <std::integral T>
struct board : public matrix<T>
{
    using value_type = T;
    using matrix<T>::matrix;
    
    
    bool merge_multiple(std::ranges::range auto&& rng, auto compose_inner)
    {
        
        return fold_left(rng | std::views::transform([&](std::ranges::range auto&& r)
                                                     {
                                                         return merge(r | compose_inner);
                                                     }), false, std::logical_or {});
    }
    bool merge_multiple(std::ranges::range auto&& rng)
    {
        return merge_multiple(
                std::forward<decltype(rng)>(rng),
                std::views::transform(std::identity{}));
    }
    
    void reset()
    {
        for (auto& x: *this)
            x = T(0);
    }
    
    bool move_left()
    {
        return merge_multiple(rows(*this));
    }
    
    bool move_right()
    {
        return merge_multiple(rows(*this), std::views::reverse);
    }
    
    bool move_up()
    {
        return merge_multiple(columns(*this));
    }
    
    bool move_down()
    {
        return merge_multiple(columns(*this), std::views::reverse);
    }
    
    [[nodiscard]] bool is_lost() const
    {
        auto contains_two_adjacent_equal_elements = [](std::ranges::range auto&& rng)
        {
            return std::ranges::adjacent_find(rng) != std::ranges::end(rng);
        };
        
        if (std::ranges::any_of(*this, is_zero) ||
            std::ranges::any_of(columns(*this), contains_two_adjacent_equal_elements) ||
            std::ranges::any_of(rows(*this), contains_two_adjacent_equal_elements))
            return false;
        
        return true;
    }
    
    [[nodiscard]] T score() const
    {
        return std::accumulate(this->begin(), this->end(), T(0));
    }
    
    /**
     * Puts @num_tiles random tiles of values choosen randomly from the set {1 << 1, 1 << 2, ..., 1 << @tiles_range}
     * @param tiles_range
     * @param num_tiles
     * @param generator
     */
    void put_random_tiles(size_t tiles_range, size_t num_tiles, auto&& generator)
    {
        std::uniform_int_distribution<typename std::decay_t<decltype(generator)>::result_type> distr(1, tiles_range);
        for (size_t n = 0; n < num_tiles; ++n) {
            auto zero_board = *this | std::views::filter(is_zero);
            auto it = take_random(zero_board, generator);
            if (it == std::ranges::end(zero_board))
                return;
            *it = 1 << distr(generator);
        }
    }
};
