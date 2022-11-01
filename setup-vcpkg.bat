@echo off

@rem check working dir
if not exist scripts/vcpkg (
    echo scripts dir not found.
    goto :Lab_Exit
)

echo ================================= Detect Proxy =================================

for /f "tokens=1,2,* " %%i in ('reg query "HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Internet Settings" /v ProxyEnable') do set "proxy_enable=%%k"
for /f "tokens=1,2,* " %%i in ('reg query "HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Internet Settings" /v ProxyServer') do set "proxy_server=%%k"

@rem chang proxy var if system proxy is enabled
if "%proxy_enable%" == "0x1" (
    set http_proxy=http://%proxy_server%
    echo set http_proxy as %proxy_server%
    
    set https_proxy=http://%proxy_server%
    echo set https_proxy as %proxy_server%
) else (
    echo No system proxy detected.
)

@rem check if repository is empty, remove if empty
if exist vcpkg (
    for /F %%i in ('dir /b /a vcpkg') do (
        goto Lab_Pull
    )
    echo vcpkg dir is empty, auto remove
    rd /s /q vcpkg
)

:Lab_Pull

echo.
echo.
echo ================================= Pull vcpkg =================================

git clone https://github.com/microsoft/vcpkg.git

echo.
echo.
echo ================================= Build vcpkg =================================

@rem check if repository is valid
if not exist vcpkg\bootstrap-vcpkg.bat (
    echo bootstrap-vcpkg.bat not found.
    goto :Lab_Exit
)

cd vcpkg
call "bootstrap-vcpkg.bat"

if not exist vcpkg.exe (
    echo vcpkg.exe not found.
    goto :Lab_Exit_And_Up
)

echo.
echo.
echo ================================= Build zlib =================================

vcpkg install zlib:x64-windows

echo.
echo.
echo ================================= Build sdl2 =================================

vcpkg install sdl2:x64-windows

echo.
echo.
echo ================================= Build yaml-cpp =================================

vcpkg install yaml-cpp:x64-windows

echo.
echo.
echo ================================= Build quazip =================================

vcpkg install quazip:x64-windows --overlay-ports=../scripts/vcpkg/ports --overlay-triplets=../scripts/vcpkg/triplets

echo.
echo.
echo ================================= Build framelesshelper =================================

vcpkg install framelesshelper:x64-windows --overlay-ports=../scripts/vcpkg/ports --overlay-triplets=../scripts/vcpkg/triplets

echo.
echo.
echo ================================= Build ffmpeg =================================

@REM vcpkg install ffmpeg[core,avcodec,avdevice,avfilter,avformat,swresample,swscale,ffmpeg,ffprobe]:x64-windows --recurse
vcpkg install ffmpeg-fake:x64-windows --overlay-ports=../scripts/vcpkg/ports

echo.
echo.
echo ================================= Clean Up =================================

echo Remove Build Trees
rd /s /q buildtrees

echo Remove Downloads
rd /s /q downloads

echo Remove Packages
rd /s /q packages

@REM echo Remove Caches
@REM rd /s /q %LOCALAPPDATA%\vcpkg

echo.
echo.
echo Pre-build task finished.

:Lab_Exit_And_Up

cd ..

:Lab_Exit
