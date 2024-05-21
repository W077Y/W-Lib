#include <exception>
#include <wlib.hpp>

namespace wlib::errors
{
  class wlib_exception: public std::exception
  {
  public:
    wlib_exception(char const* msg)
        : m_msg(msg)
    {
    }

    char const* what() const noexcept override { return this->m_msg; }

  private:
    char const* m_msg;
  };

  void throw_error(char const* msg) { throw wlib_exception(msg); }

  void throw_multible_active_con_error() { return throw_error("Invalid use. There is already an active connection."); }
}    // namespace wlib::errors
