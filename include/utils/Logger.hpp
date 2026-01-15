#pragma once

#ifdef DEBUG_MODE
    #include <iostream>
    #include <format>

    #define LOG_SUCCESS(...) std::cout << "[+] " << std::format(__VA_ARGS__) << '\n';
    #define LOG_ERROR(...) std::cout << "[-] " << std::format(__VA_ARGS__) << '\n';
    #define LOG_INFO(...) std::cout << "[*] " << std::format(__VA_ARGS__) << '\n';
    #define LOG_ALERT(...) std::cout << "[!] " << std::format(__VA_ARGS__) << '\n';
#else
    #define LOG_SUCCESS(...)
    #define LOG_ERROR(...)
    #define LOG_INFO(...)
    #define LOG_ALERT(...)
#endif