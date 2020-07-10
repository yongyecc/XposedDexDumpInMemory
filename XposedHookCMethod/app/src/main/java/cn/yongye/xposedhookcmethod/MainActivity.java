package cn.yongye.xposedhookcmethod;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

public class MainActivity extends AppCompatActivity  implements View.OnClickListener{


    String stConfigFp = null;
    String stPkgName = null;
    EditText pkgEdittext = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        stConfigFp = "pkgname";
        pkgEdittext = findViewById(R.id.packageEdit);

        Button verifyButton = findViewById(R.id.verifyButton);
        verifyButton.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        try {
            stPkgName = pkgEdittext.getText().toString();
            FileOutputStream out = openFileOutput(stConfigFp, MODE_WORLD_READABLE);
            out.write(stPkgName.getBytes());
            out.close();
            Toast.makeText(MainActivity.this, "包名为：" + stPkgName + "", Toast.LENGTH_LONG).show();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
