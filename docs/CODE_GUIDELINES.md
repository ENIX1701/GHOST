# Code guidelines

## General standards

**Language standard**: `C++20`
**Indentation**: 4 spaces
**Line endings**: LF (Unix style)
**File encoding**: `UTF-8`

## Naming conventions

Consistency is key. I like keys :3

| Type                      | Convention                    | Example                           |
|---------------------------|-------------------------------|-----------------------------------|
| **Class**                 | PascalCase                    | `Ghost`, `SystemUtils`            |
| **Interface**             | PascalCase with `I` prefix    | `IModule`, `IPersistenceMethod`   |
| **Instance method**       | camelCase                     | `execute()`, `getName()`          |
| **Static util methods**   | PascalCase                    | `ExecuteCommand()`, `ReadFile()`  |
| **Variables**             | camelCase                     | `sleepInterval`, `targetDir`      |
| **Constants**             | SCREAMING_SNAKE_CASE          | `SHADOW_URL`, `DEFAULT_SLEEP_SEC` |
| **Macros**                | SCREAMING_SNAKE_CASE          | `DEBUG_MODE`, `LOG_INFO`          |
| **Namespaces**            | PascalCase                    | `Config`                          |


## Project structure

Refer to [architecture overview](ARCHITECTURE.md#directory-structure).

## Implementation details

### *Modules* and *Methods*

GHOST relies on a strict manager/worker pattern. For a high-level architecture overview consult [ARCHITECTURE.md](ARCHITECTURE.md#module-system).

#### Inheritance

*Managers* (like `Persistence`) must implement `IModule`.
*Methods* (like `RunControlMethod`) must implement their specific interface (`IPersistenceMethod` in this case).

#### Memory management

Use `std::unique_ptr` for managing the lifecycle of methods within a module. Please don't use raw pointers. Please...

#### Error handling

Functions should generally return `bool` to indicate success or failure. Exceptions should be caught locally and handled as silently as possible. They should be logged using `LOG_ERROR`.

### Configuration

All configurable constants must be defined in `include/core/Config.hpp`.

> [!CAUTION]
> Do not hardcode magic strings or configuration values inside implementation files.

> [!IMPORTANT]
> Values that are crucial for the functionality, but relevant only locally (like files list in `RunControl`), are acceptable, but highly discouraged.

### CMake and build system

New features must be optional and controllable via CMake flags for reasons outlined [here](ARCHITECTURE.md#build-system).

## Dependencies

### JSON

The project uses [`nlohmann/json`](https://github.com/nlohmann/json) for data serialization. 

### Networking

Handled by [`cpr`](https://github.com/libcpr/cpr). Always use the `Comms` wrapper class to ensure consistency (User-Agent headers, etc.).

### Utilities

Use the internal utility classes outlined [here](ARCHITECTURE.md#utilities). 

## Terminal output

Terminal output should be structured in accordance to the below guidelines. The output should only be visible in `debug` builds (`ENABLE_DEBUG` flag). Use wrapper macros provided in `include/utils/Logger.hpp` instead of raw `std::cout` or `printf`.

| Symbol    | Meaning   | Macro                 | Usage case examples                                                           |
|-----------|-----------|-----------------------|-------------------------------------------------------------------------------|
| `[+]`     | Success   | `LOG_SUCCESS(...)`    | Task completed, persistence established, file downloaded                      |
| `[-]`     | Error     | `LOG_ERROR(...)`      | Connection lost, persmission denied, file not found, server not responding    |
| `[*]`     | Info      | `LOG_INFO(...)`       | SHADOW connection, sleep time, beacon info                                    |
| `[!]`     | Alert     | `LOG_ALERT(...)`      | Hex dumps, raw payloads, critical errors                                      |
