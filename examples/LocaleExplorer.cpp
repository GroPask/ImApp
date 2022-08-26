#include <ImApp/ImApp.hpp>

#include <algorithm>
#include <iterator>
#include <locale>
#include <string>
#include <vector>

#include <ctime>

namespace
{
    auto addDefaultLocaleAndGetIt(std::vector<std::string>& inOutLocales)
    {
        std::string defaultLocaleName = std::locale{}.name();

        auto defaultLocaleIt = std::find(inOutLocales.begin(), inOutLocales.end(), defaultLocaleName);
        if (defaultLocaleIt != inOutLocales.end())
            return defaultLocaleIt;

        inOutLocales.emplace_back(std::move(defaultLocaleName));

        return std::prev(inOutLocales.end());
    }

    std::string computeCurrentTime()
    {
        const std::time_t now = std::time(nullptr);

#pragma warning(push)
#pragma warning(disable : 4996)
        const std::tm tmNow = *std::localtime(&now);
#pragma warning(pop)

        char timeTextBuffer[256];
        if (std::strftime(timeTextBuffer, sizeof(timeTextBuffer), "%c\n", &tmNow) == 0)
            return {};

        return timeTextBuffer;
    }
}

int main()
{
    std::vector<std::string> locales = { "de_DE", "en_GB", "en_US", "fr_FR", "ja_JP", "kr_KR" };
    auto selectedLocaleIt = addDefaultLocaleAndGetIt(locales);

    return ImApp::Run("ImApp_Example_LocaleExplorer", [&]()
    {
        if (ImApp::BeginMainWindowContent())
        {
            const std::string currentTime = computeCurrentTime();
            ImGui::Text("%s", currentTime.c_str());

            if (ImGui::BeginCombo("Locale", selectedLocaleIt->c_str()))
            {
                for (auto localeIt = locales.begin(); localeIt != locales.end(); ++localeIt)
                {
                    bool currentLocaleIsSelected = (selectedLocaleIt == localeIt);

                    if (ImGui::Selectable(localeIt->c_str(), currentLocaleIsSelected))
                    {
                        selectedLocaleIt = localeIt;
                        std::locale::global(std::locale(*selectedLocaleIt));
                    }

                    if (currentLocaleIsSelected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }
        }

        ImApp::EndMainWindowContent();
    });
}
