include_directories ("${PROJECT_SOURCE_DIR}/ac/include")
include_directories ("${PROJECT_SOURCE_DIR}/csv")
include_directories ("${PROJECT_SOURCE_DIR}/fir")

set (CSV_TEST_LIBS csv)
set (FIR_TB_LIBS csv fir)

add_executable(csv_test csv_test.cpp)
target_link_libraries (csv_test ${CSV_TEST_LIBS})

add_executable(fir_filter_testbench fir_filter_testbench.cpp)
target_link_libraries (fir_filter_testbench ${FIR_TB_LIBS})
