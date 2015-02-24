package com.penghuang.bindervssocket;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TextView;

public class MainActivity extends Activity {

    private static final String TAG = "MainActivity";
    static {
        System.loadLibrary("MyLib");
    }

    private native double socketTest();

    private native double binderTest();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Log.d(TAG, "onCreate()");
    }

    @Override
    protected void onStart() {
        super.onStart();
        // double result = socketTest();
        Intent i = new Intent();
        i.setComponent(new ComponentName("com.penghuang.bindervssocket", "com.penghuang.bindervssocket.EchoService"));
        this.bindService(i,
                         mConnection, Context.BIND_AUTO_CREATE);
        TextView tv;


        if (true) {
            tv = (TextView) findViewById(R.id.textView_socket);
            tv.setText("socket = " + socketTest());

            tv = (TextView) findViewById(R.id.textView_binder);
            tv.setText("binder = " + binderTest());
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    private IEcho mIEcho = null;

    private final ServiceConnection mConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            Log.d(TAG, "onServiceConnected()");
            mIEcho = IEcho.Stub.asInterface(service);
            try {
                if (false) {
                    long begin = System.nanoTime();
                    for (int i = 0; i < 100000; i++) {
                        mIEcho.echo("Hello World");
                    }
                    long end = System.nanoTime();
                    double dur = (double)end - begin;
                    Log.d(TAG, "duration=" + (dur / 1000000000));
                }
            } catch (RemoteException e) {
                e.printStackTrace();
            }

        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            mIEcho = null;
        }
    };

}
