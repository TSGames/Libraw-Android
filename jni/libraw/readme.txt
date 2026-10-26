PhotoMate's LibRaw module -- what is patched, and what only looked patched
==========================================================================

This file used to read:

    Include swab.h to all files missing the function

    libraw_datastream.cpp: (at the end of imports)
    #include <photomate_fixes.h>

    libraw_cxx.cpp:
    #include <swab.h>
    #include <stdio.h>
    #include <jpeglib.h>

None of that is in the tree. Checked against pristine LibRaw 0.21.4 (T-084), and
true of 0.22.2 as well, where jni/src is pristine upstream apart from the R6
Mark III patch and the four aggregation units (which contain only #include
lines of upstream sources):

  - No LibRaw source includes swab.h, stdio.h, jpeglib.h or photomate_fixes.h
    on PhotoMate's behalf. src/libraw_datastream.cpp needs no change any more:
    upstream 0.22 includes libraw/libraw_datastream.h itself.
  - No file anywhere under pictureInfo/ndk/Libraw/ includes
    photomate_fixes.h or swab.h.

So photomate_fixes.h is dead: it is kept because it costs nothing and
documents which Bionic gaps once had to be papered over, but it takes part in
no build. swab.c, by contrast, IS live on Android -- Android.mk compiles it
directly to supply swab() to the rest of the module -- but it is provided as
a separate translation unit, not as an include into LibRaw sources. iOS needs
neither (see ios/Native/build-libraw-xcframework.sh).

The real modifications to LibRaw, all of them, are kept as patch files with a
manifest under pictureInfo/ndk/Libraw/patches/ and are regenerated (and
verified against the tree) by ios/Native/generate-libraw-patches.py. Read the
README there before changing anything under jni/src: LibRaw is LGPL 2.1 /
CDDL 1.0, and an unrecorded change to that tree is a licence problem, not just
an undocumented edit.

The files in THIS directory (jni/libraw/) are PhotoMate's own, not LibRaw's:
libraw.c is the JNI bridge, swab.c/swab.h the Bionic shim, photomate_fixes.h
the dead header above.

The libraw_*.h headers here are plain copies of LibRaw's public headers from
jni/src/libraw (0.22.2 since the 2026-10-26 update; they were 0.22.0 headers next
to a 0.21.4 build before that). They are inert: Android.mk lists them only under
LOCAL_EXPORT_C_INCLUDES, not LOCAL_C_INCLUDES, so the module itself compiles
against jni/src/libraw, and no other NDK module declares liblibraw as a
dependency. iOS never sees them (build-libraw-xcframework.sh works out of
jni/src). Copy them again whenever jni/src/libraw changes, so that a module
which does depend on liblibraw one day compiles against the declarations of the
library it links.
