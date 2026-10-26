# Build both ARMv5TE and ARMv7-A machine code.
APP_ABI := armeabi-v7a x86 x86_64 arm64-v8a
#APP_ABI := armeabi-v7a
#APP_ABI := arm64-v8a
#APP_ABI := armeabi

APP_STL := c++_static
APP_CPPFLAGS := -frtti -fexceptions -latomic -w
APP_OPTIM := release

APP_PLATFORM := android-21

#gcc is about 2x slower
#NDK_TOOLCHAIN_VERSION := 4.9