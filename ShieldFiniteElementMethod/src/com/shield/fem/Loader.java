package com.shield.fem;

public class Loader extends android.app.NativeActivity {

    static {
       System.loadLibrary("assimp");
    }
 }