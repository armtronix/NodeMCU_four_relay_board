package net.armtronix.wififourrelay;

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

import net.armtronix.wififourrelay.R;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
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
		new Thread(new ClientThread()).start();
		mHandler = new Handler();
        mHandler.post(mUpdate);
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
            	String temp="";
            	if(isChecked)
            	{
            	 temp="on1\n\r";
            	 //onoffbutton.updateMovieFavorite(movieObj.getId().intValue(), 1);
            	 //onoffbutton.setText("ON");
            	}
            	else
            	{
                 temp="off1\n\r";
                 //onoffbutton.setText("OFF");
                 //onoffbutton.updateMovieFavorite(movieObj.getId().intValue(), 1);
            	}
            		
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
            	}
            );
		
        
      final ToggleButton onoffbutton2=(ToggleButton) findViewById(R.id.onoffbutton2);
        onoffbutton2.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @SuppressLint("NewApi")
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
            {
            	String temp="";
            	if(isChecked)
            	{
            	 temp="on2\n\r";
            	 //onoffbutton.updateMovieFavorite(movieObj.getId().intValue(), 1);
            	 //onoffbutton.setText("ON");
            	}
            	else
            	{
                 temp="off2\n\r";
                 //onoffbutton.setText("OFF");
                 //onoffbutton.updateMovieFavorite(movieObj.getId().intValue(), 1);
            	}
            		
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
            	}
            );
        final ToggleButton onoffbutton3=(ToggleButton) findViewById(R.id.onoffbutton3);
        onoffbutton3.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @SuppressLint("NewApi")
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
            {
            	String temp="";
            	if(isChecked)
            	{
            	 temp="on3\n\r";
            	 //onoffbutton.updateMovieFavorite(movieObj.getId().intValue(), 1);
            	 //onoffbutton.setText("ON");
            	}
            	else
            	{
                 temp="off3\n\r";
                 //onoffbutton.setText("OFF");
                 //onoffbutton.updateMovieFavorite(movieObj.getId().intValue(), 1);
            	}
            		
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
            	}
            );
        
        final ToggleButton onoffbuttonbutton4=(ToggleButton) findViewById(R.id.onoffbutton4);
        onoffbuttonbutton4.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @SuppressLint("NewApi")
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
            {
            	String temp="";
            	if(isChecked)
            	{
            		temp="on4\n\r";;

            	}
            	else
            	{
            		temp="off4\n\r";

            	}
            		
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

	
	public void startVoiceRecognitionActivity() 
	{
	    Intent intent = new Intent(RecognizerIntent.ACTION_RECOGNIZE_SPEECH);
	    intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE_MODEL,
	        RecognizerIntent.LANGUAGE_MODEL_FREE_FORM);
	    intent.putExtra(RecognizerIntent.EXTRA_MAX_RESULTS,5);
	    startActivityForResult(intent, VOICE_RECOGNITION_REQUEST_CODE);
	}
	
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
	    super.onActivityResult(requestCode, resultCode, data);
	    //String wordStr = null;
	    if (requestCode == VOICE_RECOGNITION_REQUEST_CODE && resultCode == RESULT_OK) {
	    	ArrayList<String> matches = data.getStringArrayListExtra(RecognizerIntent.EXTRA_RESULTS);
	    	serveroutputdata= matches.get(0);
	    	 et.setText(serveroutputdata);
	    	try {
				PrintWriter out = new PrintWriter(new BufferedWriter(
						new OutputStreamWriter(socket.getOutputStream())),
						true);
				out.println(serveroutputdata);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
	    	
	    	//serveroutputdata=null;
	    }

	       // if (matches.contains("information")) {
	        //    informationMenu();
	      
	    }
	private Runnable mUpdate = new Runnable() {
		   public void run() {
			   EditText dataget = (EditText) findViewById(R.id.Textget);
			   dataget.setText(serverinputdata);
			   mHandler.postDelayed(this, 100);
		    }
	};
    
    
	class ClientThread implements Runnable {

		@Override
		public void run() {
			
			BufferedReader br;
			//BufferedReader stdIn;
			
			try {
				InetAddress serverAddr = InetAddress.getByName(SERVER_MAIN);
				socket = new Socket(serverAddr, SERVERPORT_MAIN);
				br = new BufferedReader(new InputStreamReader(socket.getInputStream()));
				//stdIn =new BufferedReader(new InputStreamReader(System.in));
				
				while (true) {
				    //out.println(str);
				    //System.out.println("echo: " + br.readLine());
				    serverinputdata=br.readLine();
				    Log.i(TAG, "DATA:"+ serverinputdata);
			        //updateLabel.sendEmptyMessage(0);

				}
				
				//new Thread(new Thread1()).start();
				
			} catch (UnknownHostException e1) {
				e1.printStackTrace();
			} catch (IOException e1) {
				e1.printStackTrace();
			}
   
		}

	}


	


	

}