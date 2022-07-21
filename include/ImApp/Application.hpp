#ifndef IMAPP_APPLICATION_HPP
#define IMAPP_APPLICATION_HPP

#include <functional>
#include <string>

namespace ImApp
{
    class Application
    {
    public:
        Application(int width, int height, const char* title);

        int run(const std::function<void()>& updateFunction);

        void close();

    private:
        int m_width;
        int m_height;

        std::string m_title;

        bool m_shouldClose = false;
    };
}

#endif
