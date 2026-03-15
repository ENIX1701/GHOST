cmake_minimum_required(VERSION 3.14)

function(add_build_test NAME FLAGS)
    set(TEST_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/build_${NAME}")

    add_test(
        NAME "Build_${NAME}"
        COMMAND ${CMAKE_COMMAND}
            -E env
            ${CMAKE_COMMAND}
            -S "${CMAKE_SOURCE_DIR}"
            -B "${TEST_BUILD_DIR}"
            -G "${CMAKE_GENERATOR}"
            ${FLAGS}
            -DENABLE_TESTING=OFF
    )

    add_test(
        NAME "Compile_${NAME}"
        COMMAND ${CMAKE_COMMAND} --build "${TEST_BUILD_DIR}" -- -j2
    )

    set_tests_properties("Compile_${NAME}" PROPERTIES DEPENDS "Build_${NAME}")
endfunction()

# PERMUTATIONS
add_build_test("All_Features" "")

add_build_test("Minimal" "-DENABLE_DEBUG=OFF -DENABLE_PERSISTENCE=OFF -DENABLE_IMPACT=OFF -DENABLE_EXFIL=OFF -DENABLE_GATHER=OFF")

# Persistence matrix
foreach(RC IN ITEMS ON OFF)
    foreach(RC_ONCE IN ITEMS ON OFF)
        foreach(SVC IN ITEMS ON OFF)
            foreach(CRON IN ITEMS ON OFF)
                set(NAME "Persist_RC-${RC}_ONCE-${RC_ONCE}_SVC-${SVC}_CRON-${CRON}")
                set(FLAGS "-DENABLE_PERSISTENCE=ON -DPERSIST_RUNCONTROL=${RC} -DPERSIST_RC_ONCE=${RC_ONCE} -DPERSIST_SERVICE=${SVC} -DPERSIST_CRON=${CRON}")
                add_build_test("${NAME}" "${FLAGS}")
            endforeach()
        endforeach()
    endforeach()
endforeach()

# Impact matrix
foreach(LEVEL IN ITEMS TEST USER SYSTEM)
    foreach(ALGO IN ITEMS XOR AES CHACHA)
        set(NAME "Impact_Encrypt_${ALGO}_Level_${LEVEL}")
        set(FLAGS "-DENABLE_IMPACT=ON -DIMPACT_ENCRYPT=ON -DENCRYPTION_ALGO=${ALGO} -DIMPACT_LEVEL=${LEVEL}")
        add_build_test("${NAME}" "${FLAGS}")
    endforeach()

    set(WIPE_NAME "Impact_Wipe_Level_${LEVEL}")
    set(WIPE_FLAGS "-DENABLE_IMPACT=ON -DIMPACT_ENCRYPT=OFF -DIMPACT_WIPE=ON -DIMPACT_LEVEL=${LEVEL}")
    add_build_test("${WIPE_NAME}" "${WIPE_FLAGS}")
endforeach()

# Exfiltration permutations
add_build_test("Exfil_HTTP" "-DENABLE_EXFIL=ON -DEXFIL_HTTP=ON -DEXFIL_DNS=OFF")
add_build_test("Exfil_DNS" "-DENABLE_EXFIL=ON -DEXFIL_HTTP=OFF -DEXFIL_DNS=ON")

# Gathering permutations
add_build_test("Gather_Sysinfo" "-DENABLE_GATHER=ON -DMETHOD_SYSINFO=ON")
add_build_test("Gather_SSH" "-DENABLE_GATHER=ON -DMETHOD_SSH=ON")
add_build_test("Gather_EtcPasswd" "-DENABLE_GATHER=ON -DMETHOD_ETCPASSWD=ON")
add_build_test("Gather_EtcShadow" "-DENABLE_GATHER=ON -DMETHOD_ETCSHADOW=ON")

# Scenarios
add_build_test("Scenario_Ransomware" "-DSCENARIO_MODE=RANSOMWARE")
add_build_test("Scenario_Espionage" "-DSCENARIO_MODE=ESPIONAGE")
add_build_test("Scenario_Wiper" "-DSCENARIO_MODE=WIPER")
add_build_test("Scenario_Infostealer" "-DSCENARIO_MODE=INFOSTEALER")
add_build_test("Scenario_Apt" "-DSCENARIO_MODE=APT")
add_build_test("Scenario_Apt29" "-DSCENARIO_MODE=APT29")
add_build_test("Scenario_Apt44" "-DSCENARIO_MODE=APT44")
add_build_test("Scenario_Apt38" "-DSCENARIO_MODE=APT38")
