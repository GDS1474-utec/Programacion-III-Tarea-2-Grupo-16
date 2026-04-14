#ifndef CORE_NUMERIC_H
#define CORE_NUMERIC_H

#include <concepts>
#include <iterator>
#include <type_traits>
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace core_numeric {

// =========================
// CONCEPTS OBLIGATORIOS
// =========================
template <typename C>
concept Iterable = requires(C c) {
    std::begin(c);
    std::end(c);
};

template <typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

template <typename T>
concept Divisible = requires(T a, std::size_t n) {
    { a / n } -> std::same_as<T>;
};

// =========================
// CONCEPT PROPIO
// =========================
template <typename T>
concept NumericLike =
requires(T a, T b, std::size_t n) {
    { T{} } -> std::same_as<T>;
    { a + b } -> std::same_as<T>;
    { a - b } -> std::same_as<T>;
    { a * b } -> std::same_as<T>;
    { a / n } -> std::convertible_to<T>;
    { a < b } -> std::convertible_to<bool>;
};

// =========================
// SUM
// =========================
template <Iterable C>
requires Addable<typename C::value_type>
auto sum(const C& container) {
    using T = typename C::value_type;
    T result{};
    for (const auto& v : container)
        result = result + v;
    return result;
}

// =========================
// MEAN
// =========================
template <Iterable C>
requires Addable<typename C::value_type> &&
         Divisible<typename C::value_type>
auto mean(const C& container) {
    using T = typename C::value_type;

    std::size_t count = 0;
    for (const auto& _ : container) {
        (void)_;
        count++;
    }

    if (count == 0) throw std::runtime_error("empty");

    auto total = sum(container);

    if constexpr (std::is_integral_v<T>)
        return static_cast<double>(total) / count;
    else
        return total / count;
}

// =========================
// VARIANCE
// =========================
template <Iterable C>
requires Addable<typename C::value_type> &&
         NumericLike<typename C::value_type>
auto variance(const C& container) {
    using T = typename C::value_type;

    std::size_t count = 0;
    for (const auto& _ : container) {
        (void)_;
        count++;
    }

    if (count == 0) throw std::runtime_error("empty");

    auto m = mean(container);

    if constexpr (std::is_integral_v<T>) {
        double acc = 0.0;
        for (const auto& v : container) {
            double d = v - m;
            acc += d * d;
        }
        return acc / count;
    } else {
        T acc{};
        for (const auto& v : container) {
            T d = v - m;
            acc = acc + (d * d);
        }
        return acc / count;
    }
}

// =========================
// MAX
// =========================
template <Iterable C>
requires NumericLike<typename C::value_type>
auto max(const C& container) {
    auto it = std::begin(container);
    if (it == std::end(container))
        throw std::runtime_error("empty");

    auto best = *it++;
    for (; it != std::end(container); ++it)
        if (best < *it) best = *it;

    return best;
}

// =========================
// TRANSFORM REDUCE
// =========================
template <Iterable C, typename Func>
auto transform_reduce(const C& container, Func f) {
    using R = decltype(f(*std::begin(container)));
    R result{};
    for (const auto& v : container)
        result = result + f(v);
    return result;
}

// =========================
// VARIADIC
// =========================
template <typename... Args>
requires NumericLike<std::common_type_t<Args...>>
auto sum_variadic(Args... args) {
    using T = std::common_type_t<Args...>;
    return (T{} + ... + static_cast<T>(args));
}

template <typename... Args>
requires NumericLike<std::common_type_t<Args...>>
auto mean_variadic(Args... args) {
    using T = std::common_type_t<Args...>;
    T total = (T{} + ... + static_cast<T>(args));

    if constexpr (std::is_integral_v<T>)
        return static_cast<double>(total) / sizeof...(Args);
    else
        return total / sizeof...(Args);
}

template <typename... Args>
requires NumericLike<std::common_type_t<Args...>>
auto variance_variadic(Args... args) {
    using T = std::common_type_t<Args...>;
    auto m = mean_variadic(args...);

    if constexpr (std::is_integral_v<T>) {
        double acc = ( ((static_cast<double>(args) - m) * (static_cast<double>(args) - m)) + ... );
        return acc / sizeof...(Args);
    } else {
        T acc = ( ((static_cast<T>(args) - m) * (static_cast<T>(args) - m)) + ... );
        return acc / sizeof...(Args);
    }
}

template <typename First, typename... Rest>
requires NumericLike<std::common_type_t<First, Rest...>>
auto max_variadic(First first, Rest... rest) {
    using T = std::common_type_t<First, Rest...>;
    T result = static_cast<T>(first);
    ((result = (result < static_cast<T>(rest) ? static_cast<T>(rest) : result)), ...);
    return result;
}

}

#endif
