#include <ranges>
#include <algorithm>

auto is_zero = [](auto x) { return x == 0; };

/**
 * @brief Performs merging of range [inbegin,inend) into range [outbegin,outend) in 2048-like fashion
 * @example [2,2,2,2] -> [4,4,0,0]
 * 			[2,0,2,0] -> [4,0,0,0]
 * @return true, if the range has changed, false otherwise
 */
template <typename InputIt, typename OutputIt>
bool merge(InputIt inbegin, InputIt inend, OutputIt outbegin, OutputIt outend)
{
    bool has_changed = false;
    auto append = [&](int x)
    {
        has_changed = has_changed || *outbegin != x;
        *outbegin++ = x;
    };
    while (true) {
        while (inbegin != inend && *inbegin == 0)
            ++inbegin;
        if (inbegin == inend)
            break;
        int one = *inbegin++;
        while (inbegin != inend && *inbegin == 0)
            ++inbegin;
        if (inbegin == inend) {
            append(one);
            break;
        }
        int two = *inbegin;
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

auto take_random(std::ranges::range auto&& rng, auto&& gen)
{
    auto it = std::ranges::begin(rng);
    auto ret = it;
    size_t len = 0;
    for (; it != std::ranges::end(rng); ++it)
        if (gen() % ++len == 0)
            ret = it;
    return ret;
}