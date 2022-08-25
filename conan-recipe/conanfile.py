from os import path
from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout
from conan.tools.files import get

class ImAppConan(ConanFile):
    name = "imapp"
    version = "0.0.1"

    # Optional metadata
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of Imapp here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    generators = "CMakeDeps", "CMakeToolchain"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def requirements(self):
        self.requires("glfw/3.3.6")
        self.requires("imgui/cci.20220621+1.88.docking")
        self.requires("opengl/system")

    def imports(self):
        self.copy("*", path.join("build", "ImGuiBackendsImpl"), path.join(self.deps_cpp_info["imgui"].resdirs[0], "bindings"), root_package="imgui")

    def source(self):
        get(self, **self.conan_data["sources"][str(self.version)], strip_root=True)

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cache_entries = {
            "IMAPP_BUILD_EXAMPLES": "False",
            "IMAPP_BUILD_TESTS": "False"
        }
        cmake.configure(variables=cache_entries)
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["ImApp"]
        self.cpp_info.set_property("cmake_file_name", "ImApp")
        self.cpp_info.set_property("cmake_target_name", "ImApp::ImApp")
