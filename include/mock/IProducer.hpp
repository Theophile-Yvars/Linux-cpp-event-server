#pragma once

class IProducer {
public:
    virtual ~IProducer() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
};