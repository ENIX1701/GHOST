# Code guidelines

## Terminal output

Terminal output should be structured in accordance to the below guidelines. The output should only be visible in `debug` builds (`ENABLE_DEBUG` flag). Use wrapper macros provided in `include/utils/Logger.hpp` instead of raw `std::cout` or `printf`.

| Symbol    | Meaning   | Macro                 | Usage case examples                                                           |
|-----------|-----------|-----------------------|-------------------------------------------------------------------------------|
| `[+]`     | Success   | `LOG_SUCCESS(...)`    | Task completed, persistence established, file downloaded                      |
| `[-]`     | Error     | `LOG_ERROR(...)`      | Connection lost, persmission denied, file not found, server not responding    |
| `[*]`     | Info      | `LOG_INFO(...)`       | SHADOW connection, sleep time, beacon info                                    |
| `[!]`     | Alert     | `LOG_ALERT(...)`      | Hex dumps, raw payloads, critical errors                                      |
