# Build both ARMv5TE and ARMv7-A machine code.
APP_ABI := armeabi-v7a x86 x86_64 arm64-v8a
#APP_ABI := armeabi-v7a
#APP_ABI := arm64-v8a
#APP_ABI := armeabi

APP_STL := c++_static
APP_CPPFLAGS := -frtti -fexceptions -latomic -w
APP_OPTIM := release

# android-24 and not 21: LibRaw 0.22's libraw_datastream.h includes <fstream>, and
# libc++ then needs fseeko/ftello, which bionic declares for 32-bit ABIs only from
# API 24 (armeabi-v7a failed with "no member named 'fseeko'" on NDK 29). PhotoMate's
# minSdk is 24 anyway, and NativeRenderer is built for android-24 too.
APP_PLATFORM := android-24

#gcc is about 2x slower
#NDK_TOOLCHAIN_VERSION := 4.9