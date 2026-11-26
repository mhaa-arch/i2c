#include <i2c/i2c.hpp>
#include "fake.hpp"

using namespace mhaa::i2c;
using namespace example;

int main()
{
    Fake controller;
    I2C bus(controller);
    if (!bus.set_frequency_hz(100000))
    {
        return 1;
    }
    const uint8_t command = 0x20;
    uint8_t reply = 0;
    const Result result = bus.write_then_read({0x42, &command, 1, &reply, 1, 10});
    return result.status == Status::Success && reply == 10 ? 0 : 1;
}
