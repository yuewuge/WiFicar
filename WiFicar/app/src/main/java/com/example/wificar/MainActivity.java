package com.example.wificar;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.ImageButton;
import android.widget.ImageView;


import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MqttDefaultFilePersistence;



public class MainActivity extends AppCompatActivity {

    MqttAndroidClient mClient;
    ImageView im;
    boolean flag_go;
    boolean flag_back;
    ImageButton btn_go;
    ImageButton btn_back;
    ImageButton btn_left;
    ImageButton btn_right;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        im = (ImageView)findViewById(R.id.image);
        btn_go = (ImageButton)findViewById(R.id.go);
        btn_back = (ImageButton)findViewById(R.id.back);
        btn_left = (ImageButton)findViewById(R.id.left);
        btn_right = (ImageButton)findViewById(R.id.right);
        btn_go.setOnTouchListener(listnner1);
        btn_back.setOnTouchListener(listnner1);
        btn_left.setOnTouchListener(listnner1);
        btn_right.setOnTouchListener(listnner1);
        WebView webView = (WebView)findViewById(R.id.wv);
        webView.loadUrl("http://*******/stream");//******为花生壳域名
        webView.setWebViewClient(new WebViewClient());
        try {
            initClient();//连接ONENET
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    private View.OnTouchListener listnner1 = new View.OnTouchListener() {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            int action = event.getAction();
            switch (v.getId()){
                case R.id.go:
                    if(action == MotionEvent.ACTION_DOWN){
                        try {
                            mClient.publish("go","1".getBytes(),0,false);
                            Log.e("go","1");
                            flag_go = true;
                        } catch (MqttException e) {
                            e.printStackTrace();
                        }
                    }else if(action == MotionEvent.ACTION_UP){
                        try {
                            mClient.publish("go","0".getBytes(),0,false);
                            flag_go = false;
                            Log.e("go","0");
                        } catch (MqttException e) {
                            e.printStackTrace();
                        }
                    }break;
                case R.id.back:
                    if(action == MotionEvent.ACTION_DOWN){
                        try {
                            mClient.publish("back","2".getBytes(),0,false);
                            flag_back = true;
                            Log.e("back","1");
                        } catch (MqttException e) {
                            e.printStackTrace();
                        }
                    }else if(action == MotionEvent.ACTION_UP){
                        try {
                            mClient.publish("back","0".getBytes(),0,false);
                            flag_back = false;
                            Log.e("back","0");
                        } catch (MqttException e) {
                            e.printStackTrace();
                        }
                    }break;
                case R.id.left:
                    if((action == MotionEvent.ACTION_DOWN) && flag_go){
                        try {
                            mClient.publish("left","3".getBytes(),0,false);
                            Log.e("left","1");
                        } catch (MqttException e) {
                            e.printStackTrace();
                        }
                    }else if((action == MotionEvent.ACTION_UP) && flag_go){
                        try {
                            mClient.publish("left","1".getBytes(),0,false);
                            Log.e("left","0");
                        } catch (MqttException e) {
                            e.printStackTrace();
                        }
                    }else if((action == MotionEvent.ACTION_DOWN) && flag_back){
                        try {
                            mClient.publish("left","5".getBytes(),0,false);
                            Log.e("left","1");
                        } catch (MqttException e) {
                            e.printStackTrace();
                        }
                    }else if((action == MotionEvent.ACTION_UP) && flag_back){
                        try {
                            mClient.publish("left","2".getBytes(),0,false);
                            Log.e("left","0");
                        } catch (MqttException e) {
                            e.printStackTrace();
                        }
                    }break;
                case R.id.right:
                    if((action == MotionEvent.ACTION_DOWN) && flag_go){
                        try {
                            mClient.publish("right","4".getBytes(),0,false);
                            Log.e("right","1");
                        } catch (MqttException e) {
                            e.printStackTrace();
                        }
                    }else if((action == MotionEvent.ACTION_UP) && flag_go){
                        try {
                            mClient.publish("right","1".getBytes(),0,false);
                            Log.e("right","0");
                        } catch (MqttException e) {
                            e.printStackTrace();
                        }
                    }else if((action == MotionEvent.ACTION_DOWN) && flag_back){
                        try {
                            mClient.publish("right","6".getBytes(),0,false);
                            Log.e("right","1");
                        } catch (MqttException e) {
                            e.printStackTrace();
                        }
                    }else if((action == MotionEvent.ACTION_UP) && flag_back){
                        try {
                            mClient.publish("right","2".getBytes(),0,false);
                            Log.e("right","0");
                        } catch (MqttException e) {
                            e.printStackTrace();
                        }
                    }break;
            }

            return false;
        }
    };



    private void initClient() throws MqttException {

        //连接参数设置
        // 获取默认的临时文件路径
        String tmpDir = System.getProperty("java.io.tmpdir");

        // Mqtt的默认文件持久化
        MqttDefaultFilePersistence dataStore = new MqttDefaultFilePersistence(tmpDir);
        String serverURI = "tcp://183.230.40.39:6002";
        String clientId = "********";//设备ID
        final MqttConnectOptions options = new MqttConnectOptions();
        options.setCleanSession(false);
        options.setUserName("******");//产品ID
        options.setPassword("*****************".toCharArray());//鉴权信息
        options.setAutomaticReconnect(true);
        mClient = new MqttAndroidClient(getApplicationContext(), serverURI, clientId,dataStore);

        //连接回调
        mClient.setCallback(new MqttCallbackExtended() {

            //连接服务器成功时触发，可在这时订阅想要订阅的topic
            @Override
            public void connectComplete(boolean reconnect, String serverURI) {
                Log.e("connectComplete","connectComplete");
                try {
                    mClient.subscribe("img",2);
                } catch (MqttException e) {
                    e.printStackTrace();
                }
            }

            //连接丢失时触发
            @Override
            public void connectionLost(Throwable cause) {
                Log.e("connectionLost","connectionLost");
            }

            //接收到订阅的消息，可以进行一些逻辑业务处理
            @Override
            public void  messageArrived(String topic, MqttMessage message) throws Exception {

            }
            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {
                Log.e("deliveryComplete","deliveryComplete");
            }
        });
        mClient.connect(options);//开始连接

    }

}
