include(FetchContent)
FetchContent_Declare(
    toolchain_arm
    URL https://snapshots.linaro.org/gnu-toolchain/13.0-2022.08-1/arm-linux-gnueabihf/gcc-linaro-13.0.0-2022.08-x86_64_arm-linux-gnueabihf.tar.xz
)
FetchContent_MakeAvailable(toolchain_arm)
