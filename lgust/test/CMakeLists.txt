add_executable(langust_test
        src/tests_main.cpp
        src/InputTests.cpp
        src/LexerTests.cpp
        src/ParserTableTests.cpp
        src/ProductionTableTests.cpp)

target_link_libraries(langust_test langust)
