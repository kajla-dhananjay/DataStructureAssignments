#include <iostream>

int main() {
#ifdef _LIBCPP_VERSION
    std::cout << "Using libc++ (LLVM's implementation), version: " << _LIBCPP_VERSION << "\n";
#elif defined(__GLIBCXX__)
    std::cout << "Using libstdc++ (GNU's implementation)\n";
#else
    std::cout << "Unknown standard library\n";
#endif
    return 0;
}
