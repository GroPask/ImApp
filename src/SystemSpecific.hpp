#ifndef IMAPP_SYSTEM_SPECIFIC_HPP
#define IMAPP_SYSTEM_SPECIFIC_HPP

struct GLFWwindow;

namespace ImApp
{
	void HideCloseButton(GLFWwindow& window) noexcept;
	void ShowCloseButton(GLFWwindow& window) noexcept;
}

#endif
