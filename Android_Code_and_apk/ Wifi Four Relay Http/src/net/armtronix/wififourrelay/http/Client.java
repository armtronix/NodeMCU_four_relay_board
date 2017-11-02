package net.armtronix.wififourrelay.http;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;

import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicHttpResponse;
import org.json.JSONObject;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.speech.RecognizerIntent;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;

import android.widget.Toast;
import android.widget.ToggleButton;

public class Client extends Activity {
	 private static final String TAG = "Client";
	private Socket socket;
    public SeekBar seekBar;
	private static int SERVERPORT_MAIN = 0;
	private static String SERVER_MAIN = "";
	private String serverinputdata = null;
	private String serveroutputdata = null;
	private String temp = null;
	public EditText et;
	private Handler mHandler;
	public static final int VOICE_RECOGNITION_REQUEST_CODE = 1234;
	@Override
	public void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);		
		Intent intent = getIntent();
		SERVERPORT_MAIN = Integer.valueOf(intent.getStringExtra("SERVERPORT"));
	    SERVER_MAIN = intent.getStringExtra("SERVER_IP");
		
        et =(EditText) findViewById(R.id.EditText01);
		Button send=(Button) findViewById(R.id.myButton);
        send.setOnClickListener(new OnClickListener()
        {
    	public void onClick(View view) 
    	{
    		try {
    			
    			PrintWriter out = new PrintWriter(new BufferedWriter(
    					new OutputStreamWriter(socket.getOutputStream())),
    					true);
    			//et=(EditText) findViewById(R.id.EditText01);
    			serveroutputdata = et.getText().toString();
    			out.println(serveroutputdata);
    			et.setText("");

    		} catch (UnknownHostException e) {
    			e.printStackTrace();
    		} catch (IOException e) {
    			e.printStackTrace();
    		} catch (Exception e) {
    			e.printStackTrace();
    		}
    	 }
    	});
        
        
        
        final ToggleButton onoffbutton1=(ToggleButton) findViewById(R.id.onoffbutton1);
        onoffbutton1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @SuppressLint("NewApi")
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
            {
            	
            	if(isChecked)
            	{
            	 temp="/gpio?state_04=1";
            	 new HTTPdemo().execute("");
            	 //onoffbutton.updateMovieFavorite(movieObj.getId().intValue(), 1);
            	 //onoffbutton.setText("ON");
            	}
            	else
            	{
                 temp="/gpio?state_04=0";
                 new HTTPdemo().execute("");
                 //onoffbutton.setText("OFF");
                 //onoffbutton.updateMovieFavorite(movieObj.getId().intValue(), 1);
            	}
            		
            
    			}
            	}
            );
		
        
      final ToggleButton onoffbutton2=(ToggleButton) findViewById(R.id.onoffbutton2);
        onoffbutton2.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @SuppressLint("NewApi")
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
            {
            	
            	if(isChecked)
            	{
            	 temp="/gpio?state_12=1";
            	 new HTTPdemo().execute("");
            	 //onoffbutton.updateMovieFavorite(movieObj.getId().intValue(), 1);
            	 //onoffbutton.setText("ON");
            	}
            	else
            	{
                 temp="/gpio?state_12=0";
                 new HTTPdemo().execute("");
                 //onoffbutton.setText("OFF");
                 //onoffbutton.updateMovieFavorite(movieObj.getId().intValue(), 1);
            	}
            		
    			}
            	}
            );
        final ToggleButton onoffbutton3=(ToggleButton) findViewById(R.id.onoffbutton3);
        onoffbutton3.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @SuppressLint("NewApi")
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
            {
            	
            	if(isChecked)
            	{
            	 temp="/gpio?state_13=1";
            	 new HTTPdemo().execute("");
            	 //onoffbutton.updateMovieFavorite(movieObj.getId().intValue(), 1);
            	 //onoffbutton.setText("ON");
            	}
            	else
            	{
                 temp="/gpio?state_13=0";
                 new HTTPdemo().execute("");
                 //onoffbutton.setText("OFF");
                 //onoffbutton.updateMovieFavorite(movieObj.getId().intValue(), 1);
            	}
            		
            	
    			}
            	}
            );
        
        final ToggleButton onoffbuttonbutton4=(ToggleButton) findViewById(R.id.onoffbutton4);
        onoffbuttonbutton4.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @SuppressLint("NewApi")
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
            {
            	
            	if(isChecked)
            	{
            		temp="/gpio?state_14=1";
            		 new HTTPdemo().execute("");

            	}
            	else
            	{
            		temp="/gpio?state_14=0";
            		 new HTTPdemo().execute("");

            	}
            		
            
    			}
            	}
            );
        
        seekBar = (SeekBar) findViewById(R.id.seekBar1);
        seekBar.setOnSeekBarChangeListener(

                new OnSeekBarChangeListener() {
                    int progress = 0;
                    @Override
               public void onProgressChanged(SeekBar seekBar, 

               int progresValue, boolean fromUser) {
               progress = progresValue;
               String temp="br "+(String.valueOf(progress))+"\n\r";
               try {
				PrintWriter out = new PrintWriter(new BufferedWriter(
							new OutputStreamWriter(socket.getOutputStream())),
							true);
				out.println(temp);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
               
			
			
   			
               }
                    @Override
                    public void onStartTrackingTouch(SeekBar seekBar) {
                    	
                    }
                    @Override
                    public void onStopTrackingTouch(SeekBar seekBar) {
                      // Display the value in textview
                     
                    }}

                );
	}
	
	public void informationMenu() 
	{
	    startActivity(new Intent("android.intent.action.INFOSCREEN"));
	}


	private String SendDataFromAndroidDevice() {
	    String result = "";
        String u="http://"+SERVER_MAIN+":"+SERVERPORT_MAIN+temp;
	    try {
	        HttpClient httpclient = new DefaultHttpClient();
	        HttpGet getMethod = new HttpGet(u);

	        BufferedReader in = null;
	        BasicHttpResponse httpResponse = (BasicHttpResponse) httpclient
	                .execute(getMethod);
	        in = new BufferedReader(new InputStreamReader(httpResponse
	                .getEntity().getContent()));

	        StringBuffer sb = new StringBuffer("");
	        String line = "";
	        while ((line = in.readLine()) != null) {
	            sb.append(line);
	        }
	        in.close();
	        result = sb.toString(); 


	    } catch (Exception e) {
	        e.printStackTrace();
	    }
	    return result;
	}
	
private class HTTPdemo extends
    AsyncTask<String, Void, String> {

@Override
protected void onPreExecute() {}

@Override
protected String doInBackground(String... params) {
    String result = SendDataFromAndroidDevice();
    return result;
}

@Override
protected void onProgressUpdate(Void... values) {}

@Override
protected void onPostExecute(String result) {

    if (result != null && !result.equals("")) {
        try {
            JSONObject resObject = new JSONObject(result);

            } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
}
	


	

}