package net.armtronix.wififourrelay.http;



import net.armtronix.wififourrelay.http.About;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

public class Enter_IP extends Activity {
	

	public void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.enterval);
		
		Button connect=(Button) findViewById(R.id.connect);
        connect.setOnClickListener(new OnClickListener() {
            @Override
			public void onClick(View v) {
        EditText ipaddress = (EditText) findViewById(R.id.ip);
        EditText portnumber = (EditText) findViewById(R.id.port);
		String str = ipaddress.getText().toString();
		String a=portnumber.getText().toString();
		Intent intent = new Intent(getApplicationContext(), Client.class);
		intent.putExtra("SERVERPORT",a);
		intent.putExtra("SERVER_IP",str);
		startActivity(intent);
            }
        });
	}
	 @Override
	    public boolean onCreateOptionsMenu(Menu menu) {
	        MenuInflater inflater = getMenuInflater();
	        inflater.inflate(R.menu.option_menu, menu);
	        return super.onCreateOptionsMenu(menu);
	        //return true;
	    }
    public boolean onOptionsItemSelected(MenuItem item) 
    {
    	switch (item.getItemId()) 
    	{
        case R.id.about:
            // Ensure this device is discoverable by others
           // ensureDiscoverable();
        	Intent aboutus=new Intent(this, About.class);
        	startActivity(aboutus);
            return true;
        }
        return super.onOptionsItemSelected(item);
        //return false;
    }
    	
	
}

