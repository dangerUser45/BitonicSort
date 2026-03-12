#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <limits>
#include <type_traits>

namespace bitsort {

namespace detail {

template <typename IterT, typename CompareT>
inline void compare_exchange(IterT lhs_iter, IterT rhs_iter, CompareT comp)
{
    if (comp(*rhs_iter, *lhs_iter)) {
        std::iter_swap(lhs_iter, rhs_iter);
    }
}

template <typename ValueT, typename CompareT>
inline ValueT padding_value(CompareT)
{
    using comparator_t = std::decay_t<CompareT>;

    if constexpr (std::is_same_v<comparator_t, std::less<>> ||
                  std::is_same_v<comparator_t, std::less<ValueT>>) {
        return std::numeric_limits<ValueT>::max();
    } else if constexpr (std::is_same_v<comparator_t, std::greater<>> ||
                         std::is_same_v<comparator_t, std::greater<ValueT>>) {
        return std::numeric_limits<ValueT>::lowest();
    } else {
        static_assert(std::is_same_v<comparator_t, void>,
                      "Unsupported comparator for automatic padding");
    }
}

template <typename ContainerT, typename CompareT>
inline std::size_t add_extra_elem(ContainerT& cont, std::size_t num_elems, CompareT comp)
{
    std::size_t pow_two = 2;
    while (num_elems > pow_two) {
        pow_two *= 2;
    }

    const std::size_t extra_elems = pow_two - num_elems;
    if (!extra_elems) {
        return 0;
    }

    using value_t = typename ContainerT::value_type;
    cont.insert(cont.end(), extra_elems, padding_value<value_t>(comp));
    return extra_elems;
}

template <typename ContainerT>
inline void remove_extra_elems(ContainerT& cont, std::size_t extra_elems)
{
    cont.erase(std::prev(cont.end(), extra_elems), cont.end());
}

} // namespace detail

template <typename ContainerT, typename CompareT>
inline void bitonic_sort(ContainerT& cont, CompareT comp)
{
    const std::size_t num_elems = cont.size();
    if (num_elems <= 1) {
        return;
    }

    const std::size_t extra_elems = detail::add_extra_elem(cont, num_elems, comp);
    const std::size_t ext_num_elems = num_elems + extra_elems;
    auto first = cont.begin();
    auto last = cont.end();

    for (std::size_t merge_size = 2; merge_size <= ext_num_elems; merge_size *= 2) {
        bool ascending_block = true;
        for (auto it = first; it != last; it += merge_size) {
            for (std::size_t split_size = merge_size / 2; split_size >= 1; split_size /= 2) {
                for (std::size_t block = 0; block < merge_size; block += 2 * split_size) {
                    for (std::size_t idx = 0; idx < split_size; ++idx) {
                        if (ascending_block) {
                            detail::compare_exchange(it + block + idx,
                                                     it + block + idx + split_size,
                                                     comp);
                        } else {
                            detail::compare_exchange(it + block + idx + split_size,
                                                     it + block + idx,
                                                     comp);
                        }
                    }
                }
                if (split_size == 1) {
                    break;
                }
            }
            ascending_block = !ascending_block;
        }
    }

    detail::remove_extra_elems(cont, extra_elems);
}

template <typename ContainerT>
inline void bitonic_sort(ContainerT& cont)
{
    bitonic_sort(cont, std::less<>{});
}

} // namespace bitsort
