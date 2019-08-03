#pragma once
#ifndef _NON_COPYABLE_H_
#define _NON_COPYABLE_H_ 
class noncopyable
{
public:
    noncopyable() {}
    ~noncopyable() {}

private:
    noncopyable(noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};
#endif
