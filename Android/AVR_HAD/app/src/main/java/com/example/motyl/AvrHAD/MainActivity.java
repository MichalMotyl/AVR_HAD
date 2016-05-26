package com.example.motyl.AvrHAD;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TabHost;
import android.widget.TextView;
import android.widget.ToggleButton;
import com.example.motyl.AvrHAD.avr_lan_client.avr_client;
import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.common.api.GoogleApiClient;

public class MainActivity extends AppCompatActivity {
    boolean connection_ok = false;

    private avr_client avr_lan_devices;
    private static final String DBG_TAG = "MM_DEBUG";
    //List<avr_client.had_device> outputs = new ArrayList<avr_client.had_device>();
    //List<avr_client.had_device> temperature = new ArrayList<avr_client.had_device>();
    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    private GoogleApiClient client;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TabHost tabshost = (TabHost) findViewById(R.id.tabHost);
        tabshost.setup();

        TabHost.TabSpec spec = tabshost.newTabSpec("Inputs");
        spec.setContent(R.id.linearLayout);
        spec.setIndicator("Inputs");
        tabshost.addTab(spec);

        spec = tabshost.newTabSpec("Outputs");
        spec.setContent(R.id.linearLayout2);
        spec.setIndicator("Outputs");
        tabshost.addTab(spec);

        spec = tabshost.newTabSpec("Settings");
        spec.setContent(R.id.linearLayout3);
        spec.setIndicator("Settings");
        tabshost.addTab(spec);
        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client = new GoogleApiClient.Builder(this).addApi(AppIndex.API).build();

