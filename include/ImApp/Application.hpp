#ifndef IMAPP_APPLICATION_HPP
#define IMAPP_APPLICATION_HPP

#include <functional>
#include <string>
#include <string_view>

namespace ImApp
{
    class Application
    {
    public:
        Application(int width, int height, std::string_view title);

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
