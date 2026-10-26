package com.tssystems.librawtest;

import android.graphics.Bitmap;
import android.util.Log;

import com.tssystems.Libraw;

/** What the first version of the sample had as Libraw.decodeAsBitmap, kept next to the app. */
final class Decoder {
    private Decoder() {
    }

    static Bitmap decodeAsBitmap(String file, boolean halfSize) {
        int result = Libraw.open(file);
        Libraw.setOutputBps(8);
        Libraw.setQuality(2);
        Libraw.setHalfSize(halfSize);
        if (result != 0) {
            return null;
        }
        Bitmap bitmap = null;
        int[] pixels = Libraw.getPixels8();
        Log.d("libraw", "pixels8 " + (pixels != null));
        if (pixels != null) {
            Log.d("libraw", "pixels8 size " + Libraw.getBitmapWidth() + "x"
                    + Libraw.getBitmapHeight() + " " + pixels.length);
            bitmap = Bitmap.createBitmap(pixels, Libraw.getBitmapWidth(),
                    Libraw.getBitmapHeight(), Bitmap.Config.ARGB_8888);
        }
        Libraw.cleanup();
        return bitmap;
    }
}
