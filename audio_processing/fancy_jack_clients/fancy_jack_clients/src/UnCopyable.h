#pragma once

class UnCopyable {
protected:
    UnCopyable() {}
    ~UnCopyable() {}

private:
    UnCopyable(const UnCopyable&);
    UnCopyable& operator=(const UnCopyable&);
};
