
/**

                        /~` _  _ _|_. _     _ |_ | _
                        \_,(_)| | | || ||_|(_||_)|(/_

                    https://github.com/Naios/continuable
                                   v1.1.0

  Copyright(c) 2015 - 2017 Denis Blank <denis.blank at outlook dot com>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files(the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions :

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
**/

#ifndef CONTINUABLE_BASE_HPP_INCLUDED__
#define CONTINUABLE_BASE_HPP_INCLUDED__

#include <atomic>
#include <cassert>
#include <cstdint>
#include <future>
#include <memory>
#include <mutex>
#include <tuple>
#include <type_traits>
#include <utility>

/// Declares the continuable library namespace.
///
/// The most important class is cti::continuable_base, that provides the
/// whole functionality for continuation chaining.
///
/// The class cti::continuable_base is created through the
/// cti::make_continuable() function which accepts a callback taking function.
///
/// Also there are following support functions available:
/// - cti::when_all() - connects cti::continuable_base's to an `all` connection.
/// - cti::when_any() - connects cti::continuable_base's to an `any` connection.
/// - cti::when_seq() - connects cti::continuable_base's to a sequence.
///
namespace cti {
/// \cond false
inline namespace abi_v1 {
/// \endcond

/// The main class of the continuable library, it provides the functionality
/// for chaining callbacks and continuations together to a unified hierarchy.
///
/// The most important method is the cti::continuable_base::then() method,
/// which allows to attach a callback to the continuable.
///
/// \tparam Data The internal data which is used to store the current
///         continuation and intermediate lazy connection result.
///
/// \tparam Annotation The internal data used to store the current signature
///         hint or strategy used for combining lazy connections.
///
/// \note Nearly all methods of the cti::continuable_base are required to be
///       called as r-value. This is required because the continuable carries
///       variables which are consumed when the object is transformed as part
///       of a method call. You may copy a continuable which underlying
///       storages are copyable to split the call hierarchy into multiple parts.
///
/// \attention The continuable_base objects aren't intended to be stored.
///            If you want to store a continuble_base you should always
///            call the continuable_base::freeze method for disabling the
///            invocation on destruction.
///
/// \since version 1.0.0
template <typename Data, typename Annotation> class continuable_base;

/// Declares the internal private namespace of the continuable library
/// which isn't intended to be used by users of the library.
namespace detail {
/// Utility namespace which provides useful meta-programming support
namespace util {

/// \cond false
#define CTI__FOR_EACH_BOOLEAN_BIN_OP(CTI__OP__)                                \
  CTI__OP__(==)                                                                \
  CTI__OP__(!=) CTI__OP__(<=) CTI__OP__(>=) CTI__OP__(<) CTI__OP__(>)
#define CTI__FOR_EACH_BOOLEAN_UNA_OP(CTI__OP__) CTI__OP__(!)
#define CTI__FOR_EACH_INTEGRAL_BIN_OP(CTI__OP__)                               \
  CTI__OP__(*)                                                                 \
  CTI__OP__(/) CTI__OP__(+) CTI__OP__(-) CTI__FOR_EACH_BOOLEAN_BIN_OP(CTI__OP__)
#define CTI__FOR_EACH_INTEGRAL_UNA_OP(CTI__OP__)                               \
  CTI__OP__(~) CTI__FOR_EACH_BOOLEAN_UNA_OP(CTI__OP__)
/// \endcond

template <typename T, T Value>
struct constant : std::integral_constant<T, Value> {
/// \cond false
#define CTI__INST(CTI__OP)                                                     \
  template <typename OT, OT OValue>                                            \
  /*constexpr*/ auto operator CTI__OP(std::integral_constant<OT, OValue>)      \
      const noexcept {                                                         \
    return constant<decltype((Value CTI__OP OValue)),                          \
                    (Value CTI__OP OValue)>{};                                 \
  }
  CTI__FOR_EACH_INTEGRAL_BIN_OP(CTI__INST)
#undef CTI__INST
#define CTI__INST(CTI__OP)                                                     \
  /*constexpr*/ auto operator CTI__OP() const noexcept {                       \
    return constant<decltype((CTI__OP Value)), (CTI__OP Value)>{};             \
  }
  CTI__FOR_EACH_INTEGRAL_UNA_OP(CTI__INST)
#undef CTI__INST
  /// \endcond
};

template <bool Value>
struct constant<bool, Value> : std::integral_constant<bool, Value> {
/// \cond false
#define CTI__INST(CTI__OP)                                                     \
  template <typename OT, OT OValue>                                            \
  /*constexpr*/ auto operator CTI__OP(std::integral_constant<bool, OValue>)    \
      const noexcept {                                                         \
    return constant<bool, (Value CTI__OP OValue)>{};                           \
  }
  CTI__FOR_EACH_BOOLEAN_BIN_OP(CTI__INST)
#undef CTI__INST
#define CTI__INST(CTI__OP)                                                     \
  /*constexpr*/ auto operator CTI__OP() const noexcept {                       \
    return constant<bool, CTI__OP Value>{};                                    \
  }
  CTI__FOR_EACH_BOOLEAN_UNA_OP(CTI__INST)
#undef CTI__INST
  /// \endcond
};

template <bool Value> using bool_constant = constant<bool, Value>;
template <std::size_t Value> using size_constant = constant<std::size_t, Value>;

template <typename T, bool Value>
constexpr auto constant_of(std::integral_constant<T, Value> /*value*/ = {}) {
  return constant<T, Value>{};
}
template <std::size_t Value>
constexpr auto
size_constant_of(std::integral_constant<std::size_t, Value> /*value*/ = {}) {
  return size_constant<Value>{};
}
template <bool Value>
constexpr auto
bool_constant_of(std::integral_constant<bool, Value> /*value*/ = {}) {
  return bool_constant<Value>{};
}

#undef CTI__FOR_EACH_BOOLEAN_BIN_OP
#undef CTI__FOR_EACH_BOOLEAN_UNA_OP
#undef CTI__FOR_EACH_INTEGRAL_BIN_OP
#undef CTI__FOR_EACH_INTEGRAL_UNA_OP

/// Evaluates to the element at position I.
template <std::size_t I, typename... Args>
using at_t = decltype(std::get<I>(std::declval<std::tuple<Args...>>()));

/// Evaluates to an integral constant which represents the size
/// of the given pack.
template <typename... Args> using size_of_t = size_constant<sizeof...(Args)>;

/// A tagging type for wrapping other types
template <typename... T> struct identity {};
template <typename T> struct identity<T> : std::common_type<T> {};

template <typename> struct is_identity : std::false_type {};
template <typename... Args>
struct is_identity<identity<Args...>> : std::true_type {};

template <typename T>
identity<std::decay_t<T>> constexpr identity_of(T const& /*type*/) noexcept {
  return {};
}
template <typename... Args>
constexpr identity<Args...> identity_of(identity<Args...> /*type*/) noexcept {
  return {};
}
template <typename T> constexpr auto identity_of() noexcept {
  return std::conditional_t<is_identity<std::decay_t<T>>::value, T,
                            identity<std::decay_t<T>>>{};
}

template <std::size_t I, typename... T>
constexpr auto get(identity<T...>) noexcept {
  return identity_of<at_t<I, T...>>();
}

/// Helper to trick compilers about that a parameter pack is used
template <typename... T> void unused(T&&... args) {
  auto use = [](auto&& type) mutable {
    (void)type;
    return 0;
  };
  auto deduce = {0, use(std::forward<decltype(args)>(args))...};
  (void)deduce;
  (void)use;
}

namespace detail {
// Equivalent to C++17's std::void_t which targets a bug in GCC,
// that prevents correct SFINAE behavior.
// See http://stackoverflow.com/questions/35753920 for details.
template <typename...> struct deduce_to_void : std::common_type<void> {};
} // end namespace detail

/// C++17 like void_t type
template <typename... T>
using void_t = typename detail::deduce_to_void<T...>::type;

namespace detail {
template <typename T, typename Check, typename = void_t<>>
struct is_valid_impl : std::common_type<std::false_type> {};

template <typename T, typename Check>
struct is_valid_impl<T, Check,
                     void_t<decltype(std::declval<Check>()(std::declval<T>()))>>
    : std::common_type<std::true_type> {};

template <typename Type, typename TrueCallback>
constexpr void static_if_impl(std::true_type, Type&& type,
                              TrueCallback&& trueCallback) {
  std::forward<TrueCallback>(trueCallback)(std::forward<Type>(type));
}

template <typename Type, typename TrueCallback>
constexpr void static_if_impl(std::false_type, Type&& /*type*/,
                              TrueCallback&& /*trueCallback*/) {}

template <typename Type, typename TrueCallback, typename FalseCallback>
constexpr auto static_if_impl(std::true_type, Type&& type,
                              TrueCallback&& trueCallback,
                              FalseCallback&& /*falseCallback*/) {
  return std::forward<TrueCallback>(trueCallback)(std::forward<Type>(type));
}

template <typename Type, typename TrueCallback, typename FalseCallback>
constexpr auto static_if_impl(std::false_type, Type&& type,
                              TrueCallback&& /*trueCallback*/,
                              FalseCallback&& falseCallback) {
  return std::forward<FalseCallback>(falseCallback)(std::forward<Type>(type));
}
} // end namespace detail

/// Returns the pack size of the given type
template <typename... Args>
constexpr auto pack_size_of(identity<std::tuple<Args...>>) noexcept {
  return size_of_t<Args...>{};
}
/// Returns the pack size of the given type
template <typename First, typename Second>
constexpr auto pack_size_of(identity<std::pair<First, Second>>) noexcept {
  return size_of_t<First, Second>{};
}
/// Returns the pack size of the given type
template <typename... Args>
constexpr auto pack_size_of(identity<Args...>) noexcept {
  return size_of_t<Args...>{};
}

/// Returns an index sequence of the given type
template <typename T> constexpr auto sequenceOf(T&& /*sequenceable*/) noexcept {
  return std::make_index_sequence<decltype(
      pack_size_of(std::declval<T>()))::value>();
}

/// Returns a check which returns a true type if the current value
/// is below the
template <std::size_t End>
constexpr auto isLessThen(size_constant<End> end) noexcept {
  return [=](auto current) { return end > current; };
}

/// Compile-time check for validating a certain expression
template <typename T, typename Check>
constexpr auto is_valid(T&& /*type*/, Check&& /*check*/) noexcept {
  return typename detail::is_valid_impl<T, Check>::type{};
}

/// Creates a static functional validator object.
template <typename Check>
constexpr auto validatorOf(Check&& check) noexcept(
    std::is_nothrow_move_constructible<std::decay_t<Check>>::value) {
  return [check = std::forward<Check>(check)](auto&& matchable) {
    return is_valid(std::forward<decltype(matchable)>(matchable), check);
  };
}

/// Invokes the callback only if the given type matches the check
template <typename Type, typename Check, typename TrueCallback>
constexpr void static_if(Type&& type, Check&& check,
                         TrueCallback&& trueCallback) {
  detail::static_if_impl(std::forward<Check>(check)(type),
                         std::forward<Type>(type),
                         std::forward<TrueCallback>(trueCallback));
}

/// Invokes the callback only if the given type matches the check
template <typename Type, typename Check, typename TrueCallback,
          typename FalseCallback>
constexpr auto static_if(Type&& type, Check&& check,
                         TrueCallback&& trueCallback,
                         FalseCallback&& falseCallback) {
  return detail::static_if_impl(std::forward<Check>(check)(type),
                                std::forward<Type>(type),
                                std::forward<TrueCallback>(trueCallback),
                                std::forward<FalseCallback>(falseCallback));
}

/// A compile-time while loop, which loops as long the value matches
/// the predicate. The handler shall return the next value.
template <typename Value, typename Predicate, typename Handler>
constexpr auto static_while(Value&& value, Predicate&& predicate,
                            Handler&& handler) {
  return static_if(std::forward<Value>(value), predicate,
                   [&](auto&& result) mutable {
                     return static_while(
                         handler(std::forward<decltype(result)>(result)),
                         std::forward<Predicate>(predicate),
                         std::forward<Handler>(handler));
                   },
                   [&](auto&& result) mutable {
                     return std::forward<decltype(result)>(result);
                   });
}

/// Returns a validator which checks whether the given sequenceable is empty
inline auto is_empty() noexcept {
  return [](auto const& checkable) {
    return pack_size_of(checkable) == size_constant_of<0>();
  };
}

/// Calls the given unpacker with the content of the given sequence
template <typename U, std::size_t... I>
constexpr auto unpack(std::integer_sequence<std::size_t, I...>, U&& unpacker) {
  return std::forward<U>(unpacker)(size_constant_of<I>()...);
}

/// Calls the given unpacker with the content of the given sequenceable
template <typename F, typename U, std::size_t... I>
constexpr auto unpack(F&& firstSequenceable, U&& unpacker,
                      std::integer_sequence<std::size_t, I...>) {
  using std::get;
  (void)firstSequenceable;
  return std::forward<U>(unpacker)(
      get<I>(std::forward<F>(firstSequenceable))...);
}
/// Calls the given unpacker with the content of the given sequenceable
template <typename F, typename S, typename U, std::size_t... IF,
          std::size_t... IS>
constexpr auto unpack(F&& firstSequenceable, S&& secondSequenceable,
                      U&& unpacker, std::integer_sequence<std::size_t, IF...>,
                      std::integer_sequence<std::size_t, IS...>) {
  using std::get;
  (void)firstSequenceable;
  (void)secondSequenceable;
  return std::forward<U>(unpacker)(
      get<IF>(std::forward<F>(firstSequenceable))...,
      get<IS>(std::forward<S>(secondSequenceable))...);
}
/// Calls the given unpacker with the content of the given sequenceable
template <typename F, typename U>
auto unpack(F&& firstSequenceable, U&& unpacker) {
  return unpack(std::forward<F>(firstSequenceable), std::forward<U>(unpacker),
                sequenceOf(identity_of(firstSequenceable)));
}
/// Calls the given unpacker with the content of the given sequenceables
template <typename F, typename S, typename U>
constexpr auto unpack(F&& firstSequenceable, S&& secondSequenceable,
                      U&& unpacker) {
  return unpack(std::forward<F>(firstSequenceable),
                std::forward<S>(secondSequenceable), std::forward<U>(unpacker),
                sequenceOf(identity_of(firstSequenceable)),
                sequenceOf(identity_of(secondSequenceable)));
}

/// Applies the handler function to each element contained in the sequenceable
template <typename Sequenceable, typename Handler>
constexpr void static_for_each_in(Sequenceable&& sequenceable,
                                  Handler&& handler) {
  unpack(
      std::forward<Sequenceable>(sequenceable), [&](auto&&... entries) mutable {
        auto consume = [&](auto&& entry) mutable {
          handler(std::forward<decltype(entry)>(entry));
          return 0;
        };
        // Apply the consume function to every entry inside the pack
        auto deduce = {0, consume(std::forward<decltype(entries)>(entries))...};
        (void)deduce;
        (void)consume;
      });
}

/// Adds the given type at the back of the left sequenceable
template <typename Left, typename Element>
constexpr auto push(Left&& left, Element&& element) {
  return unpack(std::forward<Left>(left), [&](auto&&... leftArgs) {
    return std::make_tuple(std::forward<decltype(leftArgs)>(leftArgs)...,
                           std::forward<Element>(element));
  });
}

/// Adds the element to the back of the identity
template <typename... Args, typename Element>
constexpr auto push(identity<Args...>, identity<Element>) noexcept {
  return identity<Args..., Element>{};
}

/// Removes the first element from the identity
template <typename First, typename... Rest>
constexpr auto pop_first(identity<First, Rest...>) noexcept {
  return identity<Rest...>{};
}

/// Returns the merged sequence
template <typename Left> constexpr auto merge(Left&& left) {
  return std::forward<Left>(left);
}
/// Merges the left sequenceable with the right ones
template <typename Left, typename Right, typename... Rest>
constexpr auto merge(Left&& left, Right&& right, Rest&&... rest) {
  // Merge the left with the right sequenceable and
  // merge the result with the rest.
  return merge(unpack(std::forward<Left>(left), std::forward<Right>(right),
                      [&](auto&&... args) {
                        // Maybe use: template <template<typename...> class T,
                        // typename... Args>
                        return std::make_tuple(
                            std::forward<decltype(args)>(args)...);
                      }),
               std::forward<Rest>(rest)...);
}
/// Merges the left identity with the right ones
template <typename... LeftArgs, typename... RightArgs, typename... Rest>
constexpr auto merge(identity<LeftArgs...> /*left*/,
                     identity<RightArgs...> /*right*/, Rest&&... rest) {
  return merge(identity<LeftArgs..., RightArgs...>{},
               std::forward<Rest>(rest)...);
}

/// Combines the given arguments with the given folding function
template <typename F, typename First>
constexpr auto fold(F&& /*folder*/, First&& first) {
  return std::forward<First>(first);
}
/// Combines the given arguments with the given folding function
template <typename F, typename First, typename Second, typename... Rest>
auto fold(F&& folder, First&& first, Second&& second, Rest&&... rest) {
  auto res = folder(std::forward<First>(first), std::forward<Second>(second));
  return fold(std::forward<F>(folder), std::move(res),
              std::forward<Rest>(rest)...);
}

/// Returns a folding function using operator `&&`.
inline auto and_folding() noexcept {
  return [](auto&& left, auto&& right) {
    return std::forward<decltype(left)>(left) &&
           std::forward<decltype(right)>(right);
  };
}
/// Returns a folding function using operator `||`.
inline auto or_folding() noexcept {
  return [](auto&& left, auto&& right) {
    return std::forward<decltype(left)>(left) ||
           std::forward<decltype(right)>(right);
  };
}
/// Returns a folding function using operator `>>`.
inline auto seq_folding() noexcept {
  return [](auto&& left, auto&& right) {
    return std::forward<decltype(left)>(left) >>
           std::forward<decltype(right)>(right);
  };
}

/// Deduces to a std::false_type
template <typename T>
using fail = std::integral_constant<bool, !std::is_same<T, T>::value>;

namespace detail {
template <typename T, typename Args, typename = void_t<>>
struct is_invokable_impl : std::common_type<std::false_type> {};

template <typename T, typename... Args>
struct is_invokable_impl<
    T, std::tuple<Args...>,
    void_t<decltype(std::declval<T>()(std::declval<Args>()...))>>
    : std::common_type<std::true_type> {};
} // end namespace detail

/// Deduces to a std::true_type if the given type is callable with the arguments
/// inside the given tuple.
/// The main reason for implementing it with the detection idiom instead of
/// hana like detection is that MSVC has issues with capturing raw template
/// arguments inside lambda closures.
///
/// ```cpp
/// util::is_invokable_t<object, std::tuple<Args...>>
/// ```
template <typename T, typename Args>
using is_invokable_t = typename detail::is_invokable_impl<T, Args>::type;

namespace detail {
/// Forwards every element in the tuple except the last one
template <typename T> auto forward_except_last(T&& sequenceable) {
  auto size = pack_size_of(identity_of(sequenceable)) - size_constant_of<1>();
  auto sequence = std::make_index_sequence<size.value>();

  return unpack(std::forward<T>(sequenceable),
                [](auto&&... args) {
                  return std::forward_as_tuple(
                      std::forward<decltype(args)>(args)...);
                },
                sequence);
}

/// We are able to call the callable with the arguments given in the tuple
template <typename T, typename... Args>
auto partial_invoke_impl(std::true_type, T&& callable,
                         std::tuple<Args...> args) {
  return unpack(std::move(args), [&](auto&&... arg) {
    return std::forward<T>(callable)(std::forward<decltype(arg)>(arg)...);
  });
}

/// We were unable to call the callable with the arguments in the tuple.
/// Remove the last argument from the tuple and try it again.
template <typename T, typename... Args>
auto partial_invoke_impl(std::false_type, T&& callable,
                         std::tuple<Args...> args) {

  // If you are encountering this assertion you tried to attach a callback
  // which can't accept the arguments of the continuation.
  //
  // ```cpp
  // continuable<int, int> c;
  // std::move(c).then([](std::vector<int> v) { /*...*/ })
  // ```
  static_assert(
      sizeof...(Args) > 0,
      "There is no way to call the given object with these arguments!");

  // Remove the last argument from the tuple
  auto next = forward_except_last(std::move(args));

  // Test whether we are able to call the function with the given tuple
  is_invokable_t<decltype(callable), decltype(next)> is_invokable;

  return partial_invoke_impl(is_invokable, std::forward<T>(callable),
                             std::move(next));
}

/// Shortcut - we can call the callable directly
template <typename T, typename... Args>
auto partial_invoke_impl_shortcut(std::true_type, T&& callable,
                                  Args&&... args) {
  return std::forward<T>(callable)(std::forward<Args>(args)...);
}

/// Failed shortcut - we were unable to invoke the callable with the
/// original arguments.
template <typename T, typename... Args>
auto partial_invoke_impl_shortcut(std::false_type failed, T&& callable,
                                  Args&&... args) {

  // Our shortcut failed, convert the arguments into a forwarding tuple
  return partial_invoke_impl(
      failed, std::forward<T>(callable),
      std::forward_as_tuple(std::forward<Args>(args)...));
}
} // end namespace detail

/// Partially invokes the given callable with the given arguments.
///
/// \note This function will assert statically if there is no way to call the
///       given object with less arguments.
template <typename T, typename... Args>
auto partial_invoke(T&& callable, Args&&... args) {
  // Test whether we are able to call the function with the given arguments.
  is_invokable_t<decltype(callable), std::tuple<Args...>> is_invokable;

  // The implementation is done in a shortcut way so there are less
  // type instantiations needed to call the callable with its full signature.
  return detail::partial_invoke_impl_shortcut(
      is_invokable, std::forward<T>(callable), std::forward<Args>(args)...);
}

// Class for making child classes non copyable
struct non_copyable {
  constexpr non_copyable() = default;
  non_copyable(non_copyable const&) = delete;
  constexpr non_copyable(non_copyable&&) = default;
  non_copyable& operator=(non_copyable const&) = delete;
  non_copyable& operator=(non_copyable&&) = default;
};

// Class for making child classes non copyable and movable
struct non_movable {
  constexpr non_movable() = default;
  non_movable(non_movable const&) = delete;
  constexpr non_movable(non_movable&&) = delete;
  non_movable& operator=(non_movable const&) = delete;
  non_movable& operator=(non_movable&&) = delete;
};
} // end namespace util

/// This class is responsible for holding an abstract copy- and
/// move-able ownership that is invalidated when the object
/// is moved to another instance.
class ownership {
  explicit constexpr ownership(bool acquired, bool frozen)
      : acquired_(acquired), frozen_(frozen) {}

public:
  constexpr ownership() : acquired_(true), frozen_(false) {}
  constexpr ownership(ownership const&) = default;
  ownership(ownership&& right) noexcept
      : acquired_(right.consume()), frozen_(right.is_frozen()) {}
  ownership& operator=(ownership const&) = default;
  ownership& operator=(ownership&& right) noexcept {
    acquired_ = right.consume();
    frozen_ = right.is_frozen();
    return *this;
  }

  // Merges both ownerships together
  ownership operator|(ownership const& right) const noexcept {
    return ownership(is_acquired() && right.is_acquired(),
                     is_frozen() || right.is_frozen());
  }

  constexpr bool is_acquired() const noexcept { return acquired_; }
  constexpr bool is_frozen() const noexcept { return frozen_; }

  void release() noexcept {
    assert(is_acquired() && "Tried to release the ownership twice!");
    acquired_ = false;
  }
  void freeze(bool enabled = true) noexcept {
    assert(is_acquired() && "Tried to freeze a released object!");
    frozen_ = enabled;
  }

private:
  bool consume() noexcept {
    if (is_acquired()) {
      release();
      return true;
    }
    return false;
  }

  /// Is true when the object is in a valid state
  bool acquired_ : 1;
  /// Is true when the automatic invocation on destruction is disabled
  bool frozen_ : 1;
};

/// Represents a present signature hint
template <typename... Args> using signature_hint_tag = util::identity<Args...>;
/// Represents an absent signature hint
struct absent_signature_hint_tag {};

template <typename> struct is_absent_hint : std::false_type {};
template <>
struct is_absent_hint<absent_signature_hint_tag> : std::true_type {};

struct this_thread_executor_tag {};

/// The namespace `base` provides the low level API for working
/// with continuable types.
///
/// Important methods are:
/// - Creating a continuation from a callback taking functional
///   base::attorney::create(auto&& callback)
///     -> base::continuation<auto>
/// - Chaining a continuation together with a callback
///   base::chain_continuation(base::continuation<auto> continuation,
///                            auto&& callback)
///     -> base::continuation<auto>
/// - Finally invoking the continuation chain
///    base::finalize_continuation(base::continuation<auto> continuation)
///     -> void
namespace base {
/// Returns the signature hint of the given continuable
template <typename T> constexpr auto hint_of(util::identity<T>) {
  static_assert(util::fail<T>::value,
                "Expected a continuation with an existing signature hint!");
  return util::identity_of<void>();
}
/// Returns the signature hint of the given continuable
template <typename Data, typename... Args>
constexpr auto
hint_of(util::identity<continuable_base<Data, signature_hint_tag<Args...>>>) {
  return signature_hint_tag<Args...>{};
}

template <typename T> struct is_continuation : std::false_type {};
template <typename Data, typename Annotation>
struct is_continuation<continuable_base<Data, Annotation>> : std::true_type {};

/// Helper class to access private methods and members of
/// the continuable_base class.
struct attorney {
  /// Makes a continuation wrapper from the given argument
  template <typename T, typename A = absent_signature_hint_tag>
  static auto create(T&& continuation, A /*hint*/, ownership ownership_) {
    return continuable_base<std::decay_t<T>, std::decay_t<A>>(
        std::forward<T>(continuation), ownership_);
  }

  /// Invokes a continuation object in a reference correct way
  template <typename Data, typename Annotation, typename Callback>
  static auto
  invoke_continuation(continuable_base<Data, Annotation>&& continuation,
                      Callback&& callback) {
    auto materialized = std::move(continuation).materialize();
    materialized.release();
    return materialized.data_(std::forward<Callback>(callback));
  }

  template <typename Data, typename Annotation>
  static auto materialize(continuable_base<Data, Annotation>&& continuation) {
    return std::move(continuation).materialize();
  }

  template <typename Data, typename Annotation>
  static Data&&
  consume_data(continuable_base<Data, Annotation>&& continuation) {
    return std::move(continuation).consume_data();
  }

  template <typename Continuable>
  static ownership ownership_of(Continuable&& continuation) {
    return continuation.ownership_;
  }
};

// Returns the invoker of a callback, the next callback
// and the arguments of the previous continuation.
//
// The return type of the invokerOf function matches a functional of:
//   void(auto&& callback, auto&& nextCallback, auto&&... args)
//
// The invoker decorates the result type in the following way
// - void              -> nextCallback()
// - ?                 -> nextCallback(?)
// - std::pair<?, ?>   -> nextCallback(?, ?)
// - std::tuple<?...>  -> nextCallback(?...)
//
// When the result is a continuation itself pass the callback to it
// - continuation<?...> -> result(nextCallback);
namespace decoration {
/// Helper class wrapping the underlaying unwrapping lambda
/// in order to extend it with a hint method.
template <typename T, typename Hint> class invoker : public T {
public:
  explicit invoker(T invoke) : T(std::move(invoke)) {}

  using T::operator();

  /// Returns the underlaying signature hint
  static constexpr Hint hint() noexcept { return {}; }
};

template <typename T, typename... Args>
constexpr auto make_invoker(T&& invoke, signature_hint_tag<Args...>) {
  return invoker<std::decay_t<T>, signature_hint_tag<Args...>>(
      std::forward<T>(invoke));
}

/// - continuable<?...> -> result(nextCallback);
template <typename Data, typename Annotation>
constexpr auto invoker_of(util::identity<continuable_base<Data, Annotation>>) {
  /// Get the hint of the unwrapped returned continuable
  using Type = decltype(attorney::materialize(
      std::declval<continuable_base<Data, Annotation>>()));

  return make_invoker(
      [](auto&& callback, auto&& nextCallback, auto&&... args) {
        auto continuation_ = std::forward<decltype(callback)>(callback)(
            std::forward<decltype(args)>(args)...);

        attorney::invoke_continuation(
            std::move(continuation_),
            std::forward<decltype(nextCallback)>(nextCallback));
      },
      hint_of(util::identity_of<Type>()));
}

/// - ? -> nextCallback(?)
template <typename T> auto invoker_of(util::identity<T>) {
  return make_invoker(
      [](auto&& callback, auto&& nextCallback, auto&&... args) {
        auto result = std::forward<decltype(callback)>(callback)(
            std::forward<decltype(args)>(args)...);

        std::forward<decltype(nextCallback)>(nextCallback)(std::move(result));
      },
      util::identity_of<T>());
}

/// - void -> nextCallback()
inline auto invoker_of(util::identity<void>) {
  return make_invoker(
      [](auto&& callback, auto&& nextCallback, auto&&... args) {
        std::forward<decltype(callback)>(callback)(
            std::forward<decltype(args)>(args)...);

        std::forward<decltype(nextCallback)>(nextCallback)();
      },
      util::identity<>{});
}

/// Returns a sequenced invoker which is able to invoke
/// objects where std::get is applicable.
inline auto sequenced_unpack_invoker() {
  return [](auto&& callback, auto&& nextCallback, auto&&... args) {
    auto result = std::forward<decltype(callback)>(callback)(
        std::forward<decltype(args)>(args)...);

    util::unpack(std::move(result), [&](auto&&... types) {
      /// TODO Add inplace resolution here

      std::forward<decltype(nextCallback)>(nextCallback)(
          std::forward<decltype(types)>(types)...);
    });
  };
}

// - std::pair<?, ?> -> nextCallback(?, ?)
template <typename First, typename Second>
constexpr auto invoker_of(util::identity<std::pair<First, Second>>) {
  return make_invoker(sequenced_unpack_invoker(),
                      util::identity<First, Second>{});
}

// - std::tuple<?...>  -> nextCallback(?...)
template <typename... Args>
constexpr auto invoker_of(util::identity<std::tuple<Args...>>) {
  return make_invoker(sequenced_unpack_invoker(), util::identity<Args...>{});
}
} // end namespace decoration

/// Invoke the callback immediately
template <typename Invoker, typename Callback, typename NextCallback,
          typename... Args>
void packed_dispatch(this_thread_executor_tag, Invoker&& invoker,
                     Callback&& callback, NextCallback&& nextCallback,
                     Args&&... args) {

  // Invoke the callback with the decorated invoker immediately
  std::forward<Invoker>(invoker)(std::forward<Callback>(callback),
                                 std::forward<NextCallback>(nextCallback),
                                 std::forward<Args>(args)...);
}

/// Invoke the callback through the given executor
template <typename Executor, typename Invoker, typename Callback,
          typename NextCallback, typename... Args>
void packed_dispatch(Executor&& executor, Invoker&& invoker,
                     Callback&& callback, NextCallback&& nextCallback,
                     Args&&... args) {

  // Create a worker object which when invoked calls the callback with the
  // the returned arguments.
  auto work = [
    invoker = std::forward<Invoker>(invoker),
    callback = std::forward<Callback>(callback),
    nextCallback = std::forward<NextCallback>(nextCallback),
    args = std::make_tuple(std::forward<Args>(args)...)
  ]() mutable {
    util::unpack(std::move(args), [&](auto&&... captured_args) {
      // Just use the packed dispatch method which dispatches the work on
      // the current thread.
      packed_dispatch(this_thread_executor_tag{}, std::move(invoker),
                      std::move(callback), std::move(nextCallback),
                      std::forward<decltype(captured_args)>(captured_args)...);
    });
  };

  // Pass the work functional object to the executor
  std::forward<Executor>(executor)(std::move(work));
}

/// Invokes a continuation with a given callback.
/// Passes the next callback to the resulting continuable or
/// invokes the next callback directly if possible.
///
/// For example given:
/// - Continuation: continuation<[](auto&& callback) { callback("hi"); }>
/// - Callback: [](std::string) { }
/// - NextCallback: []() { }
///
template <typename... Args, typename Continuation, typename Callback,
          typename Executor, typename NextCallback>
void invoke_proxy(signature_hint_tag<Args...>, Continuation&& continuation,
                  Callback&& callback, Executor&& executor,
                  NextCallback&& nextCallback) {

  // Invoke the continuation with a proxy callback.
  // The proxy callback is responsible for passing the
  // the result to the callback as well as decorating it.
  attorney::invoke_continuation(std::forward<Continuation>(continuation), [
    callback = std::forward<Callback>(callback),
    executor = std::forward<Executor>(executor),
    nextCallback = std::forward<NextCallback>(nextCallback)
  ](Args... args) mutable {

    // In order to retrieve the correct decorator we must know what the
    // result type is.
    auto result =
        util::identity_of<decltype(std::move(callback)(std::move(args)...))>();

    // Pick the correct invoker that handles decorating of the result
    auto invoker = decoration::invoker_of(result);

    // Invoke the callback
    packed_dispatch(std::move(executor), std::move(invoker),
                    std::move(callback), std::move(nextCallback),
                    std::move(args)...);
  });
}

/// Returns the next hint when the callback is invoked with the given hint
template <typename T, typename... Args>
constexpr auto next_hint_of(util::identity<T> /*callback*/,
                            signature_hint_tag<Args...> /*current*/) {
  return decoration::invoker_of(util::identity_of<decltype(std::declval<T>()(
                                   std::declval<Args>()...))>())
      .hint();
}

/// Chains a callback together with a continuation and returns a continuation:
///
/// For example given:
/// - Continuation: continuation<[](auto&& callback) { callback("hi"); }>
/// - Callback: [](std::string) { }
///
/// This function returns a function accepting the next callback in the chain:
/// - Result: continuation<[](auto&& callback) { /*...*/ }>
///
template <typename Continuation, typename Callback, typename Executor>
auto chain_continuation(Continuation&& continuation, Callback&& callback,
                        Executor&& executor) {
  static_assert(is_continuation<std::decay_t<Continuation>>{},
                "Expected a continuation!");

  // Wrap the callback into a partial callable callback
  auto partial_callable = [callback = std::forward<Callback>(callback)](
      auto&&... args) mutable {
    return util::partial_invoke(std::move(callback),
                                std::forward<decltype(args)>(args)...);
  };

  auto hint = hint_of(util::identity_of(continuation));
  auto next_hint = next_hint_of(util::identity_of(partial_callable), hint);

  auto ownership_ = attorney::ownership_of(continuation);
  continuation.freeze();

  return attorney::create(
      [
        // TODO consume only the data here
        continuation = std::forward<Continuation>(continuation),
        partial_callable = std::move(partial_callable),
        executor = std::forward<Executor>(executor)
      ](auto&& nextCallback) mutable {
        invoke_proxy(hint_of(util::identity_of(continuation)),
                     std::move(continuation), std::move(partial_callable),
                     std::move(executor),
                     std::forward<decltype(nextCallback)>(nextCallback));
      },
      next_hint, ownership_);
}

/// Workaround for GCC bug:
/// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=64095
struct empty_callback {
  template <typename... Args> void operator()(Args...) const {}
};

/// Final invokes the given continuation chain:
///
/// For example given:
/// - Continuation: continuation<[](auto&& callback) { callback("hi"); }>
template <typename Continuation>
void finalize_continuation(Continuation&& continuation) {
  attorney::invoke_continuation(std::forward<Continuation>(continuation),
                                empty_callback{});
}

/// Workaround for GCC bug:
/// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=64095
template <typename T> class supplier_callback {
  T data_;

public:
  explicit supplier_callback(T data) : data_(std::move(data)) {}

  template <typename... Args> auto operator()(Args...) {
    return std::move(data_);
  }
};

/// Returns a continuable into a functional object returning the continuable
template <typename Continuation>
auto wrap_continuation(Continuation&& continuation) {
  continuation.freeze();
  return supplier_callback<std::decay_t<Continuation>>(
      std::forward<Continuation>(continuation));
}
} // end namespace base

/// The namespace `compose` offers methods to chain continuations together
/// with `all`, `any` or `seq` logic.
namespace compose {
struct strategy_all_tag {};
struct strategy_any_tag {};

template <typename T> struct is_strategy : std::false_type {};
template <> struct is_strategy<strategy_all_tag> : std::true_type {};
template <> struct is_strategy<strategy_any_tag> : std::true_type {};

/// Provides support for extracting the signature hint out
/// of given types and parameters.
namespace annotating {
namespace detail {
/// Void hints are equal to an empty signature
constexpr auto make_hint_of(util::identity<void>) noexcept {
  return signature_hint_tag<>{};
}
/// All other hints are the obvious hints...
template <typename... HintArgs>
constexpr auto make_hint_of(util::identity<HintArgs...> args) noexcept {
  return args; // Identity is equal to signature_hint_tag
}
} // end namespace detail

/// Extracts the signature hint of a given continuation and it's optional
/// present hint arguments.
///
/// There are 3 cases:
/// - Any argument is given:
///   -> The hint is of the argument type where void is equal to no args
/// - An unwrappable type is given which first arguments signature is known
///   -> The hint is of the mentioned signature
/// - An object which signature isn't known
///   -> The hint is unknown
///
/// In any cases the return type is a:
/// - signature_hint_tag<?...> or a
/// - absent_signature_hint_tag
///
template <typename T, typename... HintArgs>
constexpr auto extract(util::identity<T> /*type*/,
                       util::identity<HintArgs...> hint) {
  return util::static_if(hint, util::is_empty(),
                         [=](auto /*hint*/) {
                           /// When the arguments are the hint is absent
                           return absent_signature_hint_tag{};
                         },
                         [](auto hint) {
                           // When hint arguments are given just take it as hint
                           return detail::make_hint_of(hint);
                         });
}
} // end namespace annotating

namespace detail {
template <std::size_t Pos, typename T>
constexpr void assign(util::size_constant<Pos> /*pos*/, T& /*storage*/) {
  // ...
}
template <std::size_t Pos, typename T, typename Current, typename... Args>
void assign(util::size_constant<Pos> pos, T& storage, Current&& current,
            Args&&... args) {
  std::get<Pos>(storage) = std::forward<Current>(current);
  assign(pos + util::size_constant_of<1>(), storage,
         std::forward<Args>(args)...);
}

/// Caches the partial results and invokes the callback when all results
/// are arrived. This class is thread safe.
template <typename T, std::size_t Submissions, typename... Args>
class all_result_submitter : public std::enable_shared_from_this<
                                 all_result_submitter<T, Submissions, Args...>>,
                             public util::non_movable {

  T callback_;
  std::atomic<std::size_t> left_;
  std::tuple<Args...> result_;

public:
  explicit all_result_submitter(T callback)
      : callback_(std::move(callback)), left_(Submissions) {}

  /// Creates a submitter which submits it's result into the tuple
  template <std::size_t From, std::size_t To>
  auto create_callback(util::size_constant<From> from,
                       util::size_constant<To> to) {

    return [ me = this->shared_from_this(), from, to ](auto&&... args) {
      static_assert(sizeof...(args) == (To - From),
                    "Submission called with the wrong amount of arguments!");

      // Assign the values from the result to it's correct positions of the
      // tuple. Maybe think about the thread safety again...:
      // http://stackoverflow.com/questions/40845699
      assign(from, me->result_, std::forward<decltype(args)>(args)...);

      // Complete the current result
      me->complete_one();
    };
  }

private:
  // Invokes the callback with the cached result
  void invoke() {
    assert((left_ == 0U) && "Expected that the submitter is finished!");
    std::atomic_thread_fence(std::memory_order_acquire);
    util::unpack(std::move(result_), [&](auto&&... args) {
      std::move(callback_)(std::forward<decltype(args)>(args)...);
    });
  }
  // Completes one result
  void complete_one() {
    assert((left_ > 0U) && "Expected that the submitter isn't finished!");

    auto current = --left_;
    if (!current) {
      invoke();
    }
  }
};

/// Invokes the callback with the first arriving result
template <typename T>
class any_result_submitter
    : public std::enable_shared_from_this<any_result_submitter<T>>,
      public util::non_movable {

  T callback_;
  std::once_flag flag_;

public:
  explicit any_result_submitter(T callback) : callback_(std::move(callback)) {}

  /// Creates a submitter which submits it's result to the callback
  auto create_callback() {
    return [me = this->shared_from_this()](auto&&... args) {
      me->invoke(std::forward<decltype(args)>(args)...);
    };
  }

private:
  // Invokes the callback with the given arguments
  template <typename... Args> void invoke(Args&&... args) {
    std::call_once(flag_, std::move(callback_), std::forward<Args>(args)...);
  }
};
} // end namespace detail

/// Adds the given continuation tuple to the left composition
template <typename... LeftArgs, typename... RightArgs>
auto chain_composition(std::tuple<LeftArgs...> leftPack,
                       std::tuple<RightArgs...> rightPack) {

  return util::merge(std::move(leftPack), std::move(rightPack));
}

/// Normalizes a continuation to a tuple holding an arbitrary count of
/// continuations matching the given strategy.
///
/// Basically we can encounter 3 cases:
/// - The continuable isn't in any strategy:
///   -> make a tuple containing the continuable as only element
template <typename Strategy, typename Data, typename Annotation,
          std::enable_if_t<!is_strategy<Annotation>::value>* = nullptr>
auto normalize(Strategy /*strategy*/,
               continuable_base<Data, Annotation>&& continuation) {

  // If the continuation isn't a strategy initialize the strategy
  return std::make_tuple(std::move(continuation));
}
/// - The continuable is in a different strategy then the current one:
///   -> materialize it
template <typename Strategy, typename Data, typename Annotation,
          std::enable_if_t<is_strategy<Annotation>::value>* = nullptr>
auto normalize(Strategy /*strategy*/,
               continuable_base<Data, Annotation>&& continuation) {

  // If the right continuation is a different strategy materialize it
  // in order to keep the precedence in cases where: `c1 && (c2 || c3)`.
  return std::make_tuple(base::attorney::materialize(std::move(continuation)));
}
/// - The continuable is inside the current strategy state:
///   -> return the data of the tuple
template <typename Strategy, typename Data>
auto normalize(Strategy /*strategy*/,
               continuable_base<Data, Strategy>&& continuation) {

  // If we are in the given strategy we can just use the data of the continuable
  return base::attorney::consume_data(std::move(continuation));
}

/// Entry function for connecting two continuables with a given strategy.
template <typename Strategy, typename LData, typename LAnnotation,
          typename RData, typename RAnnotation>
auto connect(Strategy strategy, continuable_base<LData, LAnnotation>&& left,
             continuable_base<RData, RAnnotation>&& right) {

  auto ownership_ =
      base::attorney::ownership_of(left) | base::attorney::ownership_of(right);

  left.freeze();
  right.freeze();

  // Make the new data which consists of a tuple containing
  // all connected continuables.
  auto data = chain_composition(normalize(strategy, std::move(left)),
                                normalize(strategy, std::move(right)));

  // Return a new continuable containing the tuple and holding
  // the current strategy as annotation.
  return base::attorney::create(std::move(data), strategy, ownership_);
}

/// Creates a submitter which caches the intermediate results of `all` chains
template <typename Callback, std::size_t Submissions, typename... Args>
auto make_all_result_submitter(Callback&& callback,
                               util::size_constant<Submissions>,
                               util::identity<Args...>) {
  return std::make_shared<detail::all_result_submitter<
      std::decay_t<decltype(callback)>, Submissions, Args...>>(
      std::forward<decltype(callback)>(callback));
}

/// Finalizes the all logic of a given composition
template <typename Data>
auto finalize_composition(
    continuable_base<Data, strategy_all_tag>&& continuation) {

  auto ownership_ = base::attorney::ownership_of(continuation);

  auto composition = base::attorney::consume_data(std::move(continuation));

  // Merge all signature hints together
  auto signature = util::unpack(composition, [](auto&... entries) {
    return util::merge(base::hint_of(util::identity_of(entries))...);
  });

  return base::attorney::create(
      [ signature,
        composition = std::move(composition) ](auto&& callback) mutable {
        // We mark the current 2-dimensional position through a pair:
        // std::pair<size_constant<?>, size_constant<?>>
        //           ~~~~~~~~~~~~~~~~  ~~~~~~~~~~~~~~~~
        //           Continuation pos     Result pos
        auto begin = std::make_pair(util::size_constant_of<0>(),
                                    util::size_constant_of<0>());
        auto pack = util::identity_of(composition);
        auto end = util::pack_size_of(pack);
        auto condition = [=](auto pos) { return pos.first < end; };

        // Create the result submitter which caches all results and invokes
        // the final callback upon completion.
        auto submitter = make_all_result_submitter(
            std::forward<decltype(callback)>(callback), end, signature);

        // Invoke every continuation with it's callback of the submitter
        util::static_while(begin, condition, [&](auto current) mutable {
          auto entry =
              std::move(std::get<decltype(current.first)::value>(composition));

          // This is the length of the arguments of the current continuable
          auto arg_size =
              util::pack_size_of(base::hint_of(util::identity_of(entry)));

          // The next position in the result tuple
          auto next = current.second + arg_size;

          // Invoke the continuation with the associated submission callback
          base::attorney::invoke_continuation(
              std::move(entry),
              submitter->create_callback(current.second, next));

          return std::make_pair(current.first + util::size_constant_of<1>(),
                                next);
        });
      },
      signature, std::move(ownership_));
}

/// Creates a submitter that continues `any` chains
template <typename Callback>
auto make_any_result_submitter(Callback&& callback) {
  return std::make_shared<
      detail::any_result_submitter<std::decay_t<decltype(callback)>>>(
      std::forward<decltype(callback)>(callback));
}

template <typename T, typename... Args>
constexpr T first_of(util::identity<T, Args...>) noexcept;

template <typename Signature, typename... Args>
constexpr auto common_result_of(Signature signature, signature_hint_tag<>,
                                Args... /*args*/) {
  /// Assert that the other signatures are empty too which means all signatures
  /// had the same size.
  util::static_for_each_in(util::identity<Args...>{}, [&](auto rest) {
    auto is_empty = (util::pack_size_of(rest) == util::size_constant_of<0>());
    static_assert(is_empty.value, "Expected all continuations to have the same"
                                  "count of arguments!");
  });
  return signature;
}

/// Determine the common result between all continuation which are chained
/// with an `any` strategy, consider two continuations:
/// c1 with `void(int)` and c2 with `void(float)`, the common result shared
/// between both continuations is `void(int)`.
template <typename Signature, typename First, typename... Args>
constexpr auto common_result_of(Signature signature, First first,
                                Args... args) {
  using Common =
      util::identity<std::common_type_t<decltype(first_of(first)),
                                        decltype(first_of(args))...>>;

  return common_result_of(util::push(signature, Common{}),
                          util::pop_first(first), util::pop_first(args)...);
}

/// Finalizes the any logic of a given composition
template <typename Data>
auto finalize_composition(
    continuable_base<Data, strategy_any_tag>&& continuation) {

  auto ownership_ = base::attorney::ownership_of(continuation);

  auto composition = base::attorney::consume_data(std::move(continuation));

  // Determine the shared result between all continuations
  auto signature = util::unpack(composition, [](auto const&... args) {
    return common_result_of(signature_hint_tag<>{},
                            base::hint_of(util::identity_of(args))...);
  });

  return base::attorney::create(
      [composition = std::move(composition)](auto&& callback) mutable {

        // Create the submitter which calls the given callback once at the first
        // callback invocation.
        auto submitter = make_any_result_submitter(
            std::forward<decltype(callback)>(callback));

        util::static_for_each_in(std::move(composition),
                                 [&](auto&& entry) mutable {
                                   // Invoke the continuation with a submission
                                   // callback
                                   base::attorney::invoke_continuation(
                                       std::forward<decltype(entry)>(entry),
                                       submitter->create_callback());
                                 });
      },
      signature, std::move(ownership_));
}

/// Connects the left and the right continuable to a sequence
///
/// \note This is implemented in an eager way because we would not gain
///       any profit from chaining sequences lazily.
template <typename Left, typename Right>
auto sequential_connect(Left&& left, Right&& right) {
  left.freeze(right.is_frozen());
  right.freeze();

  return std::forward<Left>(left).then([right = std::forward<Right>(right)](
      auto&&... args) mutable {
    return std::move(right).then([previous = std::make_tuple(
                                      std::forward<decltype(args)>(args)...)](
        auto&&... args) mutable {
      return util::merge(
          std::move(previous),
          std::make_tuple(std::forward<decltype(args)>(args)...));
    });
  });
}
} // end namespace compose

/// Provides helper functions to transform continuations to other types
namespace transforms {
/// Provides helper functions and typedefs for converting callback arguments
/// to their types a promise can accept.
template <typename... Args> struct future_trait {
  /// The promise type used to create the future
  using promise_t = std::promise<std::tuple<Args...>>;
  /// Boxes the argument pack into a tuple
  static void resolve(promise_t& promise, Args... args) {
    promise.set_value(std::make_tuple(std::move(args)...));
  }
};
template <> struct future_trait<> {
  /// The promise type used to create the future
  using promise_t = std::promise<void>;
  /// Boxes the argument pack into void
  static void resolve(promise_t& promise) { promise.set_value(); }
};
template <typename First> struct future_trait<First> {
  /// The promise type used to create the future
  using promise_t = std::promise<First>;
  /// Boxes the argument pack into nothing
  static void resolve(promise_t& promise, First first) {
    promise.set_value(std::move(first));
  }
};

template <typename Hint> class promise_callback;

template <typename... Args>
class promise_callback<signature_hint_tag<Args...>>
    : public future_trait<Args...> {

  typename future_trait<Args...>::promise_t promise_;

public:
  constexpr promise_callback() = default;
  promise_callback(promise_callback const&) = delete;
  constexpr promise_callback(promise_callback&&) = default;
  promise_callback& operator=(promise_callback const&) = delete;
  promise_callback& operator=(promise_callback&&) = delete;

  /// Resolves the promise
  void operator()(Args... args) { this->resolve(promise_, std::move(args)...); }
  /// Returns the future from the promise
  auto get_future() { return promise_.get_future(); }
};

/// Transforms the continuation to a future
template <typename Data, typename Annotation>
auto as_future(continuable_base<Data, Annotation>&& continuable) {
  // Create the promise which is able to supply the current arguments
  auto hint = base::hint_of(util::identity_of(continuable));

  promise_callback<std::decay_t<decltype(hint)>> callback;
  (void)hint;

  // Receive the future
  auto future = callback.get_future();

  // Dispatch the continuation with the promise resolving callback
  std::move(continuable).then(std::move(callback)).done();

  return future;
}
} // end namespace transforms
} // end namespace detail

template <typename Data, typename Annotation> class continuable_base {
  /// \cond false
  template <typename, typename> friend class continuable_base;
  friend struct detail::base::attorney;

  // The continuation type or intermediate result
  Data data_;
  // The transferable state which represents the validity of the object
  detail::ownership ownership_;
  /// \endcond

  /// Constructor accepting the data object while erasing the annotation
  explicit continuable_base(Data data, detail::ownership ownership)
      : data_(std::move(data)), ownership_(std::move(ownership)) {}

public:
  /// Constructor accepting the data object while erasing the annotation
  explicit continuable_base(Data data) : data_(std::move(data)) {}

  /// Constructor accepting any object convertible to the data object,
  /// while erasing the annotation
  template <typename OData, std::enable_if_t<std::is_convertible<
                                std::decay_t<OData>, Data>::value>* = nullptr>
  continuable_base(OData&& data) : data_(std::forward<OData>(data)) {}

  /// Constructor taking the data of other continuable_base objects
  /// while erasing the hint.
  ///
  /// This constructor makes it possible to replace the internal data object of
  /// the continuable by any object which is useful for type-erasure.
  template <typename OData, typename OAnnotation>
  continuable_base(continuable_base<OData, OAnnotation>&& other)
      : continuable_base(std::move(other).materialize().consume_data()) {}

  /// \cond false
  continuable_base(continuable_base&&) = default;
  continuable_base(continuable_base const&) = default;

  continuable_base& operator=(continuable_base&&) = default;
  continuable_base& operator=(continuable_base const&) = default;
  /// \endcond

  /// The destructor automatically invokes the continuable_base
  /// if it wasn't consumed yet.
  ///
  /// In order to invoke the continuable early you may call the
  /// continuable_base::done() method.
  ///
  /// The continuable_base::freeze method disables the automatic
  /// invocation on destruction without invalidating the object.
  ///
  /// \since version 1.0.0
  ~continuable_base() {
    if (ownership_.is_acquired() && !ownership_.is_frozen()) {
      std::move(*this).done();
    }
    assert((!ownership_.is_acquired() || ownership_.is_frozen()) &&
           "Ownership should be released!");
  }

  /// Main method of the continuable_base to chain the current continuation
  /// with a new callback.
  ///
  /// \param callback The callback which is used to process the current
  ///        asynchronous result on arrival. The callback is required to accept
  ///        the current result at least partially (or nothing of the result).
  /// ```cpp
  /// (http_request("github.com") && http_request("atom.io"))
  ///   .then([](std::string github, std::string atom) {
  ///     // We use the whole result
  ///   });
  ///
  /// (http_request("github.com") && http_request("atom.io"))
  ///   .then([](std::string github) {
  ///     // We only use the result partially
  ///   });
  ///
  /// (http_request("github.com") && http_request("atom.io"))
  ///   .then([] {
  ///     // We discard the result
  ///   });
  /// ```
  ///
  /// \param executor The optional executor which is used to dispatch
  ///        the callback. The executor needs to accept functional objects
  ///        callable through an `operator()` through its operator() itself.
  ///        The executor can be move-only, but it's not required to.
  ///        The default executor which is used when omitting the argument
  ///        dispatches the callback on the current executing thread.
  ///        Consider the example shown below:
  /// ```cpp
  /// auto executor = [](auto&& work) {
  ///   // Dispatch the work here or forward it to an executor of
  ///   // your choice.
  ///   std::forward<decltype(work)>(work)();
  /// };
  ///
  /// http_request("github.com")
  ///   .then([](std::string github) {
  ///     // Do something...
  ///    }, executor);
  /// ```
  ///
  /// \returns Returns a continuable_base with an asynchronous return type
  ///          depending on the return value of the callback:
  /// |      Callback returns      |              Resulting type               |
  /// | : ---------------------- : | : --------------------------------------- |
  /// | `void`                     | `continuable_base with <>`                |
  /// | `Arg`                      | `continuable_base with <Arg>`             |
  /// | `std::pair<First, Second>` | `continuable_base with <First, Second>`   |
  /// | `std::tuple<Args...>`      | `continuable_base with <Args...>`         |
  /// | `continuable_base<Arg...>` | `continuable_base with <Args...>`         |
  ///          Which means the result type of the continuable_base is equal to
  ///          the plain types the callback returns (`std::tuple` and
  ///          `std::pair` arguments are unwrapped).
  ///          A single continuable_base as argument is resolved and the result
  ///          type is equal to the resolved continuable_base.
  ///          Consider the following examples:
  /// ```cpp
  /// http_request("github.com")
  ///   .then([](std::string github) { return; })
  ///   .then([] { }); // <void>
  ///
  /// http_request("github.com")
  ///   .then([](std::string github) { return 0; })
  ///   .then([](int a) { }); // <int>
  ///
  /// http_request("github.com")
  ///   .then([](std::string github) { return std::make_pair(1, 2); })
  ///   .then([](int a, int b) { }); // <int, int>
  ///
  /// http_request("github.com")
  ///   .then([](std::string github) { return std::make_tuple(1, 2, 3); })
  ///   .then([](int a, int b, int c) { }); // <int, int, int>
  ///
  /// http_request("github.com")
  ///   .then([](std::string github) { return http_request("atom.io"); })
  ///   .then([](std::string atom) { }); // <std::string>
  /// ```
  ///
  /// \since version 1.0.0
  template <typename T, typename E = detail::this_thread_executor_tag>
  auto then(T&& callback,
            E&& executor = detail::this_thread_executor_tag{}) && {
    return detail::base::chain_continuation(std::move(*this).materialize(),
                                            std::forward<T>(callback),
                                            std::forward<E>(executor));
  }

  /// Additional overload of the continuable_base::then() method
  /// which is accepting a continuable_base itself.
  ///
  /// \param continuation A continuable_base reflecting the continuation
  ///        which is used to continue the call hierarchy.
  ///        The result of the current continuable is discarded and the given
  ///        continuation is invoked as shown below.
  /// ```cpp
  /// http_request("github.com")
  ///   .then(http_request("atom.io"))
  ///   .then([](std::string atom) {
  ///     // ...
  ///   });
  /// ```
  ///
  /// \returns Returns a continuable_base representing the next asynchronous
  ///          result to continue within the asynchronous call hierarchy.
  ///
  /// \since version 1.0.0
  template <typename OData, typename OAnnotation>
  auto then(continuable_base<OData, OAnnotation>&& continuation) && {
    return std::move(*this).then(
        detail::base::wrap_continuation(std::move(continuation).materialize()));
  }

  /// Invokes both continuable_base objects parallel and calls the
  /// callback with the result of both continuable_base objects.
  ///
  /// \param right The continuable on the right-hand side to connect.
  ///
  /// \returns Returns a continuable_base with a result type matching
  ///          the result of the left continuable_base combined with the
  ///          right continuable_base.
  ///          The returned continuable_base will be in an intermediate lazy
  ///          state, further calls to its continuable_base::operator &&
  ///          will add other continuable_base objects to the current
  ///          invocation chain.
  /// ```cpp
  /// (http_request("github.com") && http_request("atom.io"))
  ///   .then([](std::string github, std::string atom) {
  ///     // ...
  ///   });
  ///
  /// auto request = http_request("github.com") && http_request("atom.io");
  /// (std::move(request) && http_request("travis-ci.org"))
  ///    // All three requests are invoked in parallel although we added
  ///    // the request to "travis-ci.org" last.
  ///   .then([](std::string github, std::string atom, std::string travis) {
  ///     // ...
  ///   });
  /// ```
  ///
  /// \note The continuable_base objects are invoked parallel on the
  ///       current thread, because the `all` strategy tries to resolve
  ///       the continuations as fast as possible.
  ///       Sequential invocation is also supported through the
  ///       continuable_base::operator>> method.
  ///
  /// \since version 1.0.0
  template <typename OData, typename OAnnotation>
  auto operator&&(continuable_base<OData, OAnnotation>&& right) && {
    return detail::compose::connect(detail::compose::strategy_all_tag{},
                                    std::move(*this), std::move(right));
  }

  /// Invokes both continuable_base objects parallel and calls the
  /// callback once with the first result available.
  ///
  /// \param right The continuable on the right-hand side to connect.
  ///              The right continuable is required to have a compatible
  ///              result to the left connected continuable_base,
  ///              such that `std::common_type_t<Left, Right>` deduces to
  ///              a variable for every type in the result of the left and
  ///              the right continuable_base.
  ///
  /// \returns Returns a continuable_base with a result type matching
  ///          the combined result which of all connected
  ///          continuable_base objects.
  ///          The combined result is evaluated through the `std::common_type`
  ///          trait which returns the type all types can be converted to.
  ///          The returned continuable_base will be in an intermediate lazy
  ///          state, further calls to its continuable_base::operator &&
  ///          will add other continuable_base objects to the current
  ///          invocation chain.
  /// ```cpp
  /// (http_request("github.com") || http_request("atom.io"))
  ///   .then([](std::string github_or_atom) {
  ///     // ...
  ///   });
  ///
  /// (supply(10, 'T') || supply(10.f, 'T'))
  ///   .then([](int a, char b) {
  ///     // ...
  ///   });
  /// ```
  ///
  /// \note The continuable_base objects are invoked parallel on the
  ///       current thread, however, the callback is only called once with
  ///       the first result which becomes available.
  ///
  /// \since version 1.0.0
  template <typename OData, typename OAnnotation>
  auto operator||(continuable_base<OData, OAnnotation>&& right) && {
    return detail::compose::connect(detail::compose::strategy_any_tag{},
                                    std::move(*this), std::move(right));
  }

  /// Invokes both continuable_base objects sequential and calls the
  /// callback with the result of both continuable_base objects.
  ///
  /// \param right The continuable on the right-hand side to connect.
  ///
  /// \returns Returns a continuable_base with a result type matching
  ///          the result of the left continuable_base combined with the
  ///          right continuable_base.
  /// ```cpp
  /// (http_request("github.com") >> http_request("atom.io"))
  ///   .then([](std::string github, std::string atom) {
  ///     // The callback is called with the result of both requests,
  ///     // however, the request to atom was started after the request
  ///     // to github was finished.
  ///   });
  /// ```
  ///
  /// \note The continuable_base objects are invoked sequential on the
  ///       current thread. Parallel invocation is also supported through the
  ///       continuable_base::operator&& method.
  ///
  /// \since version 1.0.0
  template <typename OData, typename OAnnotation>
  auto operator>>(continuable_base<OData, OAnnotation>&& right) && {
    return detail::compose::sequential_connect(std::move(*this),
                                               std::move(right));
  }

  /// Starts the continuation chain and returns the asynchronous
  /// result as `std::future<...>`.
  ///
  /// \returns Returns a `std::future<...>` which becomes ready as soon
  ///          as the the continuation chain has finished.
  ///          The signature of the future depends on the result type:
  /// |          Continuation type        |             Return type            |
  /// | : ------------------------------- | : -------------------------------- |
  /// | `continuable_base with <>`        | `std::future<void>`                |
  /// | `continuable_base with <Arg>`     | `std::future<Arg>`                 |
  /// | `continuable_base with <Args...>` | `std::future<std::tuple<Args...>>` |
  ///
  /// \since version 1.0.0
  auto futurize() && {
    return detail::transforms::as_future(std::move(*this).materialize());
  }

  /// Invokes the continuation chain manually even before the
  /// cti::continuable_base is destructed. This will release the object.
  ///
  /// \see continuable_base::~continuable_base() for further details about
  ///      the continuation invocation on destruction.
  ///
  /// \attention This method will trigger an assertion if the
  ///            continuable_base was released already.
  ///
  /// \since version 1.0.0
  void done() && { detail::base::finalize_continuation(std::move(*this)); }

  /// Predicate to check whether the cti::continuable_base is frozen or not.
  ///
  /// \returns Returns true when the continuable_base is frozen.
  ///
  /// \see continuable_base::freeze for further details.
  ///
  /// \attention This method will trigger an assertion if the
  ///            continuable_base was released already.
  ///
  /// \since version 1.0.0
  bool is_frozen() const noexcept {
    assert_acquired();
    return ownership_.is_frozen();
  }

  /// Prevents the automatic invocation of the continuation chain
  /// which happens on destruction of the continuable_base.
  /// You may still invoke the chain through the continuable_base::done method.
  ///
  /// This is useful for storing a continuable_base inside a continuation
  /// chain while storing it for further usage.
  ///
  /// \param enabled Indicates whether the freeze is enabled or disabled.
  ///
  /// \see continuable_base::~continuable_base() for further details about
  ///      the continuation invocation on destruction.
  ///
  /// \attention This method will trigger an assertion if the
  ///            continuable_base was released already.
  ///
  /// \since version 1.0.0
  continuable_base& freeze(bool enabled = true) & noexcept {
    ownership_.freeze(enabled);
    return *this;
  }

  /// \copydoc continuable_base::freeze
  continuable_base&& freeze(bool enabled = true) && noexcept {
    ownership_.freeze(enabled);
    return std::move(*this);
  }

private:
  void release() noexcept { ownership_.release(); }

  auto materialize() &&
      noexcept(std::is_nothrow_move_constructible<Data>::value) {
    assert_acquired();
    return materializeImpl(std::move(*this));
  }

  template <typename OData, typename OAnnotation,
            std::enable_if_t<
                !detail::compose::is_strategy<OAnnotation>::value>* = nullptr>
  static auto
  materializeImpl(continuable_base<OData, OAnnotation>&& continuable) {
    return std::move(continuable);
  }
  template <typename OData, typename OAnnotation,
            std::enable_if_t<
                detail::compose::is_strategy<OAnnotation>::value>* = nullptr>
  static auto
  materializeImpl(continuable_base<OData, OAnnotation>&& continuable) {
    return detail::compose::finalize_composition(std::move(continuable));
  }

  Data&& consume_data() && {
    release();
    return std::move(data_);
  }

  void assert_acquired() const {
    assert(ownership_.is_acquired() && "Tried to use a released continuable!");
  }
};

/// Creates a continuable_base from a callback taking function.
///
/// \tparam Args The types (signature hint) the given callback is called with.
/// * **Some arguments** indicate the types the callback will be invoked with.
/// ```cpp
/// auto ct = cti::make_continuable<int, std::string>([](auto&& callback) {
///   std::forward<decltype(callback)>(callback)(200, "<html>...</html>");
/// });
/// ```
/// * **void as argument** indicates that the callback will be invoked
///   with no arguments:
/// ```cpp
/// auto ct = cti::make_continuable<void>([](auto&& callback) {
///   std::forward<decltype(callback)>(callback)();
/// });
/// ```
/// * **No arguments** indicate that the types are unknown.
///   You should always give the type hint a callback is called with because
///   it's required for intermediate actions like connecting continuables.
///   You may omit the signature hint if you are erasing the type of
///   the continuable right after creation.
/// ```cpp
/// // Never do this:
/// auto ct = cti::make_continuable([](auto&& callback) {
///   std::forward<decltype(callback)>(callback)(0.f, 'c');
/// });
///
/// // However, you may do this:
/// continuable<float, char> ct = cti::make_continuable([](auto&& callback) {
///   std::forward<decltype(callback)>(callback)(0.f, 'c');
/// });
/// ```
///
/// \param continuation The continuation the continuable is created from.
/// The continuation must be a functional type accepting a callback parameter
/// which represents the object invokable with the asynchronous result of this
/// continuable.
/// ```cpp
/// auto ct = cti::make_continuable([](auto&& callback) {
///   std::forward<decltype(callback)>(callback)("result");
/// });
/// ```
/// The callback may be stored or moved.
/// In some cases the callback may be copied if supported by the underlying
/// callback chain, in order to invoke the call chain multiple times.
/// It's recommended to accept any callback instead of erasing it.
/// ```cpp
/// // Good practice:
/// auto ct = cti::make_continuable([](auto&& callback) {
///   std::forward<decltype(callback)>(callback)("result");
/// });
///
/// // Good practice using a functional object:
/// struct Continuation {
///   template<typename T>
///   void operator() (T&& continuation) const {
///     // ...
///   }
/// }
///
/// auto ct = cti::make_continuable(Continuation{});
///
/// // Bad practice (because of unnecessary type erasure):
/// auto ct = cti::make_continuable(
///   [](std::function<void(std::string)> callback) {
///     callback("result");
///   });
/// ```
///
/// \returns A continuable_base with unknown template parameters which
///          wraps the given continuation.
///          In order to convert the continuable_base to a known type
///          you need to apply type erasure.
///
/// \note You should always turn the callback into a r-value if possible
///       (`std::move` or `std::forward`) for qualifier correct invokation.
///
/// \since version 1.0.0
template <typename... Args, typename Continuation>
auto make_continuable(Continuation&& continuation) {
  auto hint = detail::compose::annotating::extract(
      detail::util::identity_of(continuation),
      detail::util::identity<Args...>{});

  return detail::base::attorney::create(
      std::forward<Continuation>(continuation), hint, detail::ownership{});
}

/// Connects the given continuables with an *all* logic.
///
/// \param continuables The continuable_base objects to connect.
///        Requires at least 2 objects to connect.
///
/// \see continuable_base::operator && for details.
///
/// \since version 1.1.0
template <typename... Continuables>
auto when_all(Continuables&&... continuables) {
  static_assert(sizeof...(continuables) >= 2,
                "Requires at least 2 continuables!");
  return detail::util::fold(detail::util::and_folding(),
                            std::forward<Continuables>(continuables)...);
}

/// \copydoc when_all
///
/// \since version 1.0.0
///
/// \deprecated Use the `when_all` function instead.
template <typename... Continuables>
[[deprecated("Replaced by cti::when_all")]]
auto all_of(Continuables&&... continuables) {
  return when_all(std::forward<Continuables>(continuables)...);
}

/// Connects the given continuables with an *any* logic.
///
/// \param continuables The continuable_base objects to connect.
///        Requires at least 2 objects to connect.
///
/// \see continuable_base::operator|| for details.
///
/// \since version 1.1.0
template <typename... Continuables>
auto when_any(Continuables&&... continuables) {
  static_assert(sizeof...(continuables) >= 2,
                "Requires at least 2 continuables!");
  return detail::util::fold(detail::util::or_folding(),
                            std::forward<Continuables>(continuables)...);
}

/// \copydoc when_any
///
/// \since version 1.0.0
///
/// \deprecated Use the `when_any` function instead.
template <typename... Continuables>
[[deprecated("Replaced by cti::when_any")]]
auto any_of(Continuables&&... continuables) {
  return when_any(std::forward<Continuables>(continuables)...);
}

/// Connects the given continuables with a *seq* logic.
///
/// \param continuables The continuable_base objects to connect.
///        Requires at least 2 objects to connect.
///
/// \see continuable_base::operator>> for details.
///
/// \since version 1.1.0
template <typename... Continuables>
auto when_seq(Continuables&&... continuables) {
  static_assert(sizeof...(continuables) >= 2,
                "Requires at least 2 continuables!");
  return detail::util::fold(detail::util::seq_folding(),
                            std::forward<Continuables>(continuables)...);
}

/// \copydoc when_seq
///
/// \since version 1.0.0
///
/// \deprecated Use the `when_seq` function instead.
template <typename... Continuables>
[[deprecated("Replaced by cti::when_seq")]]
auto seq_of(Continuables&&... continuables) {
  return when_seq(std::forward<Continuables>(continuables)...);
}

/// Trait to retrieve a continuable_base type with a given type-erasure backend.
///
/// Every object may me used as type-erasure backend as long as the
/// requirements of a `std::function` like wrapper are satisfied.
///
/// \tparam CallbackWrapper The type which is used to erase the callback.
///
/// \tparam ContinuationWrapper The type which is used to erase the
///         continuation data.
///
/// \tparam Args The current signature of the continuable.
template <template <typename> class CallbackWrapper,
          template <typename> class ContinuationWrapper, typename... Args>
struct continuable_trait {
  /// The callback type which is passed to continuations
  using callback = CallbackWrapper<void(Args...)>;

  /// The continuation type which is used to erase the internal data.
  using continuation =
      ContinuationWrapper<void(CallbackWrapper<void(Args...)>)>;

  /// The continuable type for the given parameters.
  using continuable =
      continuable_base<continuation, detail::signature_hint_tag<Args...>>;
};

/// \cond false
} // end inline namespace abi_...
/// \endcond
} // end namespace cti

#endif // CONTINUABLE_BASE_HPP_INCLUDED__
