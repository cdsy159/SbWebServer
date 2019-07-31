#pragma once

class noncopyable
{
public:
    noncopyable() {}
    ~noncopyable() {}

private:
    noncopyable(noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};

