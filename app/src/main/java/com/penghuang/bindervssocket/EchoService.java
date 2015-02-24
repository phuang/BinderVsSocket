package com.penghuang.bindervssocket;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;


public class EchoService extends Service {
    public EchoService() {
    }

    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }

    private final IEcho.Stub mBinder = new IEcho.Stub() {

        @Override
        public String echo(String str) throws RemoteException {
            return str;
        }
    };

}
