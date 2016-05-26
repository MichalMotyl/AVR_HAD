package com.example.defromonitor;

import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;
import android.os.Bundle;
import android.app.Activity;
//import android.view.View;
import android.view.Menu;
//import android.view.View.OnClickListener;
import android.widget.TextView;
import android.os.Handler;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import android.text.InputType;

public class MainActivity extends Activity {
	
	int exit_flag = 0;
	byte[] buf = new byte[10];
	private Handler mHandler = new Handler();
	MyThread mt = new MyThread ();
	int temp1 = 0;
   	int temp2 = 0;
   	int temp3 = 0;
   	int temp4 = 0;
   	Socket socket = null;
	
    @Override
    protected void onDestroy ()
    {
    	exit_flag = 1;
    	super.onDestroy();
    	
    	try{
    		System.out.print("Stop thread!!!!!!\n");
    	mt.join();
    	}
    	catch(Exception ex)
    	{
    		ex.printStackTrace();
    	}
    	
    	  if (socket != null){
     		   try {
     		    socket.close();
     		 // socket = null;
     		   } catch (IOException e) {
     		    // TODO Auto-generated catch block
     		    e.printStackTrace();
     		   }
     		  }
    }
 
    	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
    	updateTemp((TextView) findViewById(R.id.textView5),10);
    	updateTemp((TextView) findViewById(R.id.textView6),11);
    	updateTemp((TextView) findViewById(R.id.textView7),12);
    	updateTemp((TextView) findViewById(R.id.textView8),18);

    	//mHandler.postDelayed(runnable, 1);
    	try {
        	  socket = new Socket("176.111.113.67", 5000);

        	 } catch (UnknownHostException e) {
        	  // TODO Auto-generated catch block
        	  e.printStackTrace();
        	 } catch (IOException e) {
        	  // TODO Auto-generated catch block
        	  e.printStackTrace();
        	 }
    	
    	if(socket!=null)mt.start();
    }

    private Runnable runnable = new Runnable() {
        public void run() {
         //   pB.setProgressDrawable(getResources().getDrawable(R.drawable.green));
         //   pB.invalidate(); // maybe this will even not needed - try to comment out
        	System.out.printf("update!!!!!!\n");
        	updateTemp((TextView) findViewById(R.id.textView5),temp1);
        	updateTemp((TextView) findViewById(R.id.textView6),temp2);
        	updateTemp((TextView) findViewById(R.id.textView7),temp3);
        	updateTemp((TextView) findViewById(R.id.textView8),temp4);
        }
    }; 
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
    public void updateTemp(TextView txt,int val)
    {
    	txt.setInputType(InputType.TYPE_CLASS_NUMBER);
    	txt.setText(Integer.toString(val) + (char) 0x2103);
    }
    
    class MyThread extends Thread
    {
       public void run ()
       {
    	   while(exit_flag==0 && !Thread.currentThread().isInterrupted())
	        {
	        	System.out.print("test..\n");
	            try {
	                Thread.sleep(300,0);
	            } catch(Exception ex) {
	            	
	            	if (socket != null){
	 	      		   try {
	 	      		    socket.close();
	 	      		   } catch (IOException e) {
	 	      		    // TODO Auto-generated catch block
	 	      		    e.printStackTrace();
	 	      		   }
	 	      		  }
	            	System.out.print("return from thread!!!!!!\n");
	                Thread.currentThread().interrupt();  
	                return;
	            }
	            
	            try
	            {
	            	buf[0] = 0x5a;
	            	BufferedOutputStream output = new BufferedOutputStream(socket.getOutputStream(), 10);
	            	output.write(buf,0,10);
	            }
	            catch (UnknownHostException e) {
	          	  // TODO Auto-generated catch block
	          	  e.printStackTrace();
	          	 } catch (IOException e) {
	          	  // TODO Auto-generated catch block
	          	  e.printStackTrace();
	          	 }
	            
	            try {
		          	BufferedInputStream input2 = new BufferedInputStream(socket.getInputStream());
		          	 // BufferedReader input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
		  
		          	input2.read(buf, (int)0, (int)10);
		          	
		          	temp1 = buf[0] | buf[1]<<8;
		          	temp2 = buf[2] | buf[3]<<8;
		          	temp3 = buf[4] | buf[5]<<8;
		          	temp4 = buf[6] | buf[7]<<8;
		          	
		          	System.out.printf("%d %d %d %d\n",temp1,temp2,temp3,temp4);
		          	mHandler.postDelayed(runnable, 1);

		        	System.out.printf("after socket4\n");

		          	 } catch (UnknownHostException e) {
		          	  // TODO Auto-generated catch block
		          	  e.printStackTrace();
		          	 } catch (IOException e) {
		          	  // TODO Auto-generated catch block
		          	  e.printStackTrace();
		          	 }
		            /*
		            finally{
		      		  if (socket != null){
		      		   try {
		      		    socket.close();
		      		 // socket = null;
		      		   } catch (IOException e) {
		      		    // TODO Auto-generated catch block
		      		    e.printStackTrace();
		      		   }
		      		  }
		            }
		            */
    	   
       }
    	   
    	   System.out.print("return from thread222222!!!!!!\n");
    	   
	      		  if (socket != null){
	      		   try {
	      		    socket.close();
	      		   } catch (IOException e) {
	      		    // TODO Auto-generated catch block
	      		    e.printStackTrace();
	      		   }
	      		  }
	            
    }
    }
   
}
