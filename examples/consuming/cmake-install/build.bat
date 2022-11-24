set libDir=%~dp0/third-party/
set installDir=%libDir%/install

git clone --depth 1 --branch 0.0.1 https://github.com/GroPask/ImApp.git %libDir%/ImAppSrc

cmake -G "Visual Studio 16 2019" -DCMAKE_INSTALL_PREFIX=%installDir% -DIMAPP_BUILD_EXAMPLES=OFF -DIMAPP_BUILD_TESTS=OFF -S %libDir%/ImAppSrc -B %libDir%/ImAppBuild
cmake --build %libDir%/ImAppBuild --config Release
cmake --install %libDir%/ImAppBuild --config Release

cmake -G "Visual Studio 16 2019" -DCMAKE_PREFIX_PATH=%installDir% -S %~dp0 -B %~dp0/build
cmake --build %~dp0/build --config Release

pause