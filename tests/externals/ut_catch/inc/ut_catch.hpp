
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26451)
#pragma warning(disable : 26439)
#pragma warning(disable : 26495)
#include <catch.hpp>
#pragma warning(pop)
#elif __GNUG__
#include <catch.hpp>
#elif __clang__
#pragma warning(push)
#include <catch.hpp>
#pragma warning(pop)
#else
#include <catch.hpp>
#endif    // MSVC