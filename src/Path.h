#pragma once
#include <filesystem>
#include <string>

#ifdef __APPLE__
    #define ROOT std::string("/Users/asinha/Documents/BreakoutGL/")
#else
    #define ROOT std::string("C:/Projects/BreakoutGL/")
#endif


#define PATH(X) Path(X).c_str()


namespace Bout {
    class Path {
    public:
        Path(std::string subpath) :
            m_filepath{ std::filesystem::path(ROOT + subpath).generic_u8string() } {}

        const char* c_str()
        {
            return m_filepath.c_str();
        }

    private:
        std::string m_filepath;
    };
}
