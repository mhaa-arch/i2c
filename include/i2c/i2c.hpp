#ifndef MHAA_I2C_HPP
#define MHAA_I2C_HPP

#include <stdint.h>
#include <stddef.h>
#include <i2c/controller.hpp>
#include <i2c/types.hpp>

namespace mhaa
{
    namespace i2c
    {
        /** @brief Controller-mode reads, writes and combined transactions on one borrowed bus. */
        class I2C
        {
        public:
            explicit I2C(Controller &controller) : controller_(controller) {}

            /** @brief Requests a supported, nonzero nominal bus rate. */
            bool set_frequency_hz(uint32_t frequency_hz)
            {
                return frequency_hz != 0 && controller_.set_frequency_hz(frequency_hz);
            }

            Result write(uint16_t address, const uint8_t *data, size_t size, uint32_t timeout_ms)
            {
                return write_then_read({address, data, size, nullptr, 0, timeout_ms});
            }

            Result read(uint16_t address, uint8_t *data, size_t size, uint32_t timeout_ms)
            {
                return write_then_read({address, nullptr, 0, data, size, timeout_ms});
            }

            /** @brief Submits a validated transaction and preserves status and byte counts. */
            Result write_then_read(const Transaction &transaction)
            {
                if (transaction.address > 0x7F || transaction.timeout_ms == 0 ||
                    (transaction.write_size && !transaction.write_data) ||
                    (transaction.read_size && !transaction.read_data))
                {
                    return {Status::InvalidArgument, 0, 0};
                }
                if (!transaction.write_size && !transaction.read_size)
                {
                    return {Status::Success, 0, 0};
                }
                return controller_.transact(transaction);
            }

        private:
            Controller &controller_;
        };
    }
}

#endif
