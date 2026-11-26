#ifndef MHAA_I2C_EXAMPLE_FAKE_HPP
#define MHAA_I2C_EXAMPLE_FAKE_HPP

#include <i2c/controller.hpp>

namespace example
{
    using namespace mhaa::i2c;

    class Fake final : public Controller
    {
    public:
        explicit Fake(size_t max_read = 3) : max_read_(max_read) {}
        bool set_frequency_hz(uint32_t frequency) override { return frequency == 100000; }
        Result transact(const Transaction &transaction) override
        {
            ++calls;
            combined = transaction.write_size && transaction.read_size;
            if (transaction.address != 0x42)
            {
                return {Status::AddressNack, 0, 0};
            }
            if (transaction.read_size > max_read_)
            {
                return {Status::Unsupported, 0, 0};
            }
            if (forced == Status::ArbitrationLost)
            {
                return {forced, 0, 0};
            }
            const size_t completed = forced == Status::Timeout && transaction.read_size
                                         ? 1
                                         : transaction.read_size;
            for (size_t i = 0; i < completed; ++i)
            {
                transaction.read_data[i] = static_cast<uint8_t>(10 + i);
            }
            return {forced, transaction.write_size, completed};
        }

        unsigned calls = 0;
        bool combined = false;
        Status forced = Status::Success;

    private:
        size_t max_read_;
    };
}

#endif
