package com.example.lifestyleapp;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Scanner;

public class WeatherUtils {

    private static String BASE_URL = "http://api.openweathermap.org/data/2.5/weather?q=";
    private static String APPIDQUERY = "&APPID=";
    private static final String app_id = "98be1485c455ef847338d54d176fa78b";

    // Parses JSON data from the string, data, provided
    public static WeatherData CreateWeatherData(String data) throws JSONException {
        WeatherData weatherData = new WeatherData();

        //Start parsing JSON data
        JSONObject jsonObject = new JSONObject(data); //Must throw JSONException
        JSONObject jsonMain = jsonObject.getJSONObject("main");
        JSONObject jsonWind = jsonObject.getJSONObject("wind");
        JSONArray jsonWeather = jsonObject.getJSONArray("weather");

        JSONObject jsonWeatherObj = (JSONObject) jsonWeather.get(0);

        WeatherData.CurrentCondition currentCondition = weatherData.getCurrentCondition();
        currentCondition.setHumidity(jsonMain.getInt("humidity"));
        currentCondition.setPressure(jsonMain.getInt("pressure"));
        currentCondition.setDescription(jsonWeatherObj.getString("description"));
        weatherData.setCurrentCondition(currentCondition);

        //Get the temperature, wind and cloud data.
        WeatherData.Temperature temperature = weatherData.getTemperature();
        temperature.setMaxTemp(jsonMain.getDouble("temp_max"));
        temperature.setMinTemp(jsonMain.getDouble("temp_min"));
        temperature.setTemp(jsonMain.getDouble("temp"));
        weatherData.setTemperature(temperature);

        WeatherData.Wind wind = weatherData.getWind();
        wind.setSpeed(jsonWind.getDouble("speed"));
        weatherData.setWind(wind);


        return weatherData;
    }

    public static URL buildURLFromLocation(String location) {

        URL myURL = null;
        try{
            myURL = new URL(BASE_URL + location + APPIDQUERY + app_id);
        }
        catch (MalformedURLException e) {
            e.printStackTrace();
        }
        return myURL;
    }


    public static String getDataFromURL(URL url) throws IOException {
        HttpURLConnection urlConnection = (HttpURLConnection) url.openConnection();
        String retVal = null;
        try{
            InputStream inputStream = urlConnection.getInputStream();


            //Search for the next 'beginning of the input stream
            Scanner scanner = new Scanner(inputStream);
            scanner.useDelimiter("\\A");

            if(scanner.hasNext()){
                return scanner.next();
            }
            else{
                return null;
            }

        }finally {
            urlConnection.disconnect();
        }

    }
}
