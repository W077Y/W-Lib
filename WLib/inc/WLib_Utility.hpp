#pragma once
#ifndef WLIB_UTILITY_HPP
#define WLIB_UTILITY_HPP

#ifndef WLIB_NO_THROW
#include <exception>
#endif


#ifndef WLIB_NO_THROW
namespace WLib::Internal
{
  class char_const_ptr_exception final : public std::exception
  {
  public:
    char_const_ptr_exception(char const* str)
        : m_str(str)
    {
    }

    virtual const char* what() const noexcept override { return this->m_str; };

  private:
    char const* const m_str;
  };
} // namespace WLib::Internal
#endif

namespace WLib::Internal
{
  struct no_copy_t
  {
    no_copy_t()                            = default;
    no_copy_t(no_copy_t const&)            = delete;
    no_copy_t& operator=(no_copy_t const&) = delete;
  };

  struct no_move_t
  {
    no_move_t()                       = default;
    no_move_t(no_move_t&&)            = delete;
    no_move_t& operator=(no_move_t&&) = delete;
  };

  struct no_copy_no_move_t
  {
    no_copy_no_move_t()                                    = default;
    no_copy_no_move_t(no_copy_no_move_t const&)            = delete;
    no_copy_no_move_t& operator=(no_copy_no_move_t const&) = delete;
    no_copy_no_move_t(no_copy_no_move_t&&)                 = delete;
    no_copy_no_move_t& operator=(no_copy_no_move_t&&)      = delete;
  };
} // namespace WLib::Internal
#endif
