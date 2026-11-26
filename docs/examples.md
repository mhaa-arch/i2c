# Run the I2C Core example

The Core borrows a fake, checks 100 kHz, and sends command `0x20` to unshifted address `0x42`. The single `write_then_read` call requests a one-byte reply under a 10 ms budget. The fake returns 10 as the byte value. The behavior test also exercises partial timeout, arbitration loss and a replacement Adapter with a smaller receive limit.

## Example program

The [program](../examples/example.cpp) uses the [local fake](../examples/fake.hpp) to demonstrate injection. A concrete Adapter implementing the same Contract can be supplied to the same facade.

```cpp
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
```

## Build and run

Build from this repository root. Set `contract_include` to the matching Contract's `include/` directory. The value below applies to the MHAA development workspace. Both programs exit with status 0 on success and print no output. On Windows execute `example.exe` and `behavior.exe`.

```sh
contract_include=../../contracts/i2c-contract/include
g++ -std=c++11 -Wall -Wextra -Wpedantic -Werror -fno-rtti -fno-exceptions -Iinclude -I"$contract_include" examples/example.cpp -o example
./example
g++ -std=c++11 -Wall -Wextra -Wpedantic -Werror -fno-rtti -fno-exceptions -Iinclude -I"$contract_include" tests/behavior.cpp -o behavior
./behavior
```

See the [behavior test](../tests/behavior.cpp) for the additional cases described above.
