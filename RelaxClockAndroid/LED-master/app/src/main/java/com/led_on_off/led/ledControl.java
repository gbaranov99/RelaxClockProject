package com.led_on_off.led;

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
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.os.AsyncTask;

import java.io.IOException;
import java.util.List;
import java.util.UUID;


public class ledControl extends ActionBarActivity {

    // Button btnOn, btnOff, btnDis;
    Button disableWake, disableSleep, Discnt;
    EditText setTime, setWake,setSleep;
    String address = null;
    ListView wakeList, sleepList;
    public ArrayAdapter arrayAdapter1;
    public ArrayAdapter arrayAdapter2;

    private ProgressDialog progress;
    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket = null;
    private boolean isBtConnected = false;
    //SPP UUID. Look for it
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    final int MY_REQUEST_CODE = 42;
    String[] themes = {
            "Pirates of the Caribbean",
            "Star Wars",
            "Game of Thrones"
    };
    // Create the Handler object (on the main thread by default)
    Handler handler = new Handler();
    // Define the code block to be executed
    private Runnable runnableCode = new Runnable() {
        @Override
        public void run() {
            // Do something here on the main thread
//            Log.d("Handlers", "Called on main thread");
            try {
//                String btStatus = .read;
                String btStatus = "-1";
                int actualStatus = -1;
                int byteCount = btSocket.getInputStream().available();

                if(byteCount > 0) {
                    byte[] rawBytes = new byte[byteCount];
                    btSocket.getInputStream().read(rawBytes);
                    btStatus = new String(rawBytes);
                }
                btStatus = btStatus.replace("\n", "").replace("\r", "");


                actualStatus = Integer.parseInt(btStatus);
                Log.d("Handlers", btStatus);
//                if (btStatus.equals("Sleep")) {
//                    Log.d("Handlers", "Gud");
//                }
                switchPage(actualStatus);

            } catch (IOException e) {
                e.printStackTrace();
            }
            handler.postDelayed(this, 1000);
        }
    };

    void switchPage(int actualStatus) {
        Log.d("Handlers", "please save us");
        if(actualStatus == 0){
            Log.d("Handlers", "gud");
            Log.d("Handlers", "Called in wake");
            msg("hey, wakeup dinghead");
            Intent intent = new Intent(ledControl.this,AlarmActivity.class);
            intent.putExtra("CODE",1);
            startActivityForResult(intent,MY_REQUEST_CODE);

        }
        if(actualStatus == 1){
            Log.d("Handlers", "bad");
            Log.d("Handlers", "Called in sleep");
            msg("hey, go to sleep dinghead");
            Intent intent = new Intent(ledControl.this,AlarmActivity.class);
            intent.putExtra("CODE",0);
            startActivityForResult(intent,MY_REQUEST_CODE);
        }
        if(actualStatus == 2){
            Intent local = new Intent();
            local.setAction("com.destroy.action");
            sendBroadcast(local);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        Intent newint = getIntent();
        address = newint.getStringExtra(DeviceList.EXTRA_ADDRESS); //receive the address of the bluetooth device

        //view of the ledControl
        setContentView(R.layout.activity_led_control);

        handler.post(runnableCode);
        //call the widgets
        wakeList = (ListView)findViewById(R.id.wakeList);
        sleepList = (ListView)findViewById(R.id.sleepList);
        arrayAdapter1 = new ArrayAdapter(getApplicationContext(),android.R.layout.simple_list_item_1,themes);
        arrayAdapter2 = new ArrayAdapter(getApplicationContext(),android.R.layout.simple_list_item_1,themes);
        wakeList.setAdapter(arrayAdapter1);
        sleepList.setAdapter(arrayAdapter2);
        Discnt = (Button)findViewById(R.id.dis_btn);
        setTime = (EditText)findViewById(R.id.setTime);
        setWake = (EditText)findViewById(R.id.setWakeAlarm);
        setSleep  = (EditText)findViewById(R.id.setSleepAlarm);
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
                msg("Failure to sleep will result in death.");
                Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse("https://www.healthline.com/health/sleep-deprivation/effects-on-body#Causes-of-sleep-deprivation"));
                startActivity(browserIntent);
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

        sleepList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                switch (i){
                    case 0:{setTheme("1","0");
                            break;
                    }
                    case 1:{setTheme("1","1");
                        break;
                    }
                    case 2:{setTheme("1","2");
                        break;
                    }
                }
            }
        });
        wakeList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                switch (i){
                    case 0:{setTheme("0","0");
                        break;
                    }
                    case 1:{setTheme("0","1");
                        break;
                    }
                    case 2:{setTheme("0","2");
                        break;
                    }
                }
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
    @Override

    //When the second activity finishes get the information from the second activity.
    protected void onActivityResult(int requestCode, final int resultCode, Intent intent) {
        int decision = intent.getIntExtra("Decision",-1);
        int resultCode1 = intent.getIntExtra("ResultCode", -1);
        Log.d("Handlerz", Integer.toString(resultCode1));
        Log.d("Handlerzzz", Integer.toString(decision));
        if(decision ==0){
            if(resultCode1==0){
                setSnoozeSleep();
            }
            else if (resultCode1 == 1){
                setSnoozeWake();
            }
        }
        if(decision==1){
            if (resultCode1 == 0) {
                setStopSleep();
                msg("Failure to sleep will result in death.");
                Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse("https://www.healthline.com/health/sleep-deprivation/effects-on-body#Causes-of-sleep-deprivation"));
                startActivity(browserIntent);
            }
            else if (resultCode1 ==1){
                setStopWake();
            }
        }

    }
    private void setTheme(String alarmType, String theme){
        String myString = "9: "+alarmType+" "+theme;
        if (btSocket!=null)

        {
            try
            {
                btSocket.getOutputStream().write(myString.getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }
    }

    private void setSnoozeWake(){
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write("7".toString().getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }
    }
    private void setStopWake(){
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write("8".toString().getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }
    }
    private void setSnoozeSleep(){
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write("5".toString().getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }
    }
    private void setStopSleep(){
        if (btSocket!=null)
        {
            try
            {
                btSocket.getOutputStream().write("6".toString().getBytes());
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }
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



    // fast way to call Toast
    private void msg(String s)
    {
        Toast.makeText(getApplicationContext(),s,Toast.LENGTH_LONG).show();
    }

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
