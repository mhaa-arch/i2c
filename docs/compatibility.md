# Using I2C across targets

The only project dependency is the matching `i2c-contract`. Keep its API revision aligned with this Core.

Combined reads can be used across compatible controller implementations without duplicating protocol phases in application code. Check the target integration for available rates, payload limits, timing and how to recover an idle bus. Ten-bit addressing and target mode are outside this interface.

This repository's host simulations and shared API compile/link checks provide source-level evidence. Target timing and physical behavior require hardware validation.
