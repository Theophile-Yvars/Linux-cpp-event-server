#include "engine/RuleEngine.hpp"
#include "event/Event.hpp"
#include "event/EventType.hpp"
#include <gtest/gtest.h>
#include <sstream>
#include <iostream>

class RuleEngineTest : public ::testing::Test {
protected:
    RuleEngine engine;

    // Pour capturer la sortie de std::cout
    std::stringstream buffer;
    std::streambuf* oldCout;

    void SetUp() override {
        oldCout = std::cout.rdbuf(buffer.rdbuf());
    }

    void TearDown() override {
        std::cout.rdbuf(oldCout); // Restaurer cout
    }

    std::string getOutput() {
        std::string out = buffer.str();
        buffer.str(""); buffer.clear();
        return out;
    }
};

TEST_F(RuleEngineTest, CpuOverloadCritical) {
    Event e{EventType::CPU_OVERLOAD, 0.2, std::chrono::system_clock::now()};
    engine.process(e);
    std::string output = getOutput();
    EXPECT_NE(output.find("CRITICAL CPU overload"), std::string::npos);
}

TEST_F(RuleEngineTest, CpuOverloadHigh) {
    Event e{EventType::CPU_OVERLOAD, 0.1, std::chrono::system_clock::now()};
    engine.process(e);
    std::string output = getOutput();
    EXPECT_NE(output.find("High CPU"), std::string::npos);
}

TEST_F(RuleEngineTest, GoEvents) {
    Event go1{EventType::GO1, 42, std::chrono::system_clock::now()};
    engine.process(go1);
    EXPECT_NE(getOutput().find("Received event GO1 with value 42"), std::string::npos);

    Event go2{EventType::GO2, 55, std::chrono::system_clock::now()};
    engine.process(go2);
    EXPECT_NE(getOutput().find("Received event GO2 with value 55"), std::string::npos);

    Event go3{EventType::GO3, 77, std::chrono::system_clock::now()};
    engine.process(go3);
    EXPECT_NE(getOutput().find("Received event GO3 with value 77"), std::string::npos);

    Event go4{EventType::GO4, 99, std::chrono::system_clock::now()};
    engine.process(go4);
    EXPECT_NE(getOutput().find("Received event GO4 with value 99"), std::string::npos);
}