list(APPEND ex
    simple
    token
)

foreach(name ${ex})
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} ${PROJECT_NAME})
    # target_include_directories(${name} PUBLIC ../src)
endforeach()