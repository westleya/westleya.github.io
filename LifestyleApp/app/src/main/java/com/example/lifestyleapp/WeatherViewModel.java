package com.example.lifestyleapp;

import android.app.Application;
import android.arch.lifecycle.AndroidViewModel;
import android.arch.lifecycle.LiveData;
import android.arch.lifecycle.MutableLiveData;
import android.arch.lifecycle.ViewModel;

public class WeatherViewModel extends AndroidViewModel {
    private MutableLiveData<WeatherData> jsonData;
    private WeatherRepository mWeatherRepository;

    public WeatherViewModel(Application application){
        super(application);
        mWeatherRepository = new WeatherRepository(application);
        jsonData = mWeatherRepository.getData();
    }

    public void setLocation(String location){
        mWeatherRepository.setLocation(location);
    }

    public LiveData<WeatherData> getData(){
        return jsonData;
    }

}
