add_executable (SeedingTests SeedingTests.cpp)
target_include_directories (SeedingTests PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries (SeedingTests PRIVATE ActsCore ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test (NAME SeedingUnitTest COMMAND SeedingTests)
acts_add_test_to_cdash_project (PROJECT ACore TEST SeedingUnitTest TARGETS SeedingTests)

add_executable (SeedingToolsTests SeedingToolsTests.cpp)
target_include_directories (SeedingToolsTests PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries (SeedingToolsTests PRIVATE ActsCore ActsTestsCommonHelpers ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test (NAME SeedingToolsUnitTest COMMAND SeedingToolsTests)
acts_add_test_to_cdash_project (PROJECT ACore TEST SeedingToolsUnitTest TARGETS SeedingToolsTests)
