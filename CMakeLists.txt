cmake_minimum_required(VERSION 3.0)
project(fred-test-cli LANGUAGES C)

set(FRED_PATH "/opt/fredsys")

set(CMAKE_C_FLAGS "-std=gnu99 -O2 -Wall -Werror -fpic -DFRED_PATH=${FRED_PATH}")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${FRED_PATH}/include/
)

link_directories(
    ${FRED_PATH}/lib/
)

# check the depedency to libfred
find_library(LIBFRED REQUIRED
    NAMES fred libfred fred-lib fredlib
    HINTS ${FRED_PATH}/lib)
#include_directories(${LIBFRED_INCLUDE_DIR})

add_executable(${CMAKE_PROJECT_NAME} 
    demo_hw_task.c
    periodic_task.c
    main.c
)

# libraries to be linked
target_link_libraries(${CMAKE_PROJECT_NAME} m pthread fred)

INSTALL(TARGETS ${CMAKE_PROJECT_NAME}
        DESTINATION ${FRED_PATH}
        RUNTIME DESTINATION ${FRED_PATH}/examples
)
