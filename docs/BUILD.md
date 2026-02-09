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

### Impact severity

To prevent accidental damage during testing (and development...), GHOST contains the `IMPACT_LEVEL` build flag. This controls the scope and targets of most modules to ensure benign behavior locally, while potentially simulating actual destructive behavior in sandboxed EDR/AV tests.

#### `TEST` (default)

The safe mode. Boring and predictable.

**Persistence:** `~/.ghost_rc` instead of real shell configs  
**Impact:** Encrypts and decrypts a single dummy file (`~/.ghost_test`)  

#### `USER`

Destructive, but contained to typical user-mode mechanisms and data.

**Persistence:** User-level configs specified [here](FUNCTIONALITY.md#1-run-control-rc-files)  
**Impact:** Contained within current user's home directory  

#### `SYSTEM`

Oof...

**Persistence:** Same as in [`USER`](#user) for now  
**Impact:** Targets system directories. Can potentially completely wipe a system and make it unusable (and potentially unrecoverable!)  
