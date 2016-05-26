package com.example.datamonitor;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;
import android.os.Bundle;
import android.app.Activity;
import android.view.View;
import android.view.Menu;
import android.view.View.OnClickListener;
import android.widget.TextView;
import java.util.Timer;
import java.util.TimerTask;

public class MainActivity extends Activity {

	int cnt1 = 0;
	Timer timer;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        System.out.printf("mm test onCreate!!!!!!!\n");
     //   timer = new Timer();
     //   timer.schedule(new RemindTask(), 1*1000);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        System.out.print("mm test\n");
        return true;
    }
    
    public void onButton1(View view) 
    {
    	TextView text;
    	System.out.printf("mm test %d\n",cnt1++);
    	text = (TextView) findViewById(R.id.textView1);
    	text.setText("cnt1:"+cnt1);

    	Socket socket = null;
    	 //DataOutputStream dataOutputStream = null;
    	// DataInputStream dataInputStream = null;

    	 try {
    	  socket = new Socket("176.111.113.67", 8888);
    	//  dataOutputStream = new DataOutputStream(socket.getOutputStream());
    	  System.out.printf("how meny %d\n",socket.getInputStream().available());
    	  //dataInputStream = new DataInputStream(socket.getInputStream());
    	  //dataOutputStream.writeUTF(textOut.getText().toString());
    	 // text.setText(dataInputStream.readUTF());
    	  
    	  BufferedReader input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
    	  String st = input.readLine();
    	  text.setText(st);
    	 } catch (UnknownHostException e) {
    	  // TODO Auto-generated catch block
    	  e.printStackTrace();
    	 } catch (IOException e) {
    	  // TODO Auto-generated catch block
    	  e.printStackTrace();
    	 }
    	 
    	 
    	 finally{
    		  if (socket != null){
    		   try {
    		    socket.close();
    		   } catch (IOException e) {
    		    // TODO Auto-generated catch block
    		    e.printStackTrace();
    		   }
    		  }
/*
    		  if (dataOutputStream != null){
    		   try {
    		    dataOutputStream.close();
    		   } catch (IOException e) {
    		    // TODO Auto-generated catch block
    		    e.printStackTrace();
    		   }
    		  }
    		  */
    		  /*
    		  if (dataInputStream != null){
    			   try {
    			    dataInputStream.close();
    			   } catch (IOException e) {
    			    // TODO Auto-generated catch block
    			    e.printStackTrace();
    			   }
    			  }
    			  */
    	 }
    	return;
    }
    
    class RemindTask extends TimerTask {
        public void run() {
            System.out.format("Time's up!%n");
            timer.cancel(); //Terminate the timer thread
      
        }
    }
    
}
