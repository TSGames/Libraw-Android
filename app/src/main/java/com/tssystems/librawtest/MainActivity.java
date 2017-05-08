package com.tssystems.librawtest;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Debug;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.tssystems.Libraw;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStream;

public class MainActivity extends Activity {
    private static final int REQUEST_CODE = 1;
    private ImageView imageView;
    private TextView status;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        imageView=(ImageView)findViewById(R.id.image);
        status=(TextView)findViewById(R.id.status);
        findViewById(R.id.open).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
                intent.addCategory(Intent.CATEGORY_OPENABLE);
                intent.setType("*/*");
                startActivityForResult(intent, REQUEST_CODE);
            }
        });
        findViewById(R.id.list).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AlertDialog.Builder b=new AlertDialog.Builder(MainActivity.this);
                b.setMessage(Libraw.getCameraList());
                b.setCancelable(true);
                b.show();
            }
        });
    }
    @Override
    public void onActivityResult(int requestCode, int resultCode,
                                 Intent resultData) {
        if (requestCode == REQUEST_CODE && resultCode == Activity.RESULT_OK) {
            Uri uri = null;
            if (resultData != null) {
                uri = resultData.getData();
                try {

                    openImage(uri);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }
    private void openImage(final Uri uri) {
        new Thread(){
            @Override
            public void run() {

                InputStream is= null;
                try {
                    is = getContentResolver().openInputStream(uri);
                    FileOutputStream os=openFileOutput("temp",MODE_PRIVATE);
                    while(true){
                        byte[] temp=new byte[1024*512];
                        int l=is.read(temp);
                        if(l<=0)
                            break;
                        os.write(temp,0,l);
                    }
                    is.close();
                    os.close();
                } catch (Exception e) {
                    e.printStackTrace();
                    status.setText("Exception while decoding "+uri.getPath()+": "+e.toString());
                }
                String path=getFileStreamPath("temp").getAbsolutePath();

                final Bitmap bitmap=Libraw.decodeAsBitmap(path,true);
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        imageView.setImageBitmap(bitmap);
                        if(bitmap==null)
                            status.setText("Libraw failed to decode file "+uri.getPath());
                        else
                            status.setText("");
                    }
                });
            }
        }.start();

    }
}
