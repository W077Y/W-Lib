#include <WLib_SPI_Interface.hpp>
#include <exception>
#include <ut_catch.hpp>

class interface_error_exeption: public std::exception
{
public:
  virtual char const* what() const noexcept override { return "There is an error in the Interface"; }
};

class tst_dummy_hw final: public WLib::SPI::Hardware_Interface
{
  // Inherited via Hardware_Interface
  virtual void transcieve(std::byte const* tx, std::byte* rx, std::size_t const& len) override
  {
    if (this->m_hw_enable_count <= 0)
      throw interface_error_exeption();

    if (this->m_cur_cfg.get_baudrate() == 0)
      throw interface_error_exeption();

    this->m_byte_count.clk += len;
    if (tx != nullptr)
      this->m_byte_count.tx += len;
    if (rx != nullptr)
      this->m_byte_count.rx += len;
  }
  virtual void enable(SPI_configuration_t const& cfg) override
  {
    this->m_cur_cfg = cfg;
    this->m_hw_enable_count++;
    if (this->m_hw_enable_count != 1)
      throw interface_error_exeption();
  }

  virtual void disable() override
  {
    this->m_cur_cfg = SPI_configuration_t{ 0 };
    this->m_hw_enable_count--;
    if (this->m_hw_enable_count != 0)
      throw interface_error_exeption();
  }

public:
  struct byte_count_t
  {
    constexpr byte_count_t() = default;
    constexpr byte_count_t(std::size_t tx, std::size_t rx, std::size_t clk)
        : tx(tx)
        , rx(rx)
        , clk(clk)
    {
    }

    std::size_t tx  = 0;
    std::size_t rx  = 0;
    std::size_t clk = 0;

    constexpr bool operator==(byte_count_t const& rhs) const { return this->tx == rhs.tx && this->rx == rhs.rx && this->clk == rhs.clk; }
  };

  byte_count_t get_byte_count() const { return this->m_byte_count; }
  bool         is_enabled() const { return this->m_hw_enable_count != 0; }

private:
  int                 m_hw_enable_count = 0;
  SPI_configuration_t m_cur_cfg         = { 0 };
  byte_count_t        m_byte_count      = {};
};

class tst_dummy_cs final: public WLib::SPI::Chipselect_Interface
{
  // Inherited via Chipselect_Interface
  virtual void select() override
  {
    this->m_sel_count++;
    if (this->m_sel_count != 1)
      throw interface_error_exeption();
  }
  virtual void deselect() override
  {
    this->m_sel_count--;
    if (this->m_sel_count != 0)
      throw interface_error_exeption();
  }

public:
  bool is_selected() const { return this->m_sel_count != 0; }

private:
  int m_sel_count = 0;
};

