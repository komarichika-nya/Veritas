#pragma once
template<typename T>
struct Pixel{
    T rgb[3]{0,0,0};
    T sum{0};
    std::atomic<T>splatXYZ[3];
    T splatSum;
    T albedo[3]{0,0,0};
    T normal[3]{0,0,0};
    T aov[3]{0,0,0};
};