        avr_lan_devices = new avr_client();
    }

    /**
     * Method invoked when output button is pressed to change output state
     */
    private View.OnClickListener buttonOutputHandler = new View.OnClickListener() {
        public void onClick(View v) {
            // output buttons
            Log.i(DBG_TAG, "buttonOutputHandler...id: " + v.getId());
           // avr_lan_devices.had_device tmp_dev = avr_lan_devices.getOutputDevice();
            avr_client.had_device dev = (avr_client.had_device)v.getTag();
            Log.i(DBG_TAG, "buttonOutputHandler...name: " + dev.dev_features.get(v.getId()).feature_name);

        }
    };

    public void onTabChanged(String tabId) {

    }

    public void addFeature(String name, int type, int id,avr_client.had_device tag) {

        if (type == 0) {//output
           // String txt_name = null;

            ToggleButton b = new ToggleButton(this);
            //if (name == null) {

              // txt_name = "OUTPUT" + id;

            //} else {
            //  txt_name = name + id;
            //}

            b.setText(name);
            b.setTextOff(name);
            b.setTextOn(name);
            b.setId(id);
            b.setTag(tag);

            b.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT));
            b.setOnClickListener(buttonOutputHandler);
            LinearLayout Layout = (LinearLayout) findViewById(R.id.linearLayout2);
            Layout.addView(b);
            return;
        }

        if (type == 1) {//temperature
            TextView t = new TextView(this);

            //String txt_name = null;

            //txt_name = "INPUT" + id;

            t.setText(name);
            t.setId(id);
            t.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT));

            LinearLayout Layout = (LinearLayout) findViewById(R.id.linearLayout);
            Layout.addView(t);
        }
    }

    public void onButtonClickAddDevice(View view) {

        Log.i(DBG_TAG, "onButtonClickAddDevice... ");

        LayoutInflater inflater = this.getLayoutInflater();

        final View inflator = inflater.inflate(R.layout.dialog_add_device, null);

        AlertDialog.Builder builder = new AlertDialog.Builder(this);

        // Inflate and set the layout for the dialog
        // Pass null as the parent view because its going in the dialog layout
        builder.setTitle("Add Device");
        //builder.setMessage("Message");
        builder.setView(inflator);

        final EditText et1 = (EditText) inflator.findViewById(R.id.device_ip);

        builder.setPositiveButton("Add", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int whichButton) {
                String ip = et1.getText().toString();
                Log.i(DBG_TAG, "ip:" + ip);
                addDeviceToList(ip);
            }
        });

        builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int whichButton) {
                dialog.cancel();
            }
        });
        builder.create();
        builder.show();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
        switch (item.getItemId()) {
            case R.id.Connect:

                int output_cnt = 0;
                int input_cnt = 0;
                Log.i(DBG_TAG, "selected connect!!! ");
                checkLocalConnection();

                if (connection_ok) {//now we need to connect to all devices from list
                    Log.i(DBG_TAG, "connection_ok... ");

                    avr_lan_devices.readDevicesFromList(this);

                    avr_lan_devices.connect2AllDevices();

                    if(avr_lan_devices.getDevicesNum()>0)
                    {
                        for(int i =0;i<avr_lan_devices.getDevicesNum();i++)
                        {
                            while(avr_lan_devices.isDataReady()==false) {
                                try {
                                    Thread.sleep(100); // LOCK is held
                                } catch (InterruptedException e) {
                                    e.printStackTrace();
                                }
                            }

                            avr_client.had_device tmp_device = avr_lan_devices.getDevice(i);

                            int features_numb = tmp_device.dev_features.size();
                            Log.i(DBG_TAG, "features_numb... " + features_numb);


                            for(int j =0;j<features_numb;j++)
                            {
                                //String tmp_name = tmp_device.dev_features.get(j).feature_name;
                                byte type = tmp_device.dev_features.get(j).feature_type;

                                if(type==0)
                                {
                                    addFeature("OUTPUT"+output_cnt, type, tmp_device.dev_features.get(j).feature_id,tmp_device);

                                    output_cnt++;
                                }
                                else
                                {
                                    addFeature("INPUT"+input_cnt,type,tmp_device.dev_features.get(j).feature_id,tmp_device);
                                    input_cnt++;
                                }
                            }
                        }
                    }

                } else {
                    Log.i(DBG_TAG, "WIFI not enabled...");
                }
                return true;

            case R.id.Disconnect:

                return true;

            case R.id.Exit:
                System.out.printf("selected Exit!!!\n");
                super.finish();
                return true;

            default:
                return super.onOptionsItemSelected(item);
        }
    }

    public void checkLocalConnection() {
        ConnectivityManager connMgr = (ConnectivityManager)
                getSystemService(Context.CONNECTIVITY_SERVICE);

        NetworkInfo networkInfo = connMgr.getActiveNetworkInfo();

        if (networkInfo != null && networkInfo.isConnected()) {

            if (networkInfo.getType() == connMgr.TYPE_WIFI) {
                System.out.printf("connection ok!!\n");
                connection_ok = true;
            } else {
                connection_ok = false;
            }
        } else {

            connection_ok = false;
        }
    }

    public void addDeviceToList(String avr_device_ip) {

        String file = "com.tutorial2.app.settings";

        SharedPreferences settings = getSharedPreferences(file, Context.MODE_APPEND);
        SharedPreferences.Editor edit = settings.edit();

        int tmp_num_of_devices = avr_lan_devices.getDevicesNum();
        tmp_num_of_devices++;

        edit.putInt("num_of_devices", tmp_num_of_devices);
        String tmp_name = "DEV" + tmp_num_of_devices;
        Log.i(DBG_TAG, "get dev " + tmp_name);

        edit.putString(tmp_name, avr_device_ip);
        edit.apply();

        if (edit.commit()) {
            Log.i(DBG_TAG, "Save OK");
        } else {
            Log.i(DBG_TAG, "Save error? ");
        }

    }

    @Override
    public void onStart() {
        super.onStart();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client.connect();
        Action viewAction = Action.newAction(
                Action.TYPE_VIEW, // TODO: choose an action type.
                "Main Page", // TODO: Define a title for the content shown.
                // TODO: If you have web page content that matches this app activity's content,
                // make sure this auto-generated web page URL is correct.
                // Otherwise, set the URL to null.
                Uri.parse("http://host/path"),
                // TODO: Make sure this auto-generated app deep link URI is correct.
                Uri.parse("android-app://com.example.motyl.tutorial2/http/host/path")
        );
        AppIndex.AppIndexApi.start(client, viewAction);
    }

    @Override
    public void onStop() {
        super.onStop();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        Action viewAction = Action.newAction(
                Action.TYPE_VIEW, // TODO: choose an action type.
                "Main Page", // TODO: Define a title for the content shown.
                // TODO: If you have web page content that matches this app activity's content,
                // make sure this auto-generated web page URL is correct.
                // Otherwise, set the URL to null.
                Uri.parse("http://host/path"),
                // TODO: Make sure this auto-generated app deep link URI is correct.
                Uri.parse("android-app://com.example.motyl.tutorial2/http/host/path")
        );
        AppIndex.AppIndexApi.end(client, viewAction);
        client.disconnect();
    }

}
