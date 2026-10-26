# LibRaw as shipped in PhotoMate for Android

The LibRaw source that produces `liblibraw.so` in the PhotoMate Android app,
together with the JNI bridge that exposes it to Java.

LibRaw is dual licensed, GNU LGPL 2.1 or CDDL 1.0. Both licence texts are
included as upstream ships them, together with upstream's copyright notice.

## Contents

| Path | What it is |
|---|---|
| `jni/src/` | The LibRaw source tree as it is built, including the change described below |
| `jni/libraw/` | `libraw.c`, the JNI bridge (`Java_com_tssystems_Libraw_*`), `swab.c`/`swab.h` and `Android.mk` of the module |
| `jni/jpeglib/` | The static libjpeg the module links (`-DUSE_JPEG8`) |
| `jni/Android.mk`, `jni/Application.mk` | The ndk-build files |
| `java/com/tssystems/Libraw.java` | The Java side of the bridge |
| `app/`, `build.gradle`, `settings.gradle`, `gradlew` | A small sample app ("open a RAW file") that loads the library, so it can be tried on a phone |
| `.github/workflows/debug-apk.yml` | Builds the sample's debug APK in CI; download it from the workflow run's artifacts |
| `patches/` | Every difference from pristine upstream, one patch per file, plus `MANIFEST.json` |
| `LICENSE.LGPL` / `LICENSE.CDDL` / `COPYRIGHT` | Upstream's own |

## Upstream

- **LibRaw 0.22.2**
- https://github.com/LibRaw/LibRaw/archive/refs/tags/0.22.2.tar.gz
- Archive SHA-256: `55615c6a3f37a3cdf76e56e4c455307bf12b7d41f00bc7ce52f2e02833665b73`

## What differs from upstream

The tree is pristine upstream plus the four aggregation units that `#include`
upstream's sources (`jni/src/internal/dcraw_*.cpp`,
`jni/src/internal/demosaic_packs.cpp`, `jni/src/src/libraw_cxx.cpp`) plus **one
local patch**: Canon EOS R6 Mark III support, taken unchanged from LibRaw pull
request #745, which LibRaw closed without merging (`patches/local/`).
`patches/MANIFEST.json` lists the six files it touches. The patch is removed as
soon as a LibRaw release supports the camera.

## Rebuilding

With an Android NDK (the app is built with 29.0.13846066):

    ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk \
              NDK_APPLICATION_MK=jni/Application.mk NDK_LIBS_OUT=libs NDK_OUT=obj

This produces `libs/<abi>/liblibraw.so` for armeabi-v7a, x86, x86_64 and
arm64-v8a. The library is loaded by name (`System.loadLibrary("libraw")`), so a
rebuilt one can replace the one inside the app's APK.

## Sample app and debug APK

`./gradlew assembleDebug` (JDK 17, Android SDK, NDK 29.0.13846066) builds the
sample into `app/build/outputs/apk/debug/app-debug.apk`. The GitHub Actions
workflow "Debug APK" does the same on every push and keeps the APK as a workflow
artifact for 14 days; nothing is committed as a binary.
