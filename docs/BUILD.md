# Build

## Basic build

This builds the agent with the default flags (`DEBUG`, `PERSISTENCE` (RunControl), `IMPACT` (Encryption), `EXFIL` (Http)).

```bash
mkdir build
cd build

cmake ..
make
```

* **Result**: The executable will be at `build/bin/Ghost`.

## Parametrized build

You can turn features ON or OFF using the `-D` flag during the `cmake` step.

**Example: stealth**
Disable debug logs and noisy encryption module, but enable Systemd persistence.

```bash
cmake -DENABLE_DEBUG=OFF -DIMPACT_ENCRYPT=OFF -DPERSIST_SERVICE=ON ..
make
```

**Example: wiper**

Simulate typical wiper activity, with no persistence or exfiltration.

```bash
cmake -DIMPACT_WIPE=ON -DENABLE_PERSISTENCE=OFF ..
make
```

## Available flags

Flags you can control:

| Option | Default | Description |
| --- | --- | --- |
| **Global** |  |  |
| `ENABLE_DEBUG` | `OFF` | Enables console outpu |
| **Persistence** |  |  |
| `ENABLE_PERSISTENCE` | `ON` | Master switch for the *Persistence* module |
| `PERSIST_RUNCONTROL` | `ON` | Infects `.bashrc`, `.zshrc`, etc. |
| `PERSIST_SERVICE` | `OFF` | Installs a user-level Systemd service |
| `PERSIST_CRON` | `OFF` | Installs a cron job |
| **Impact** |  |  |
| `ENABLE_IMPACT` | `ON` | Master switch for the *Impact* module |
| `IMPACT_ENCRYPT` | `ON` | Encrypts files in the home directory |
| `IMPACT_WIPE` | `OFF` | Wipes drive |
| `ENCRYPTION_ALGO` | `XOR` | Encryption algorigthm, more details [here](FUNCTIONALITY.md)
| **Exfiltration** |  |  |
| `ENABLE_EXFIL` | `ON` | Master switch for the *Exfiltration* module |
| `EXFIL_HTTP` | `ON` | Exfiltrates data via HTTP POST |
| `EXFIL_DNS` | `OFF` | Exfiltrates data via DNS |
