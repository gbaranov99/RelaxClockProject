package com.led_on_off.led;

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


public class ledControl extends ActionBarActivity {

    // Button btnOn, btnOff, btnDis;
    Button disableWake, disableSleep, Discnt;
    EditText setTime, setWake,setSleep;
    String address = null;
    private ProgressDialog progress;
    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket = null;
    private boolean isBtConnected = false;
    //SPP UUID. Look for it
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        Intent newint = getIntent();
        address = newint.getStringExtra(DeviceList.EXTRA_ADDRESS); //receive the address of the bluetooth device

        //view of the ledControl
        setContentView(R.layout.activity_led_control);

        //call the widgets

        Discnt = (Button)findViewById(R.id.dis_btn);
        setTime = (EditText)findViewById(R.id.setTime);
        setWake = (EditText)findViewById(R.id.setWakeAlarm);
        setSleep = (EditText)findViewById(R.id.setSleepAlarm);
        disableWake = (Button)findViewById(R.id.disableWake);
        disableSleep = (Button)findViewById(R.id.disableSleep);

        disableWake.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                disableWakeFunc();
            }
        });

        disableSleep.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                disableSleepFunc();
            }
        });
        setTime.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int i, KeyEvent keyEvent) {
                String number = setTime.getText().toString();
                setTimeFunc(number);
//                Log.i("testestasdf", number);
                return false;
            }
        });
        setWake.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int i, KeyEvent keyEvent) {
                String number = setWake.getText().toString();
                setWakeFunc(number);
                return false;
            }
        });
        setSleep.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int i, KeyEvent keyEvent) {
                String number = setSleep.getText().toString();
                setSleepFunc(number);
                return false;
            }
        });

        new ConnectBT().execute(); //Call the class to connect


        Discnt.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                Disconnect(); //close connection
            }
        });


    }
    private void setWakeFunc(String time){
        if (btSocket!=null)
        {
            try
            {
                String output ="2: "+time;
                btSocket.getOutputStream().write(output.getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }
    }
    private void setSleepFunc(String time){
        if (btSocket!=null)
        {
            try
            {
                String output ="1: "+ time;
                btSocket.getOutputStream().write(output.getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }
    }
    private void disableWakeFunc(){
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write("4".toString().getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }
    }
    private void disableSleepFunc(){
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write("3".toString().getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }
    }
    private void setTimeFunc(String time){
        if (btSocket!=null)
        {
            try
            {
                String output ="0: "+ time;
                Log.i("kill me bb", output);
                btSocket.getOutputStream().write(output.toString().getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }
    }

    private void Disconnect()
    {
        if (btSocket!=null) //If the btSocket is busy
        {
            try
            {
                btSocket.close(); //close connection
            }
            catch (IOException e)
            { msg("Error");}
        }
        finish(); //return to the first layout

    }

    private void turnOffLed()
    {
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write("0".toString().getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }
    }

//    private void turnOnLed()
//    {
//        if (btSocket!=null)
//        {
//            try
//            {
//                btSocket.getOutputStream().write("1".toString().getBytes());
//            }
//            catch (IOException e)
//            {
//                msg("Error");
//            }
//        }
//    }

    // fast way to call Toast
    private void msg(String s)
    {
        Toast.makeText(getApplicationContext(),s,Toast.LENGTH_LONG).show();
    }
    /*
    public  void about(View v)
    {
        if(v.getId() == R.id.abt)
        {
            Intent i = new Intent(this, AboutActivity.class);
            startActivity(i);
        }
    }*/
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_led_control, menu);
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



    private class ConnectBT extends AsyncTask<Void, Void, Void>  // UI thread
    {
        private boolean ConnectSuccess = true; //if it's here, it's almost connected

        @Override
        protected void onPreExecute()
        {
            progress = ProgressDialog.show(ledControl.this, "Connecting...", "Please wait!!!");  //show a progress dialog
        }

        @Override
        protected Void doInBackground(Void... devices) //while the progress dialog is shown, the connection is done in background
        {
            try
            {
                if (btSocket == null || !isBtConnected)
                {
                    myBluetooth = BluetoothAdapter.getDefaultAdapter();//get the mobile bluetooth device
                    BluetoothDevice dispositivo = myBluetooth.getRemoteDevice(address);//connects to the device's address and checks if it's available
                    btSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID);//create a RFCOMM (SPP) connection
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();//start connection
                }
            }
            catch (IOException e)
            {
                ConnectSuccess = false;//if the try failed, you can check the exception here
            }
            return null;
        }
        @Override
        protected void onPostExecute(Void result) //after the doInBackground, it checks if everything went fine
        {
            super.onPostExecute(result);

            if (!ConnectSuccess)
            {
                msg("Connection Failed. Is it a SPP Bluetooth? Try again.");
                finish();
            }
            else
            {
                msg("Connected.");
                isBtConnected = true;
            }
            progress.dismiss();
        }
    }
}
