#!/bin/bash

TRIPLET=
MY_PORTS=../scripts/vcpkg/ports
MY_TRIPLETS=../scripts/vcpkg/triplets

# determine processor
OS=`uname -s`
if [[ $OS == MINGW* ]]; then
    if [[ `uname -m` == x86_64 ]]; then
        TRIPLET=x64-windows
    else
        TRIPLET=x86-windows
    fi
elif [[ $OS == Linux* ]]; then
    if [[ `uname -m` == x86_64 ]]; then
        TRIPLET=x64-linux
    else
        TRIPLET=x86-linux
    fi
else
    echo "To be added"
    exit
fi


function build_libs() {

    echo -e "\n"
    echo "================================= Build zlib ================================="

    ./vcpkg install zlib:$TRIPLET --overlay-triplets=$MY_TRIPLETS

    echo -e "\n"
    echo "================================= Build sdl2 ================================="

    ./vcpkg install sdl2:$TRIPLET --overlay-triplets=$MY_TRIPLETS

    echo -e "\n"
    echo "================================= Build yaml-cpp ================================="

    ./vcpkg install yaml-cpp:$TRIPLET --overlay-triplets=$MY_TRIPLETS

    echo -e "\n"
    echo "================================= Build quazip ================================="

    ./vcpkg install quazip:$TRIPLET --overlay-ports=$MY_PORTS --overlay-triplets=$MY_TRIPLETS

    echo -e "\n"
    echo "================================= Build framelesshelper ================================="

    ./vcpkg install framelesshelper:$TRIPLET --overlay-ports=$MY_PORTS --overlay-triplets=$MY_TRIPLETS

    echo -e "\n"
    echo "================================= Build ffmpeg ================================="

    #./vcpkg install ffmpeg[core,avcodec,avdevice,avfilter,avformat,swresample,swscale,ffmpeg,ffprobe]:$TRIPLET --recurse
    ./vcpkg install ffmpeg-fake:$TRIPLET --overlay-ports=$MY_PORTS --overlay-triplets=$MY_TRIPLETS

    echo -e "\n"
    echo "================================= Clean Up ================================="

    echo "Remove Build Trees"
    rm -rf buildtrees

    echo "Remove Downloads"
    rm -rf downloads

    echo "Remove Packages"
    rm -rf packages

    # echo "Remove Caches"
    # rm -rf ~/.cache/vcpkg

}




# ================================= Start =================================

# check working dir
if [ ! -e "scripts/vcpkg" ]; then
    echo "scripts dir not found."
    exit 0
fi


echo "================================= Detect Proxy ================================="

echo "No system proxy detected."


# check if repository is empty, remove if empty
if [ -f "vcpkg" ]; then
    rm -rf vcpkg
elif [ -e "vcpkg" ]; then
    if [ -z `ls -A vcpkg` ]; then
        echo "vcpkg dir is empty, auto remove"
        rm -rf vcpkg
    fi
fi


echo -e "\n"
echo "================================= Pull vcpkg ================================="

git clone https://github.com/microsoft/vcpkg.git


echo -e "\n"
echo "================================= Build vcpkg ================================="

# check if repository is valid
if [ ! -f "vcpkg/bootstrap-vcpkg.sh" ]; then
    echo "bootstrap-vcpkg.sh not found."
    exit 0
fi

cd vcpkg

if [ ! -f "vcpkg" ]; then
    ./bootstrap-vcpkg.sh
fi

if [ ! -f "vcpkg" ]; then
    echo "vcpkg executable not found."
    exit 0
fi

build_libs

echo -e "\n"
echo "Pre-build task finished."