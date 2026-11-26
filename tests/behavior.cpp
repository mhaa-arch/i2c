#include <i2c/i2c.hpp>
#include "../examples/fake.hpp"

#include <cassert>

using namespace mhaa::i2c;
using namespace example;

int main()
{
    Fake controller;
    I2C bus(controller);
    uint8_t input[] = {99, 99};
    const uint8_t output = 0x20;
    assert(bus.read(0x142, input, 2, 10).status == Status::InvalidArgument);
    assert(bus.write(0x42, &output, 1, 0).status == Status::InvalidArgument);
    assert(bus.read(0x42, nullptr, 1, 10).status == Status::InvalidArgument);
    assert(bus.read(0x42, nullptr, 0, 10).status == Status::Success);
    assert(controller.calls == 0);
    assert(bus.write(0x22, &output, 1, 10).status == Status::AddressNack);
    controller.forced = Status::Timeout;
    const Transaction transaction{0x42, &output, 1, input, 2, 10};
    const Result partial = bus.write_then_read(transaction);
    assert(partial.status == Status::Timeout && partial.written == 1 && partial.read == 1);
    assert(controller.calls == 2 && controller.combined && input[0] == 10 && input[1] == 99);
    controller.forced = Status::ArbitrationLost;
    const Result lost = bus.write_then_read(transaction);
    assert(lost.status == Status::ArbitrationLost && lost.read == 0 && lost.written == 0);
    Fake smaller(1);
    I2C limited(smaller);
    assert(limited.write_then_read(transaction).status == Status::Unsupported);

    assert(!bus.set_frequency_hz(0));
    assert(!bus.set_frequency_hz(1234));
    assert(bus.set_frequency_hz(100000));
}
