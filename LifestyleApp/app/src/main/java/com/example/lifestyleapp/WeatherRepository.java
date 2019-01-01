package com.example.lifestyleapp;

import android.app.Application;
import android.arch.lifecycle.MutableLiveData;
import android.os.AsyncTask;

import org.json.JSONException;

import java.io.IOException;
import java.net.URL;

public class WeatherRepository {
    private final MutableLiveData<WeatherData> jsonData =
            new MutableLiveData<WeatherData>();
    private String mLocation;

    WeatherRepository(Application application){

    }

    public void setLocation(String location){
        mLocation = location;
        loadData();
    }

    public MutableLiveData<WeatherData> getData() {
        return jsonData;
    }

    private void loadData(){
        new AsyncTask<String,Void,String>(){
            @Override
            protected String doInBackground(String... strings) {
                 String location = strings[0];
                URL weatherDataURL = null;
                String retrievedJsonData = null;
                if(location!=null) {
                    weatherDataURL = WeatherUtils.buildURLFromLocation(location);
                    try {
                        // ERROR HERE: Sometimes getDataFromURL fails with bad city names (I think)
                        retrievedJsonData = WeatherUtils.getDataFromURL(weatherDataURL);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                return retrievedJsonData;
            }

            // 's' is basically "retrievedJsonData" from doInBackground() function (I think).
            @Override
            protected void onPostExecute(String s) {
                try {
                    jsonData.setValue(WeatherUtils.CreateWeatherData(s));
                }catch(JSONException e){
                    e.printStackTrace();
                }

            }
        }.execute(mLocation);
    }
}