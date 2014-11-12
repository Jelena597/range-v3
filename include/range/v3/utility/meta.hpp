// Range v3 library
//
//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//

#ifndef RANGES_V3_UTILITY_META_HPP
#define RANGES_V3_UTILITY_META_HPP

namespace ranges
{
    inline namespace v3
    {
        ////////////////////////////////////////////////////////////////////////////////////
        // General meta-programming utilities
        template<typename F, typename...Args>
        using meta_apply = typename F::template apply<Args...>;

        template<typename T>
        using meta_eval = typename T::type;

#if __GNUC__ == 4 && __GNUC_MINOR__ <= 9
        // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=61738
        template<template<typename...> class C, typename...As>
        using meta_quote_apply = typename C<As...>::type;
#else
        template<template<typename...> class C, typename...As>
        using meta_quote_apply = meta_eval<C<As...>>;
#endif

        template<template<typename...> class C>
        struct meta_quote
        {
        private:
            // Indirection here needed to avoid Core issue 1430
            // http://open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
            template<typename...Ts>
            struct impl
            {
                using type = meta_quote_apply<C, Ts...>;
            };
        public:
            using type = meta_quote;

            template<typename...Ts>
            using apply = meta_quote_apply<impl, Ts...>;
        };

        template<template<typename...> class C>
        struct meta_quote_alias
        {
        private:
            // Indirection here needed to avoid Core issue 1430
            // http://open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
            template<typename...Ts>
            struct impl
            {
                using type = C<Ts...>;
            };
        public:
            using type = meta_quote_alias;

            template<typename...Ts>
            using apply = meta_quote_apply<impl, Ts...>;
        };

        template<typename T, template<T...> class F>
        struct meta_quote_i
        {
        private:
            // Indirection here needed to avoid Core issue 1430
            // http://open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
            template<typename ...Ts>
            struct impl
            {
                using type = typename F<Ts::value...>::type;
            };
        public:
            using type = meta_quote_i;

            template<typename...Ts>
            using apply = meta_quote_apply<impl, Ts...>;
        };

        template<typename T, template<T...> class F>
        struct meta_quote_alias_i
        {
        private:
            // Indirection here needed to avoid Core issue 1430
            // http://open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1430
            template<typename ...Ts>
            struct impl
            {
                using type = F<Ts::value...>;
            };
        public:
            using type = meta_quote_alias_i;

            template<typename...Ts>
            using apply = meta_quote_apply<impl, Ts...>;
        };

        template<typename...Fs>
        struct meta_compose
        {
            using type = meta_compose;
        };

        template<typename F0>
        struct meta_compose<F0>
        {
            using type = meta_compose;

            template<typename...Ts>
            using apply = meta_apply<F0, Ts...>;
        };

        template<typename F0, typename...Fs>
        struct meta_compose<F0, Fs...>
        {
            using type = meta_compose;

            template<typename...Ts>
            using apply = meta_apply<F0, meta_apply<meta_compose<Fs...>, Ts...>>;
        };

        template<typename T>
        struct meta_always
        {
            using type = meta_always;

            template<typename...>
            using apply = T;
        };

        template<typename F, typename...Ts>
        struct meta_bind_front
        {
            using type = meta_bind_front;

            template<typename...Us>
            using apply = meta_apply<F, Ts..., Us...>;
        };

        template<typename F, typename...Us>
        struct meta_bind_back
        {
            using type = meta_bind_back;

            template<typename...Ts>
            using apply = meta_apply<F, Ts..., Us...>;
        };
    }
}

#endif