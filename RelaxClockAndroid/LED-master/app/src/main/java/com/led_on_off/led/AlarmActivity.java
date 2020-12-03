package com.led_on_off.led;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.IntentFilter;
import android.net.Uri;
import android.os.Handler;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;

import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.os.AsyncTask;

import java.io.IOException;
import java.util.UUID;

public class AlarmActivity extends ActionBarActivity{
    int decision, resultCode1;
    BroadcastReceiver receiver;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.alarm_activity);
        Intent myIntent = getIntent();
        resultCode1 = myIntent.getIntExtra("CODE",-1);
        TextView title = (TextView)findViewById(R.id.title);
        Button snooze = (Button)findViewById(R.id.snooze);
        Button stop = (Button)findViewById(R.id.stop);

        IntentFilter filter = new IntentFilter();

        filter.addAction("com.destroy.action");
        receiver = new BroadcastReceiver() {

            @Override
            public void onReceive(Context context, Intent intent) {
                goToMain(2);

            }
        };
        registerReceiver(receiver, filter);

        if(resultCode1 ==0){
            title.setText("Sleep Alarm Options");
        }
        if(resultCode1 ==1){
            title.setText("Wake Alarm Options");
        }

        snooze.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                decision = 0;
                goToMain(decision);
            }
        });
        stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                decision = 1;

                goToMain(decision);
            }
        });

    }
    void goToMain(int code){
        Intent i = new Intent(this, ledControl.class);
        i.putExtra("Decision",code);
        i.putExtra("ResultCode", resultCode1);
        setResult(RESULT_OK, i);
        finish();
    }
    // fast way to call Toast
    private void msg(String s)
    {
        Toast.makeText(getApplicationContext(),s,Toast.LENGTH_LONG).show();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unregisterReceiver(receiver);
    }
}
