# Architecture

GHOST is modular. GHOST is extensible. GHOST is limited only by your imagination.

## High-level overview

The implant operates on a simple lifecycle:
1. **Initialization**: GHOST initializes and generates an id
2. **Mothership call**: GHOST attempts to register itself with [SHADOW](https://github.com/ENIX1701/SHADOW)
3. **Beacon**: After registration is done, GHOST starts to beacon. This includes:
    - **Heartbeat**: Get new tasks and send back results
    - **Execution**: Delegate tasks to appropriate modules, execute them and prepare results
    - **Sleep**: Sleep for a configurable, jittered length of time

## Directory structure

```bash
GHOST/
├── docs/                   # Documentation files
├── include/                # Header files
│   ├── core/               # Core agent logic and config
│   ├── interfaces/         # Abstract base classes
│   ├── modules/            # Module managers
│   │   ├── exfiltration/   # Headers for specific methods
│   │   ├── impact/
│   │   └── persistence/
│   ├── network/            # Communication wrappers
│   └── utils/              # Utilities
└── src/                    # Source files with implementations of header classes
    ├── core/
    ├── modules/
    │   ├── exfiltration/
    │   ├── impact/
    │   └── persistence/ 
    ├── network/ 
    └── utils/ 
```

## Core components

Core components are parts of the system that are not meant to change often (or at all, but sometimes extending the functionality is required to accomplish set objectives). These form the base of what GHOST is and isn't. They pave the ground for everything else.

### Ghost core

**Located at**: `/{src,include}/core/`

`Ghost` class is the central orchestrator. It hold a list of active modules (`std::map<std::string, std::unique_ptr<IModule>>`) and manages the main event loop. It handles task dispatching and lifecycle management (mainly execution and cleanup). 

### Configuration

**Located at**: `/include/core/Config.hpp`

Contains compile-time configuration constants such as the `SHADOW_URL`, user agent and default sleep intervals and jitter. This is the place to put constants used elsewhere in the code. This makes it a lot easier to parametrize builds with.

### Communication

**Located at**: `/{src,include}/network/`

`Comms` class wraps the `cpr` library to handle HTTP/S POST requests. It's stateless and used statically by *Core* and *Modules* to send data.

### Utilities

**Located at**: `/{src,include}/utils/`

- `FileUtils` - helpers for reading/writing/listing files
- `SystemUtils` - OS-level actions, like executing commands and getting home path
- `CryptoUtils` - encryption routines, mainly for `impact/Encryption` functionality
- `Logger` - macros for debug output that are stripped in release builds (for details check [here](CODE_GUIDELINES.md#terminal-output))

## Module system

The architecture relies heavily on polymorphism via interfaces defined in `include/interfaces/`.

### Module pattern

Every major capability of GHOST is a **Module**.

**Interface**: `IModule`
**Responsibility**: A *Module* acts as a **Manager**. It hold a list of *Methods* and decides which one to execute.
**Example**: The `Persistence` module tries to establish all available persistence methods (for example RunControl, Service, Cron) sequentially.

### Method pattern

Specific techniques within a module are **Methods**.

**Interfaces**: `IPersistenceMethod`, `IImpactMethod`, `IExfilMethod`
**Responsibility**: Implements the actual logic for a specific technique (for example editing `.bashrc` or encrypting files)

## Build system

GHOST uses [`CMake`](../CMakeLists.txt) to selectively compile features. This reduces binary size, avoids triggering detection with unused features and changes the signature of the implant.

> [!NOTE]
> CMake flags (like `-DPERSIST_CRON=ON`) are converted to preprocessor definitions (`#define METHOD_CRON`)

> [!NOTE]
> Optional features are guarded by `#ifdef` blocks. If a feature is disabled, its code it entirely excluded from the resulting binary

## Extension guide

### Example: adding a new persistence method (just pretend RunControl is not already done zzz)

**1. Define the class**

Create a header file in `include/modules/persistence/RunControl.hpp`. Remember to inherit from the correct *Method* (`IPersistenceMethod` in this case).

```C++
#pragma once
#include "interfaces/IPersistenceMethod.hpp"

class RunControlMethod : public IPersistenceMethod {
public:
    std::string getName() const override { return "RunControl"; }
    bool install() override;
    bool remove() override;
};
```

**2. Implement the logic**

Create `src/modules/persistence/RunControl.hpp`. Create necessary methods (`bool install()` and `bool remove()` for this one).

> [!TIP]
> Consult [RunControl.cpp](../src/modules/persistence/RunControl.cpp) for full implementation

**3. Register the method**

Add the include and register it in constructor in the correct *Module* (`src/modules/Persistence.cpp` for this one). Remember to guard it with a macro to parametrize the build.

```C++
#ifdef METHOD_RUNCONTROL
    #include "modules/persistence/RunControl.hpp"
#endif
// ... other imports

Persistence::Persistence() {
    #ifdef METHOD_RUNCONTROL
    methods.push_back(std::make_unique<RunControlMethod>());
    #endif
    // ... other methods
}
```

**4. Update the build system**

Navigate to [`CMakeLists.txt`](../CMakeLists.txt) and add the macro name to the list.

```CMake
# ==========================================
# MODULE: PERSISTENCE
# ==========================================
option(ENABLE_PERSISTENCE "Enable Persistence Manager" ON)
if(ENABLE_PERSISTENCE)
    add_compile_definitions(FEATURE_PERSISTENCE)
    
    option(PERSIST_RUNCONTROL "Method: BashRC/ZshRC" ON)    # <- here
    option(PERSIST_SERVICE    "Method: Systemd Service" OFF)
    option(PERSIST_CRON       "Method: Cron Job" OFF)

    if(PERSIST_RUNCONTROL)  # <- and here
        add_compile_definitions(METHOD_RUNCONTROL)
        list(APPEND MODULE_SOURCES src/modules/persistence/RunControl.cpp)
    endif()

    if(PERSIST_SERVICE)
        add_compile_definitions(METHOD_SERVICE)
        list(APPEND MODULE_SOURCES src/modules/persistence/Service.cpp)
    endif()

    if(PERSIST_CRON)
        add_compile_definitions(METHOD_CRON)
        list(APPEND MODULE_SOURCES src/modules/persistence/CronJob.cpp)
    endif()
endif()
```

### Add a completely new Module

Follow the instructions outlined [above](#example-adding-a-new-persistence-method-just-pretend-runcontrol-is-not-already-done-zzz), but with the following steps:
1. Create a header for the class in `include/modules/`. Make sure it extends `IModule`.
2. Add it to the `Ghost` constructor like you would with a *Method*.
3. Add the corresponding feature flag in `CMakeLists.txt`. You can copy an already existing section and adjust as needed. Make sure it's easily extendable (for the future you :3).
