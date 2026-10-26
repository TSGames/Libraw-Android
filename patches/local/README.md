# Local patches that are not LibRaw releases

Everything here is applied to `pictureInfo/ndk/Libraw/jni/src` on top of the
pristine upstream release named in `../MANIFEST.json`, and is meant to
**disappear again** once upstream ships the same thing.

## `pr745-canon-eos-r6-mark-iii.patch`

Canon EOS R6 Mark III, taken unchanged from
[LibRaw pull request #745](https://github.com/LibRaw/LibRaw/pull/745), which
LibRaw closed without merging on 2026-03-06 ("still not complete or correct").
LibRaw 0.22.2 has no entry for the camera.

Without it a CR3 of the camera decodes with the ColorData of the R6 Mark II:
the white level is read from the wrong place and the picture comes out with a
strong magenta cast.

What the patch does, and what was checked on the host against the four
raw.pixls.us samples (`IMG_4241` to `IMG_4244`, 2026-10-26):

| | |
|---|---|
| camera id, model name, camera list, body features | adds `CanonID_EOS_R6m3` (0x518) |
| colour matrix | `8309,-1786,-1095,-4851,12736,2353,-956,1911,5534`, identical to the D65 ColorMatrix of the independent dnglab project (`rawler/data/cameras/canon/r6m3.toml`); both come from Adobe's DNG ColorMatrix2, so this is a profile value and not a measurement |
| white level | read at ColorData offset 0x22b for ColorDataSubVer 66: 13200 / 13995, correct |
| black level | measured from the masked border: 511, correct. The ColorData black level is still read at the R6 Mark II offset and yields 0/58/145/113, which is ignored |
| crop | 6972x4652 (full) and 4332x2886 (1.6x). Canon states 6960x4640 and 4320x2880, so this is about 12 and 6 pixels too large |
| white balance presets | still read at the R6 Mark II offsets and wrong; the as-shot white balance is correct |
| burst, Dual Pixel RAW | not covered by the pull request |

## Removing it

The guard `windows-port/tools/tests/test_libraw_local_patches.py` turns red in
two situations, and both mean the same thing:

* the vendored LibRaw is no longer the version this patch was written for, or
* the camera shows up twice (upstream added it and the patch is still on top).

On a LibRaw update, check whether the new release lists "Canon EOS R6 Mark III"
in `src/tables/cameralist.cpp`:

* **yes:** delete this directory and the test, and drop the "Local patches"
  paragraph from CLAUDE.md.
* **no:** apply the patch again (`patch -p1 < ...`), raise the version in the
  test, re-run `ios/Native/generate-libraw-patches.py`.
