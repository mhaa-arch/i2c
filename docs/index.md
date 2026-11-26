# I2C Core guide

A combined write/read is one transaction with a repeated START. Two separate facade calls have different bus boundaries.

Read the transaction shape and byte-count rules before choosing how to retry an operation.

- [Use the facade](api.md) for construction, results and optional capabilities.
- [Run the example](examples.md) to see the same API with a simulated resource.
- [Check portability](compatibility.md) for assumptions that belong to the target.
