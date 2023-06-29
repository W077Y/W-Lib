#pragma once
#ifndef WLIB_PUPLISHER_INTERFACE_HPP
#define WLIB_PUPLISHER_INTERFACE_HPP

#include <type_traits>

namespace WLib
{
  template <typename T> class Puplisher_Sink_Interface;
  template <typename T> class Subscription_Interface;
  template <typename T> class Subscriber_Interface;
  template <typename T> class Puplisher_Base;

  template <typename T> class Puplisher_Sink_Interface
  {
  public:
    using payload_t                              = std::remove_cv_t<T>;
    virtual ~Puplisher_Sink_Interface()          = default;
    virtual void puplish(payload_t const& value) = 0;
  };

  template <typename T> class Subscriber_Interface
  {
    using payload_t = std::remove_cv_t<T>;

    Subscriber_Interface(Subscriber_Interface const&)            = delete;
    Subscriber_Interface& operator=(Subscriber_Interface const&) = delete;

    Subscriber_Interface(Subscriber_Interface&&)            = delete;
    Subscriber_Interface& operator=(Subscriber_Interface&&) = delete;

  public:
    Subscriber_Interface(Subscription_Interface<T>& parent);
    virtual ~Subscriber_Interface();

    virtual void receive_data(payload_t const& data) = 0;

  private:
    friend class Puplisher_Base<T>;
    Subscription_Interface<T>& m_list;
    Subscriber_Interface*      m_next;
  };

  template <typename T> class Subscription_Interface
  {
  public:
    virtual ~Subscription_Interface()                  = default;
    virtual void add(Subscriber_Interface<T>& node)    = 0;
    virtual void remove(Subscriber_Interface<T>& node) = 0;
  };

  template <typename T>
  class Puplisher_Base
      : public Puplisher_Sink_Interface<T>
      , public Subscription_Interface<T>
  {
  public:
    using payload_t = typename Puplisher_Sink_Interface<T>::payload_t;

  public:
    template <typename U> std::enable_if_t<true, U> subscribe() { return U{ *this }; }

  protected:
    void p_puplish(payload_t const& value);
    void p_add(Subscriber_Interface<T>& node);
    void p_remove(Subscriber_Interface<T>& node);

    Subscriber_Interface<T>* m_head_node = nullptr;
  };

}

  template <typename T> WLib::Subscriber_Interface<T>::~Subscriber_Interface() { this->m_list.remove(*this); }
  template <typename T>
  WLib::Subscriber_Interface<T>::Subscriber_Interface(Subscription_Interface<T>& parent)
      : m_list(parent)
      , m_next(nullptr)
  {
    parent.add(*this);
  }

  template <typename T> void WLib::Puplisher_Base<T>::p_puplish(payload_t const& value)
  {
    Subscriber_Interface<T>* cur = this->m_head_node;
    while (cur != nullptr)
    {
      cur->receive_data(value);
      cur = cur->m_next;
    }
  }

  template <typename T> void WLib::Puplisher_Base<T>::p_add(Subscriber_Interface<T>& node)
  {
    node.m_next = this->m_head_node;
    this->m_head_node = &node;
  }

  template <typename T> void WLib::Puplisher_Base<T>::p_remove(Subscriber_Interface<T>& node)
  {
    if (this->m_head_node == nullptr)
      return;

    if (this->m_head_node == &node)
    {
      this->m_head_node = node.m_next;
      return;
    }

    Subscriber_Interface<T>* cur = this->m_head_node;
    while (cur->m_next != nullptr)
    {
      if (cur->m_next == &node)
      {
        cur->m_next = node.m_next;
        return;
      }
      cur = cur->m_next;
    }
  }
#endif    // !WLIB_Puplisher_Interface_hpp