TEST_CASE()
{
  using b_c = tst_dummy_hw::byte_count_t;
  tst_dummy_hw                   spi_1;
  tst_dummy_hw                   spi_2;
  tst_dummy_cs                   cs_a;
  tst_dummy_cs                   cs_b;
  tst_dummy_cs                   cs_c;
  tst_dummy_cs                   cs_d;
  WLib::SPI::SPI_configuration_t cfg{ 100 };

  REQUIRE_FALSE(spi_1.is_enabled());
  REQUIRE_FALSE(spi_2.is_enabled());
  REQUIRE(spi_1.get_byte_count() == b_c{ 0, 0, 0 });
  REQUIRE(spi_2.get_byte_count() == b_c{ 0, 0, 0 });
  REQUIRE_FALSE(cs_a.is_selected());
  REQUIRE_FALSE(cs_b.is_selected());
  REQUIRE_FALSE(cs_c.is_selected());
  REQUIRE_FALSE(cs_d.is_selected());

  {
    WLib::SPI::Hardware_handle_t hw_handle(spi_1, cfg);

    REQUIRE(spi_1.is_enabled());
    REQUIRE_FALSE(spi_2.is_enabled());
    REQUIRE(spi_1.get_byte_count() == b_c{ 0, 0, 0 });
    REQUIRE(spi_2.get_byte_count() == b_c{ 0, 0, 0 });
    REQUIRE_FALSE(cs_a.is_selected());
    REQUIRE_FALSE(cs_b.is_selected());
    REQUIRE_FALSE(cs_c.is_selected());
    REQUIRE_FALSE(cs_d.is_selected());

    {
      WLib::SPI::Connection_handle_t con_handle = hw_handle.select(cs_a);

      REQUIRE(spi_1.is_enabled());
      REQUIRE_FALSE(spi_2.is_enabled());
      REQUIRE(spi_1.get_byte_count() == b_c{ 0, 0, 0 });
      REQUIRE(spi_2.get_byte_count() == b_c{ 0, 0, 0 });
      REQUIRE(cs_a.is_selected());
      REQUIRE_FALSE(cs_b.is_selected());
      REQUIRE_FALSE(cs_c.is_selected());
      REQUIRE_FALSE(cs_d.is_selected());

      std::byte buf[128] = {};
      con_handle.transcieve(nullptr, nullptr, 2);
      con_handle.transcieve(buf, nullptr, 3);
      con_handle.transcieve(nullptr, buf, 5);
      con_handle.transcieve(buf, buf, 7);

      REQUIRE(spi_1.is_enabled());
      REQUIRE_FALSE(spi_2.is_enabled());
      REQUIRE(spi_1.get_byte_count() == b_c{ 10, 12, 17 });
      REQUIRE(spi_2.get_byte_count() == b_c{ 0, 0, 0 });
      REQUIRE(cs_a.is_selected());
      REQUIRE_FALSE(cs_b.is_selected());
      REQUIRE_FALSE(cs_c.is_selected());
      REQUIRE_FALSE(cs_d.is_selected());
    }
    REQUIRE(spi_1.is_enabled());
    REQUIRE_FALSE(spi_2.is_enabled());
    REQUIRE(spi_1.get_byte_count() == b_c{ 10, 12, 17 });
    REQUIRE(spi_2.get_byte_count() == b_c{ 0, 0, 0 });
    REQUIRE_FALSE(cs_a.is_selected());
    REQUIRE_FALSE(cs_b.is_selected());
    REQUIRE_FALSE(cs_c.is_selected());
    REQUIRE_FALSE(cs_d.is_selected());
    {
      WLib::SPI::Connection_handle_t con_handle = hw_handle.select(cs_b);

      REQUIRE(spi_1.is_enabled());
      REQUIRE_FALSE(spi_2.is_enabled());
      REQUIRE(spi_1.get_byte_count() == b_c{ 10, 12, 17 });
      REQUIRE(spi_2.get_byte_count() == b_c{ 0, 0, 0 });
      REQUIRE_FALSE(cs_a.is_selected());
      REQUIRE(cs_b.is_selected());
      REQUIRE_FALSE(cs_c.is_selected());
      REQUIRE_FALSE(cs_d.is_selected());

      std::byte buf[128] = {};
      con_handle.transcieve(nullptr, nullptr, 2);
      con_handle.transcieve(buf, nullptr, 3);
      con_handle.transcieve(nullptr, buf, 5);
      con_handle.transcieve(buf, buf, 7);

      REQUIRE(spi_1.is_enabled());
      REQUIRE_FALSE(spi_2.is_enabled());
      REQUIRE(spi_1.get_byte_count() == b_c{ 20, 24, 34 });
      REQUIRE(spi_2.get_byte_count() == b_c{ 0, 0, 0 });
      REQUIRE_FALSE(cs_a.is_selected());
      REQUIRE(cs_b.is_selected());
      REQUIRE_FALSE(cs_c.is_selected());
      REQUIRE_FALSE(cs_d.is_selected());
    }
  }

  REQUIRE_FALSE(spi_1.is_enabled());
  REQUIRE_FALSE(spi_2.is_enabled());
  REQUIRE(spi_1.get_byte_count() == b_c{ 20, 24, 34 });
  REQUIRE(spi_2.get_byte_count() == b_c{ 0, 0, 0 });
  REQUIRE_FALSE(cs_a.is_selected());
  REQUIRE_FALSE(cs_b.is_selected());
  REQUIRE_FALSE(cs_c.is_selected());
  REQUIRE_FALSE(cs_d.is_selected());

  {
    WLib::SPI::Hardware_handle_t hw_handle(spi_2, cfg);

    REQUIRE_FALSE(spi_1.is_enabled());
    REQUIRE(spi_2.is_enabled());
    REQUIRE(spi_1.get_byte_count() == b_c{ 20, 24, 34 });
    REQUIRE(spi_2.get_byte_count() == b_c{ 0, 0, 0 });
    REQUIRE_FALSE(cs_a.is_selected());
    REQUIRE_FALSE(cs_b.is_selected());
    REQUIRE_FALSE(cs_c.is_selected());
    REQUIRE_FALSE(cs_d.is_selected());

    {
      WLib::SPI::Connection_handle_t con_handle = hw_handle.select(cs_c);

      REQUIRE_FALSE(spi_1.is_enabled());
      REQUIRE(spi_2.is_enabled());
      REQUIRE(spi_1.get_byte_count() == b_c{ 20, 24, 34 });
      REQUIRE(spi_2.get_byte_count() == b_c{ 0, 0, 0 });
      REQUIRE_FALSE(cs_a.is_selected());
      REQUIRE_FALSE(cs_b.is_selected());
      REQUIRE(cs_c.is_selected());
      REQUIRE_FALSE(cs_d.is_selected());

      std::byte buf[128] = {};
      con_handle.transcieve(nullptr, nullptr, 2);
      con_handle.transcieve(buf, nullptr, 3);
      con_handle.transcieve(nullptr, buf, 5);
      con_handle.transcieve(buf, buf, 7);

      REQUIRE_FALSE(spi_1.is_enabled());
      REQUIRE(spi_2.is_enabled());
      REQUIRE(spi_1.get_byte_count() == b_c{ 20, 24, 34 });
      REQUIRE(spi_2.get_byte_count() == b_c{ 10, 12, 17 });
      REQUIRE_FALSE(cs_a.is_selected());
      REQUIRE_FALSE(cs_b.is_selected());
      REQUIRE(cs_c.is_selected());
      REQUIRE_FALSE(cs_d.is_selected());
    }
    REQUIRE_FALSE(spi_1.is_enabled());
    REQUIRE(spi_2.is_enabled());
    REQUIRE(spi_1.get_byte_count() == b_c{ 20, 24, 34 });
    REQUIRE(spi_2.get_byte_count() == b_c{ 10, 12, 17 });
    REQUIRE_FALSE(cs_a.is_selected());
    REQUIRE_FALSE(cs_b.is_selected());
    REQUIRE_FALSE(cs_c.is_selected());
    REQUIRE_FALSE(cs_d.is_selected());
    {
      WLib::SPI::Connection_handle_t con_handle = hw_handle.select(cs_d);

      REQUIRE_FALSE(spi_1.is_enabled());
      REQUIRE(spi_2.is_enabled());
      REQUIRE(spi_1.get_byte_count() == b_c{ 20, 24, 34 });
      REQUIRE(spi_2.get_byte_count() == b_c{ 10, 12, 17 });
      REQUIRE_FALSE(cs_a.is_selected());
      REQUIRE_FALSE(cs_b.is_selected());
      REQUIRE_FALSE(cs_c.is_selected());
      REQUIRE(cs_d.is_selected());

      std::byte buf[128] = {};
      con_handle.transcieve(nullptr, nullptr, 2);
      con_handle.transcieve(buf, nullptr, 3);
      con_handle.transcieve(nullptr, buf, 5);
      con_handle.transcieve(buf, buf, 7);

      REQUIRE_FALSE(spi_1.is_enabled());
      REQUIRE(spi_2.is_enabled());
      REQUIRE(spi_1.get_byte_count() == b_c{ 20, 24, 34 });
      REQUIRE(spi_2.get_byte_count() == b_c{ 20, 24, 34 });
      REQUIRE_FALSE(cs_a.is_selected());
      REQUIRE_FALSE(cs_b.is_selected());
      REQUIRE_FALSE(cs_c.is_selected());
      REQUIRE(cs_d.is_selected());
    }
  }

  REQUIRE_FALSE(spi_1.is_enabled());
  REQUIRE_FALSE(spi_2.is_enabled());
  REQUIRE(spi_1.get_byte_count() == b_c{ 20, 24, 34 });
  REQUIRE(spi_2.get_byte_count() == b_c{ 20, 24, 34 });
  REQUIRE_FALSE(cs_a.is_selected());
  REQUIRE_FALSE(cs_b.is_selected());
  REQUIRE_FALSE(cs_c.is_selected());
  REQUIRE_FALSE(cs_d.is_selected());
}
