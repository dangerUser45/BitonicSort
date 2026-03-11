#pragma once

#include <algorithm>
#include <functional>
#include <limits>

namespace bitsort {

template <typename T, typename CompareT>
void compare_exchange(T lhs_iter, T rhs_iter,CompareT comp) {
    if(comp(*rhs_iter, *lhs_iter))
        std::iter_swap(lhs_iter, rhs_iter);
}

template <typename T, typename CompareT>
T padding_value(CompareT)
{
    using C = std::decay_t<CompareT>;

    if constexpr (std::is_same_v<C, std::less<>> ||
                  std::is_same_v<C, std::less<T>>) {
        return std::numeric_limits<T>::max();
    } else if constexpr (std::is_same_v<C, std::greater<>> ||
                         std::is_same_v<C, std::greater<T>>) {
        return std::numeric_limits<T>::lowest();
    } else {
        static_assert(std::is_same_v<C, void>,
                      "Unsupported comparator for automatic padding");
    }
}

template <typename ContainerT, typename CompareT>
std::size_t add_extra_elem(ContainerT& cont, std::size_t num_elems, CompareT comp)
{
    std::size_t pow_two = 2;
    while(num_elems > pow_two) { pow_two *= 2; }

    std::size_t extra_elems = pow_two - num_elems;
    if(!extra_elems) return 0;

    using value_t = typename ContainerT::value_type;
    value_t value = padding_value<value_t>(comp);

    cont.insert(cont.end(), extra_elems, value);
    return extra_elems;
}

template <typename ContainerT>
void remove_extra_elems(ContainerT& cont, std::size_t extra_elems)
{
    cont.erase(std::prev(cont.end(), extra_elems), cont.end());
}

template <typename ContainerT, typename CompareT>
inline void bitonic_sort(ContainerT& cont, CompareT comp)
{
    std::size_t num_elems = cont.size();
    if (num_elems <= 1)
        return;

    std::size_t extra_elems = add_extra_elem(cont, num_elems, comp);
    size_t ext_num_elems = num_elems + extra_elems;
    auto first = cont.begin();
    auto last = cont.end();

    for (auto merge_size = 2; merge_size <= ext_num_elems; merge_size *= 2) {
        bool ascending_block = true;
        for (auto it = first; it != last; it += merge_size) {
            if (ascending_block) {
                for (auto split_size = merge_size / 2; split_size >= 1; split_size /= 2) {
                    for (auto block = 0; block < merge_size; block += 2 * split_size) {
                        for (auto idx = 0; idx < split_size; ++idx) {
                            compare_exchange(it + block + idx,
                                             it + block + idx + split_size,
                                             comp);
                        }
                    }
                    if (split_size == 1)
                        break;
                }
            } else {
                for (auto split_size = merge_size / 2; split_size >= 1; split_size /= 2) {
                    for (auto block = 0; block < merge_size; block += 2 * split_size) {
                        for (auto idx = 0; idx < split_size; ++idx) {
                            compare_exchange(it + block + idx + split_size,
                                             it + block + idx,
                                             comp);
                        }
                    }
                    if (split_size == 1)
                        break;
                }
            }
            ascending_block = !ascending_block;
        }
    }

    remove_extra_elems(cont, extra_elems);
}

template <typename ContainerT>
inline void bitonic_sort(ContainerT& cont)
{
    bitonic_sort(cont, std::less());
}

} // namespace bitsort
