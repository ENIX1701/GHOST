<img src="ghost-ascii.png" alt="GHOST" style="width: 100%;">

# GHOST

C++ implant for [AETHER](https://github.com/ENIX1701/AETHER) deployable to Linux hosts.

## Prerequisites

- `C++20`
- `CMake` (`3.14+`)
- `Make`
- `cpr` and `nlohmann_json` (automatically fetched via *CMake*)

## Build locally

GHOST uses CMake for its build system. The compilation is highly parametrized and enables you to build an implant tailored exactly to your use case. For more information on that, please visit the [build guide](docs/BUILD.md).

```bash
# create and navigate to build directory
mkdir build
cd build

# configure and build
cmake ..
make
```

The resulting executable will be located at `build/bin/Ghost`.

## Deploy

### AETHER (recommended)

In a standard AETHER deployment, GHOST is built and deployed via the C2 infrastructure. For detailed instructions on that please visit [AETHER's README](https://github.com/ENIX1701/AETHER/blob/main/README.md).

### Manual

To deploy a GHOST to the system of your choice (that you, of course, **are authorized** to test), just put the binary created in [build](#build-locally) step onto the Linux host and run it.

Production builds follow the same instructions as the [local builds](#build-locally), but usually use different flags, often to simulate certain scenarios. For more details on compilation flags visit the [build guide](docs/BUILD.md).

## Testing

GHOST includes a CMake test suite to verify it builds correctly with different argument permutations.

```bash
mkdir build
cd build

cmake -DENABLE_TESTING=ON ..
ctest -j4 --output-on-failure   # -j option makes this run in parallel, use as many cores as you want
```

## Architecture

GHOST is a modular agent. It's easy to extend. For detailed information on its architecture, please visit the [architecture deep-dive](docs/ARCHITECTURE.md). Check out [code guidelines](docs/CODE_GUIDELINES.md) if you're more interested in implementation details.

## Roadmap

- [ ] Fully implement `IModule->reverse()` for automated artifact cleanup
- [ ] Further parametrize payloads
- [ ] Implement at least 3 distinct techniques in each tactic:
    - [x] Persistence
    - [ ] Discovery
    - [x] Collection (gathering)
    - [ ] Impact
- [ ] Track tactics as *Mitre ATT&CK* codes (as build parameters for example)
- [ ] Threat-actor inspired testing scenarios -> "scenario mode" being implemented right now :3
- [x] Impact severity level configuration
- [ ] Basic EDR/AV evasion by XOR string obfuscation

### Scenario mode scratchpad

Scenario mode would be one of the most valuable things in AETHER. It would not only help experienced red team operators carry out security assesments and exercises, but also open the door to less technically inclined to measure the performance of implemented security measures! It will (ideally) work as follows:
1. User picks out a threat actor they'd want to simulate
2. They configure the accepted impact level (described [here](docs/BUILD.md#impact-severity))
3. They run the simulation manually (CHARON prepares the command, they put it into the terminal on tested machine)
4. The implant is configured to mimic the kill chain of selected malware with configured impact level (if you want the real deal you'd select USER or SYSTEM)
5. (Also please note that the implant doesn't do privilege escalation currently, and it's not something too high up on the priorities list, so ymmv with how closely the simulation will be to the real thing)

Implementation-wise, it will probably look something like this:
- central scenario manager, triggered inside of `Ghost.cpp`
- scenario manager only present when compiled with some `SCENARIO_MODE` or similar flag (compile time ftw)
- uses the currently implemented (told you the refactor would be worth it!) `IModule` interface with modules map to store the entire killchain
- easiest scenarios to implement and test on are the typical ransomware and espionage with auto cleanup and stuff :3
- [ ] don't forget to create documentation with for scenarios with overview, "noise" level and such!!! (I will forget anyways ughhh)
- another important thing: scenario mode should automatically override required modules (I think that'd make the most sense - if you want a scenario, you choose the impact level and get all the functionality, which means no debugging why stuff doesn't work!)

## Contributing

You're so cool :3 Head over to the [architecture](docs/ARCHITECTURE.md) and [code guidelines](docs/CODE_GUIDELINES.md) sections to get started! There are instructions on how to implement new modules and methods. When you're done, create a pull request and we'll polish your work and merge it into the main branch!

## Legal

> [!IMPORTANT]
> This software is for educational purposes and authorized red team engagements only. The authors are not responsible for misuse.

---

Special thanks to [awesome-readme](https://github.com/matiassingers/awesome-readme) for README ideas and to [readme.so](https://readme.so/) for helping me make this one coherent at all :3
