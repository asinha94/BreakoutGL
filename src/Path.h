#include <filesystem>
#include <string>

#ifdef __APPLE__
    #define ROOT std::string("/Users/asinha/Documents/BreakoutGL/")
#else
    #define ROOT std::string("C:/Projects/BreakoutGL")
#endif


#define PATH(X) std::filesystem::path(ROOT + std::string((X))).c_str()

