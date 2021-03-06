add_executable (ConstantBFieldTests ConstantBFieldTests.cpp)
target_include_directories (ConstantBFieldTests PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries (ConstantBFieldTests PRIVATE ActsCore ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test (NAME ConstantBFieldUnitTest COMMAND ConstantBFieldTests)
acts_add_test_to_cdash_project (PROJECT ACore TEST ConstantBFieldUnitTest TARGETS ConstantBFieldTests)

add_executable (InterpolatedBFieldMapTests InterpolatedBFieldMapTests.cpp)
target_include_directories (InterpolatedBFieldMapTests PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries (InterpolatedBFieldMapTests PRIVATE ActsCore ActsTestsCommonHelpers ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test (NAME InterpolatedBFieldMapUnitTest COMMAND InterpolatedBFieldMapTests)
acts_add_test_to_cdash_project (PROJECT ACore TEST InterpolatedBFieldMapUnitTest TARGETS InterpolatedBFieldMapTests)

add_executable (SolenoidBFieldTests SolenoidBFieldTests.cpp)
target_include_directories (SolenoidBFieldTests PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries (SolenoidBFieldTests PRIVATE ActsCore ActsTestsCommonHelpers ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test (NAME SolenoidBFieldUnitTest COMMAND SolenoidBFieldTests)
acts_add_test_to_cdash_project (PROJECT ACore TEST SolenoidBFieldUnitTest TARGETS SolenoidBFieldTests)

add_executable (SolenoidFieldBenchmark SolenoidFieldBenchmark.cpp)
target_include_directories (SolenoidFieldBenchmark PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries (SolenoidFieldBenchmark PRIVATE ActsCore)

add_executable (MagneticFieldInterfaceConsistencyTests MagneticFieldInterfaceConsistencyTests.cpp)
target_include_directories (MagneticFieldInterfaceConsistencyTests PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries (MagneticFieldInterfaceConsistencyTests PRIVATE ActsCore ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(NAME MagneticFieldInterfaceConsistencyTests COMMAND MagneticFieldInterfaceConsistencyTests)
