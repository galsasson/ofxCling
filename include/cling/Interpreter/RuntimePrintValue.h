//--------------------------------------------------------------------*- C++ -*-
// CLING - the C++ LLVM-based InterpreterG :)
// author:  Boris Perovic <boris.perovic@cern.ch>
// author:  Danilo Piparo <danilo.piparo@cern.ch>
//
// This file is dual-licensed: you can choose to license it under the University
// of Illinois Open Source License or the GNU Lesser General Public License. See
// LICENSE.TXT for details.
//------------------------------------------------------------------------------
#ifndef CLING_RUNTIME_PRINT_VALUE_H
#define CLING_RUNTIME_PRINT_VALUE_H

#if !defined(__CLING__)
#error "This file must not be included by compiled programs."
#endif

#include <string>
#include <tuple>
#include <type_traits>

namespace cling {

  class Value;

  // General fallback - prints the address
  std::string printValue(const void *ptr);

  // Fallback for e.g. vector<bool>'s bit iterator:
  template <class T,
    class = typename std::enable_if<!std::is_pointer<T>::value>::type>
  std::string printValue(const T& val) { return "{not representable}"; }

  // void pointer
  std::string printValue(const void **ptr);

  // Bool
  std::string printValue(const bool *val);

  // Chars
  std::string printValue(const char *val);

  std::string printValue(const signed char *val);

  std::string printValue(const unsigned char *val);

  // Ints
  std::string printValue(const short *val);

  std::string printValue(const unsigned short *val);

  std::string printValue(const int *val);

  std::string printValue(const unsigned int *val);

  std::string printValue(const long *val);

  std::string printValue(const unsigned long *val);

  std::string printValue(const long long *val);

  std::string printValue(const unsigned long long *val);

  // Reals
  std::string printValue(const float *val);

  std::string printValue(const double *val);

  std::string printValue(const long double *val);

  // Char pointers
  std::string printValue(const char *const *val);

  std::string printValue(const char **val);

  // std::string
  std::string printValue(const std::string *val);

  // cling::Value
  std::string printValue(const Value *value);

  namespace valuePrinterInternal {
    extern const char* const kEmptyCollection;
  }

  // Collections internal
  namespace collectionPrinterInternal {
    // Maps
    template<typename Pair>
    std::string printPair(const Pair& P) {
      return printValue(&P->first) + " => " + printValue(&P->second);
    }
    
    template<typename CollectionType>
    auto printValue_impl(const CollectionType *obj, short)
    -> decltype(
    ++(obj->begin()), obj->end(),
        obj->begin()->first, obj->begin()->second,
        std::string())
    {
      auto iter = obj->begin();
      auto iterEnd = obj->end();
      if (iter == iterEnd)
        return valuePrinterInternal::kEmptyCollection;

      std::string str("{ ");
      str += printPair(iter);
      while (++iter != iterEnd) {
        str += ", ";
        str += printPair(iter);
      }
      return str + " }";
    }

    // Vector, set, deque etc.
    template<typename CollectionType>
    auto printValue_impl(const CollectionType *obj, int)
    -> decltype(
    ++(obj->begin()), obj->end(),
        *(obj->begin()),  &(*(obj->begin())),
        std::string())
    {
      auto iter = obj->begin();
      auto iterEnd = obj->end();
      if (iter == iterEnd)
        return valuePrinterInternal::kEmptyCollection;

      std::string str("{ ");
      str += printValue(&(*iter));
      while (++iter != iterEnd) {
        str += ", ";
        str += printValue(&(*iter));
      }
      return str + " }";
    }

    // As above, but without ability to take address of elements.
    template<typename CollectionType>
    auto printValue_impl(const CollectionType *obj, long)
    -> decltype(
    ++(obj->begin()), obj->end(),
        *(obj->begin()),
        std::string())
     {
      auto iter = obj->begin();
      auto iterEnd = obj->end();
      if (iter == iterEnd)
        return valuePrinterInternal::kEmptyCollection;

      std::string str("{ ");
      const auto value0 = (*iter);
      str += printValue(&value0);
      while (++iter != iterEnd) {
        const auto valueN = (*iter);
        str += ", ";
        str += printValue(&valueN);
      }
      return str + " }";
     }
  }

  // Collections
  template<typename CollectionType>
  auto printValue(const CollectionType *obj)
  -> decltype(collectionPrinterInternal::printValue_impl(obj, 0), std::string())
  {
    // short -> int -> long = priority order
    return collectionPrinterInternal::printValue_impl(obj, (short)0);
  }

  // Arrays
  template<typename T, size_t N>
  std::string printValue(const T (*obj)[N]) {
    if (N == 0)
      return valuePrinterInternal::kEmptyCollection;

    std::string str = "{ ";
    str += printValue(*obj + 0);
    for (size_t i = 1; i < N; ++i) {
      str += ", ";
      str += printValue(*obj + i);
    }
    return str + " }";
  }

  // Tuples
  template <class... ARGS>
  std::string printValue(std::tuple<ARGS...> *);

  namespace collectionPrinterInternal {

    template <std::size_t N>
    const char *GetCommaOrEmpty()
    {
      static const auto comma = ", ";
      return comma;
    }

    template <>
    const char *GetCommaOrEmpty<0>()
    {
      static const auto empty = "";
      return empty;
    }

    // We loop at compile time from element 0 to element TUPLE_SIZE - 1
    // of the tuple. The running index is N which has as initial value
    // TUPLE_SIZE. We can therefore stop the iteration and account for the
    // empty tuple case with one single specialisation.
    template <class TUPLE,
              std::size_t N = std::tuple_size<TUPLE>(),
              std::size_t TUPLE_SIZE = std::tuple_size<TUPLE>()>
    struct tuplePrinter {
      static std::string print(TUPLE *t)
      {
        constexpr std::size_t elementNumber = TUPLE_SIZE - N;
        using Element_t = decltype(std::get<elementNumber>(*t));
        std::string ret;
        ret += GetCommaOrEmpty<elementNumber>();
        ret += cling::printValue(&std::get<elementNumber>(*t));
        // If N+1 is not smaller than the size of the tuple,
        // reroute the call to the printing function to the
        // no-op specialisation to stop recursion.
        constexpr std::size_t Nm1 = N - 1;
        ret += tuplePrinter<TUPLE, Nm1>::print((TUPLE *)t);
        return ret;
      }
    };

    // Special case: no op if last element reached or empty tuple
    template <class TUPLE, std::size_t TUPLE_SIZE>
    struct tuplePrinter<TUPLE, 0, TUPLE_SIZE>
    {
      static std::string print(TUPLE *t) {return "";}
    };

    template <class T>
    std::string tuplePairPrintValue(T *val)
    {
      std::string ret("{ ");
      ret += collectionPrinterInternal::tuplePrinter<T>::print(val);
      ret += " }";
      return ret;
    }
  }

  template <class... ARGS>
  std::string printValue(std::tuple<ARGS...> *val)
  {
    using T = std::tuple<ARGS...>;
    if (std::tuple_size<T>::value == 0)
      return valuePrinterInternal::kEmptyCollection;
    return collectionPrinterInternal::tuplePairPrintValue<T>(val);
  }

  template <class... ARGS>
  std::string printValue(std::pair<ARGS...> *val)
  {
    using T = std::pair<ARGS...>;
    return collectionPrinterInternal::tuplePairPrintValue<T>(val);
  }
}

#endif
