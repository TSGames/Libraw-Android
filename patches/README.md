# PhotoMate's modifications to LibRaw 0.22.2

LibRaw is dual-licensed **LGPL 2.1 or CDDL 1.0**, and PhotoMate ships a
**modified** copy of it. Both licenses require that a modified version be
marked as modified and that the modifications be available. This directory is
that marking: one `.patch` file per changed upstream file, plus
`MANIFEST.json`, which records every file the vendored tree adds on top of
upstream and whether that file ends up in the shipped binary.

Nothing here is written by hand. `ios/Native/generate-libraw-patches.py`
generates all of it from the vendored tree at
`pictureInfo/ndk/Libraw/jni/src` and pristine LibRaw 0.22.2, and
`--check` re-derives it and fails if the two have drifted apart.
`PhotoMateTests.LicenseComplianceTests.testPatchesMatchTheSourceTree` runs
the same check offline: it reverses each patch against the file in the tree
and compares the result with the recorded upstream hash.

## Upstream reference

`https://github.com/LibRaw/LibRaw/archive/refs/tags/0.22.2.tar.gz`. The manifest
of the 0.22.2 update (2026-10-26) was generated from `git archive` of that tag
because GitHub's archive endpoint was not reachable from the session, so
`archive_sha256` is the hash of the git archive. Regenerating without
`--archive` records the GitHub tarball's own hash; commit that one when it is
available. The per-file hashes do not depend on it.

Upstream ships CRLF line endings in a few files and the vendored tree is LF
throughout, so every patch applies to the upstream file **after** CRLF -> LF
conversion. The manifest keeps the hash of both forms so the conversion is
auditable instead of silent.

## What is modified

The vendored tree is pristine 0.22.2 plus the four aggregation units plus the
**local patch** in `local/` (see `local/README.md`): Canon EOS R6 Mark III, taken
from LibRaw pull request #745, which was closed unmerged. It touches six files,
all compiled, and is meant to be removed again as soon as a LibRaw release
supports the camera; `windows-port/tools/tests/test_libraw_local_patches.py`
turns red on a LibRaw update until that decision has been made.

Six translation units are compiled, identically on all platforms
(`pictureInfo/ndk/Libraw/jni/libraw/Android.mk`,
`ios/Native/build-libraw-xcframework.sh`, `windows/Native/build-libraw.ps1`);
the aggregation units pull the rest of the tree in by `#include`.

## What is added

`MANIFEST.json` lists four **generated-aggregation-unit** files, all compiled:
`internal/dcraw_common.cpp`, `internal/dcraw_fileio.cpp`,
`internal/demosaic_packs.cpp` and `src/libraw_cxx.cpp`. They contain nothing but
`#include` lines. Upstream builds every source file on its own since 0.21 and
no longer ships them, so they are kept by hand and have to be extended whenever
a LibRaw release adds a source file: 0.22 added `decoders/olympus14.cpp`,
`decoders/pana8.cpp`, `decoders/sonycc.cpp` and
`decompressors/losslessjpeg.cpp`, which `src/libraw_cxx.cpp` now includes last
(sonycc.cpp and losslessjpeg.cpp define macros they never undefine). The check:
every `src/**/*.cpp` named in upstream's `Makefile.am` must be reachable from the
six units.

Before the 0.22.2 update the tree also held about a hundred stale or generated
files (a second copy of the decoder tree, autotools output, a partial 0.22
backport) and 29 reformatted upstream files. They were replaced by pristine
upstream, which is why the patch set is small now.

## Regenerating

```bash
python3 ios/Native/generate-libraw-patches.py            # rewrite
python3 ios/Native/generate-libraw-patches.py --check    # verify only
```
