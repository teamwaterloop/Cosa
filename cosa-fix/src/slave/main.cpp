#include "Cosa/TWI.hh"
#include "Cosa/OutputPin.hh"
#include "Cosa/Watchdog.hh"
#include "Cosa/Trace.hh"
// #include "Cosa/IOStream/Driver/UART.hh"
#include "Cosa/Memory.h"

bool hello = false;

// A simple TWI slave device
class Echo : public TWI::Slave {
private:
    // Buffer for request and response
    static const uint8_t BUF_MAX = 8;
    uint8_t m_buf[BUF_MAX];

public:
    // Construct the echo slave device
    Echo() : TWI::Slave(0x5A)
    {
        hello = true;

        write_buf(m_buf, sizeof(m_buf));
        read_buf(m_buf, sizeof(m_buf));
        // write_buf(m_buf, sizeof(m_buf));
        // read_buf(m_buf, sizeof(m_buf));
    }

    // Request handler; events from incoming requests
    virtual void on_request(void* buf, size_t size);
};

void Echo::on_request(void* buf, size_t size)
{
    trace << "I am working" << endl;
    UNUSED(buf);
    UNUSED(size);
    m_buf[0] += 1;
    for (uint8_t i = 1; i < sizeof(m_buf); i++)
        m_buf[i] = m_buf[0] + i;
}

void *operator new(decltype(sizeof(0)), void *ptr) noexcept { return ptr; }

#if defined(BOARD_ATTINYX5)
Soft::UAT uart(Board::D1);
#endif
// The TWI echo device

Echo echo;

void setup()
{
    // Start trace output stream on the serial port
    // uart.begin(9600);
    // trace.begin(&uart, PSTR("CosaTWISlave: started"));

    // Check amount of free memory and size of classes
    TRACE(free_memory());
    TRACE(sizeof(Echo));
    TRACE(sizeof(OutputPin));

    // Start the watchdog ticks counter
    Watchdog::begin();

    // Start the TWI echo device
    echo.begin();
}

void loop()
{
    Event::service();
}
