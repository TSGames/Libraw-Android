package com.tssystems;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

public class Libraw {
        static {
            System.loadLibrary("libraw");
        }
        private static int COLORSPACE_SRGB=0;
        private static int COLORSPACE_ADOBE=1;
        private static int COLORSPACE_WIDE_GAMUT=2;
        private static int COLORSPACE_PRO_PHOTO=3;
        public static Bitmap decodeAsBitmap(String file,boolean halfSize){
            int result=open(file);
            setOutputBps(8);
            setQuality(2);
            setHalfSize(halfSize);
            Bitmap b=null;
            if(result!=0)
                    return b;
            int[] pixels=getPixels8();
            Log.d("libraw","pixels8 "+(pixels!=null));
            if(pixels!=null){
                Log.d("libraw","pixels8 size "+getBitmapWidth()+"x"+getBitmapHeight()+" "+pixels.length);
                b=Bitmap.createBitmap(pixels,getBitmapWidth(),getBitmapHeight(), Bitmap.Config.ARGB_8888);
            }
            cleanup();
            return b;
        }
    public static native int open(String file);
    public static native void cleanup();
    public static native int getBitmapWidth();
    public static native int getBitmapHeight();
    public static native int getWidth();
    public static native int getHeight();
    public static native int getOrientation();
    public static native int getColors();
    public static native int[] getPixels8();
    public static native long getPixels16();
    public static native float[] getDaylightMultiplier();
    public static native float[] getWhitebalanceMultiplier();
    public static native float[] getCamRgb();
    public static native float[] getRgbCam();
    public static native float[] getCamMatrix();
    public static native void setUserMul(float r,float g1,float b,float g2);
    public static native void setAutoWhitebalance(boolean autoWhitebalance);
    public static native void setHighlightMode(int highlightMode);
    public static native void setAutoBrightness(boolean autoBrightness);
    public static native void setOutputColorSpace(int colorSpace);
    public static native void setOutputBps(int outputBps);
    public static native void setQuality(int quality);
    public static native void setHalfSize(boolean halfSize);
    public static native void setGamma(double g1,double g2);
    public static native byte[] getThumbnail(String file);
    public static native void setUseCameraMatrix(int useCameraMatrix); // 0 = off, 1 = if auto whitebalance, 3 = always
    public static native String getCameraList(); // 0 = off, 1 = if auto whitebalance, 3 = always
}
