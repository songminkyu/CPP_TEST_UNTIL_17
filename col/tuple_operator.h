#pragma once

#include <tuple>
#include <col/src/stringt/stringt.h>
#include <col/src/col_stream/bstream.h>

template<class Ch, class Tr, std::size_t I, typename T,typename... TS>
struct tuple_bostream
{
    static void reference(basic_bostream<Ch,Tr> & out, const std::tuple<T,TS...> & t)
    {
        tuple_bostream<Ch, Tr, I-1, T, TS...>::reference(out, t);
        out << std::get<I>(t);
    }
};
template<class Ch, class Tr, typename T,typename... TS>
struct tuple_bostream<Ch, Tr, 0, T, TS...>
{
    static void reference(basic_bostream<Ch,Tr> & out, const std::tuple<T,TS...> & t)
    {
        out << std::get<0>(t);
    }
};
template<class Ch, class Tr, typename T,typename... TS>
struct tuple_bostream<Ch, Tr, -1, T, TS...>
{
    static void reference(basic_bostream<Ch,Tr> & out, const std::tuple<T,TS...> & t)
    {}
};


template<class Ch, class Tr, std::size_t I, typename T, typename... TS>
struct tuple_bistream
{
    static void reference(basic_bistream<Ch,Tr> & In, std::tuple<T,TS...> & t)
    {
        tuple_bistream<Ch, Tr, I-1, T, TS...>::reference(In, t);
        In >> std::get<I>(t);
    }
};
template<class Ch, class Tr, typename T, typename... TS>
struct tuple_bistream<Ch, Tr, 0, T, TS...>
{
    static void reference(basic_bistream<Ch,Tr> & In, std::tuple<T,TS...> & t)
    {
        In >> std::get<0>(t);
    }
};
template<class Ch, class Tr, typename T,typename... TS>
struct tuple_bistream<Ch, Tr, -1, T, TS...>
{
    static void reference(basic_bistream<Ch,Tr> & In, std::tuple<T, TS...> & t)
    {}
};
