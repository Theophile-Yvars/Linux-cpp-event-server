# GoogleTest
add_subdirectory(googletest)

# Activer les tests
enable_testing()

# Exécutable de test
add_executable(runTests
    RuleEngineTest.cpp
)

# Inclure les headers du projet
target_include_directories(runTests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Lier GoogleTest
target_link_libraries(runTests
    eventserver
    gtest
    gtest_main
)

# Découvrir automatiquement les tests
include(GoogleTest)
gtest_discover_tests(runTests)