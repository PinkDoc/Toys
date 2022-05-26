#pragma once

namespace detail {

template <std::size_t S, typename... Args> struct union_size {

  constexpr static auto value = S;

};

template <std::size_t T, typename N, typename... Args>
struct union_size<T, N, Args...> {

  constexpr static auto value = T > sizeof(N)
                                    ? union_size<T, Args...>::value
                                    : union_size<sizeof(N), Args...>::value;

};

template <typename... Args> struct size {
  constexpr static auto value = union_size<0, Args...>::value;
};

} // namespace detail

namespace tem
{
    using detail::size;

    template <typename... Args>
    class tem_union
    {   
    public:
      template <typename T> T &as() {
        return *(T *)(holder);
      }

    private:
        char holder[size<Args...>::value];
    };
}
