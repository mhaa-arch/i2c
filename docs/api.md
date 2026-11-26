# Use the I2C Core

Public entry headers are [i2c/i2c.hpp](../include/i2c/i2c.hpp).

## Send one bus operation

Construct `I2C` from a live `Controller&`. Check `set_frequency_hz` if you need to configure the bus. Use `write` and `read` for single phases and `write_then_read` for a combined transaction. The facade rejects addresses above 0x7F, zero budgets and missing buffers for nonempty phases. It returns success with zero counts for a valid empty request. Other calls reach the Adapter as one transaction.

## Use status and progress together

On `Success`, the transaction closed successfully. For a partial result, only bytes up to `read` contain new input and only bytes up to `written` were acknowledged. Inspect both counts and status before retrying. Earlier writes can already have affected the device. The Core never retries or performs bus recovery.

## Composition and lifetime

Application code calls the I2C Core. It borrows the matching Contract, which an Adapter implements using its Hardware Definitions. Future Devices will create or bind these objects and establish their lifetime. To use the examples today, construct the facade directly with the supplied fake. Keep each borrowed resource alive for its use by the facade and follow the Adapter's access and synchronization requirements.
