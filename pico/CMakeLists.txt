cmake_minimum_required(VERSION 3.20)

set(PROJECT gcigps)
# set(PICO_BOARD pico) # pico or pico_w
string(TOLOWER "pico" PICO_BOARD)
set(PICO_SDK_PATH "$ENV{HOME}/github/pico-sdk")
set(ENV{PATH} "$ENV{HOME}/github/gcc-arm/bin:$ENV{PATH}")
set(PICO_SDK_PATH "${PICO_SDK_PATH}" CACHE PATH "Path to the PICO SDK")

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

include(FetchContent)

# Need ASM for tinyUSB and picotool to work
project(${PROJECT} C CXX ASM)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

string (APPEND CMAKE_CXX_FLAGS_RELEASE " -O3 ")
string (APPEND CMAKE_C_FLAGS_RELEASE " -O3 ")
string(PREPEND CMAKE_EXE_LINKER_FLAGS " -Wl,--print-memory-usage ")


# Initialize the SDK
pico_sdk_init()

message(STATUS ">> test: ${PICO_SDK}  ${PICO_BOARD} ${ARDUINO}")

if(PICO_SDK)
    add_definitions(-DPICO_SDK=1)

    # these don't seem to work???
    if (${PICO_BOARD} STREQUAL "pico")
        add_definitions(-DPI_PICO=1)
    elseif(${PICO_BOARD} STREQUAL "pico_w")
        add_definitions(-DPI_PICO_W=1)
    endif()
endif()

if(PROJECT_IS_TOP_LEVEL)
    cmake_host_system_information(RESULT HOST QUERY HOSTNAME)
    cmake_host_system_information(RESULT OSN QUERY OS_NAME)
    cmake_host_system_information(RESULT OS_VERSION QUERY OS_RELEASE)
    cmake_host_system_information(RESULT PROC QUERY PROCESSOR_DESCRIPTION)

    message(STATUS "-------------------------------------")
    message(STATUS "  Project: ${PROJECT_NAME}")
    message(STATUS "  Hardware: ${PICO_BOARD}")
    message(STATUS "  C++ ${CMAKE_CXX_STANDARD}")
    message(STATUS "  C ${CMAKE_C_STANDARD}")
    message(STATUS "-------------------------------------")
    message(STATUS " ${HOST}")
    message(STATUS " ${OSN}: ${OS_VERSION}")
    message(STATUS " ${PROC}")
    message(STATUS "-------------------------------------")

    set(BUILD_EXAMPLES ON)
else()
    message(STATUS "-> ${PROJECT_NAME} is submodule")
    set(BUILD_EXAMPLES OFF)
    # set(BUILD_GTESTS OFF)
endif()

LIST(APPEND pico_libs
    pico_stdlib
    hardware_uart)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ../cpp/src)
target_link_libraries(${PROJECT_NAME} INTERFACE ${pico_libs})

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
