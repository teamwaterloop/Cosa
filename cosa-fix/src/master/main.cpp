#include "Cosa/TWI.hh"
#include "Cosa/OutputPin.hh"
#include "Cosa/Watchdog.hh"
#include "Cosa/RTT.hh"
#include "Cosa/Trace.hh"
#include "Cosa/UART.hh"
#include "Cosa/Memory.h"

// TWI slave address
static const uint8_t ADDR = 0x5A;
TWI::Driver dev(ADDR);

// Use the builtin led for a heartbeat
OutputPin ledPin(Board::LED);

void setup()
{
    // Start trace output stream on the serial port
    uart.begin(9600);
    trace.begin(&uart, PSTR("CosaTWImaster: started"));

    // Check amount of free memory and size of instances
    TRACE(free_memory());
    TRACE(sizeof(OutputPin));
    TRACE(sizeof(TWI));

    // Start the watchdog ticks counter
    Watchdog::begin();
    RTT::begin();
}

void loop()
{
    // Write a command to the slave
    static uint16_t cmd = 0;
    uint32_t time;
    int count;

    ledPin.toggle();
    time = RTT::micros();
    twi.acquire(&dev);
    count = twi.write(&cmd, sizeof(cmd));
    twi.release();
    time = RTT::micros() - time;
    trace << PSTR("write(2):") << time << ':' << time/sizeof(cmd) << ':';
    if (count > 0) trace.print(&cmd, count);

    // Read back the result
    uint8_t buf[8];

    time = RTT::micros();
    twi.acquire(&dev);
    do {
        count = twi.read(buf, sizeof(buf));
    } while (count < 0);
    twi.release();
    time = RTT::micros() - time;
    trace << PSTR("read(8):") << time << ':' << time/sizeof(buf) << ':';
    if (count > 0) trace.print(buf, count);

    time  = RTT::micros();
    twi.acquire(&dev);
    do {
        count = twi.read(buf, sizeof(buf) - 4);
    } while (count < 0);
    twi.release();
    time = RTT::micros() - time;
    trace << PSTR("read(4):") << time << ':' << time/(sizeof(buf) - 4) << ':';
    if (count > 0) trace.print(buf, count);
    ledPin.toggle();
    trace << endl;

    // Next transaction
    cmd += 1;
    sleep(2);
}
