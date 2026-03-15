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
| **Gathering** |  |  |
| `ENABLE_GATHER` | `ON` | Master switch for the *Gathering* module |
| `GATHER_SYSINFO` | `ON` | Gathers basic system info |
| `GATHER_SSH` | `OFF` | Extracts SSH keys and configs |
| `GATHER_ETCPASSWD` | `OFF` | Extracts `/etc/passwd` |
| `GATHER_ETCSHADOW` | `OFF` | Extracts `/etc/shadow` |

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

## Scenario mode

Gets enabled if setting any scenario with `-DSCENARIO_MODE=<name>` flag. Available scenarios are listed in the table below :3

> [!NOTE]
> All scenarios use the built-in methods, which respect severity level flags. If you want to run a scenario, but are afraid of damaging your system, check out the [*TEST* severity](#test-default).

| Option | Description |
|-|-|
|`RANSOMWARE`|Very noisy. Exfiltrates data and encrypts files.|
|`ESPIONAGE`|Low noise. Gathers host info (sensitive data included!) and exfiltrates it over the C2 channel. Good starting point for simulating [infostealer malware](https://en.wikipedia.org/wiki/Infostealer) or [APT actors](https://www.ibm.com/think/topics/advanced-persistent-threats) :3 |
|`WIPER`|Extreme noise. Destroys all files and self-terminates, leaving the system in an unusable state. Used to test EDR detection and reaction speed.|
|`INFOSTEALER`|Steals the info >:3c|
|`APT`|Stealhy and hands-on-keyboard'y... Also - very generic|
|`APT29`|Long-term espionage. Gathers SSH keys, stablishes RunControl persistence and deploys an undetectable 4-hour beacon interval.|
|`APT44 (SANDWORM)`|Pure chaos. Establishes a CRON job to survive reboots and immediately wipes the system. Nothing will survive.|
|`APT38 (LAZARUS)`|Two stage extortion. Silent harvesting of system data and SSH keys only to drop a screaming ransomware to mask the espionage.|
