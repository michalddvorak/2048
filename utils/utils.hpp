#pragma once

#include <ranges>
#include <algorithm>
#include <sstream>

inline auto is_zero = [](auto x) { return x == 0; };

/**
 * @brief Performs merging of range [inbegin,inend) into range [outbegin,outend) in 2048-like fashion
 * @example [2,2,2,2] -> [4,4,0,0]
 * 			[2,0,2,0] -> [4,0,0,0]
 * @return true, if the range has changed, false otherwise
 */
bool merge(auto inbegin, auto inend, auto outbegin, auto outend)
{
    bool has_changed = false;
    auto append = [&](auto x)
    {
        has_changed = has_changed || *outbegin != x;
        *outbegin++ = x;
    };
    while (true) {
        while (inbegin != inend && *inbegin == 0)
            ++inbegin;
        if (inbegin == inend)
            break;
        auto one = *inbegin++;
        while (inbegin != inend && *inbegin == 0)
            ++inbegin;
        if (inbegin == inend) {
            append(one);
            break;
        }
        auto two = *inbegin;
        if (one == two) {
            append(one + two);
            ++inbegin;
        }
        else
            append(one);
    }
    while (outbegin != outend)
        *outbegin++ = 0;
    return has_changed;
}

template <typename IOIt>
bool merge(IOIt begin, IOIt end)
{
    return merge(begin, end, begin, end);
}

bool merge(std::ranges::range auto&& rng)
{
    return merge(std::ranges::begin(rng), std::ranges::end(rng));
}

auto take_random(std::ranges::range auto&& rng, auto&& generator)
{
    auto it = std::ranges::begin(rng);
    auto end = std::ranges::end(rng);
    auto ret = it;
    size_t len = 0;
    for (; it != end; ++it, ++len) {
        std::uniform_int_distribution<std::size_t> dist(0, len);
        if (dist(generator) == 0)
            ret = it;
    }
    return ret;
}


//---------
namespace detail
{
    struct fold_left_object
    {
        constexpr auto operator()(auto first, auto last, auto init, auto f) const
        {
            using U = std::decay_t<std::invoke_result_t<decltype(f)&, decltype(init), std::iter_reference_t<decltype(first)>>>;
            if (first == last)
                return U(std::move(init));
            U accum = std::invoke(f, std::move(init), *first);
            for (++first; first != last; ++first)
                accum = std::invoke(f, std::move(accum), *first);
            return std::move(accum);
        }
        
        constexpr auto operator()(std::ranges::range auto&& r, auto init, auto f) const
        {
            return (*this)(std::ranges::begin(r), std::ranges::end(r), std::move(init), std::ref(f));
        }
    };
}
inline constexpr detail::fold_left_object fold_left;

//----------

template <typename Fn>
class with_result_of_t
{
  public:
    using result_type = std::invoke_result_t<Fn&>;
    
    explicit with_result_of_t(Fn fn) : m_fn(std::move(fn)) { }
    
    operator result_type() { return std::invoke(m_fn); }
  
  private:
    Fn m_fn;
};

template <typename Fn>
auto with_result_of(Fn&& fn)
{
    return with_result_of_t<std::decay_t<Fn>>(std::forward<Fn>(fn));
}

auto constant(auto x)
{
    return [=]() { return x; };
}

template<typename T>
std::optional<T> parse(const std::string& s)
{
    std::stringstream ss(s);
    T x;
    if(!(ss >> x))
        return std::nullopt;
    return x;
}