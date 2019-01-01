package com.example.lifestyleapp;

import java.io.Serializable;

public class WeatherData implements Serializable {
    private CurrentCondition mCurrentCondition = new CurrentCondition();
    private Temperature mTemperature = new Temperature();
    private Wind mWind = new Wind();
    private Snow mSnow = new Snow();
    private Rain mRain = new Rain();
    private Clouds mClouds = new Clouds();


    public class CurrentCondition {
        private long mWeatherId;
        private String mCondition;
        private String mDescription;
        private String mIcon;
        private double mPressure;
        private double mHumidity;

        public double getHumidity() {
            return mHumidity;
        }

        public double getPressure() {
            return mPressure;
        }

        public long getWeatherId() {
            return mWeatherId;
        }

        public String getCondition() {
            return mCondition;
        }

        public String getDescription() {
            return mDescription;
        }

        public String getIcon() {
            return mIcon;
        }

        public void setCondition(String mCondition) {
            this.mCondition = mCondition;
        }

        public void setHumidity(double mHumidity) {
            this.mHumidity = mHumidity;
        }

        public void setDescription(String mDescription) {
            this.mDescription = mDescription;
        }

        public void setPressure(double mPressure) {
            this.mPressure = mPressure;
        }

        public void setIcon(String mIcon) {
            this.mIcon = mIcon;
        }

        public void setWeatherId(long mWeatherId) {
            this.mWeatherId = mWeatherId;
        }
    }

    public class Temperature {
        private double mTemp;
        private double mMaxTemp;
        private double mMinTemp;

        public double getMaxTemp() {
            return mMaxTemp;
        }

        public double getMinTemp() {
            return mMinTemp;
        }

        public double getTemp() {
            return mTemp;
        }

        public void setMaxTemp(double mMaxTemp) {
            this.mMaxTemp = mMaxTemp;
        }

        public void setMinTemp(double mMinTemp) {
            this.mMinTemp = mMinTemp;
        }

        public void setTemp(double mTemp) {
            this.mTemp = mTemp;
        }
    }

    public class Wind {
        private double mSpeed;
        private double mDeg;

        public double getDeg() {
            return mDeg;
        }

        public double getSpeed() {
            return mSpeed;
        }

        public void setDeg(double mDeg) {
            this.mDeg = mDeg;
        }
        public void setSpeed(double mSpeed) {
            this.mSpeed = mSpeed;
        }
    }

    public class Snow {
        private String mTime;
        private double mAmount;

        public String getTime() {
            return mTime;
        }

        public double getAmount() {
            return mAmount;
        }

        public void setTime(String mTime) {
            this.mTime = mTime;
        }

        public void setAmount(double mAmount) {
            this.mAmount = mAmount;
        }
    }

    public class Rain {
        private String mTime;
        private double mAmount;

        public double getAmount() {
            return mAmount;
        }

        public String getTime() {
            return mTime;
        }

        public void setAmount(double mAmount) {
            this.mAmount = mAmount;
        }

        public void setTime(String mTime) {
            this.mTime = mTime;
        }
    }

    public class Clouds {
        private long mPercent;

        public long getPercent() {
            return mPercent;
        }

        public void setPercent(long mPercent) {
            this.mPercent = mPercent;
        }
    }

    // Getters and Setters
    public Clouds getClouds() {
        return mClouds;
    }

    public void setClouds(Clouds mClouds) {
        this.mClouds = mClouds;
    }

    public CurrentCondition getCurrentCondition() {
        return mCurrentCondition;
    }

    public void setCurrentCondition(CurrentCondition mCurrentCondition) {
        this.mCurrentCondition = mCurrentCondition;
    }

    public Rain getRain() {
        return mRain;
    }

    public void setRain(Rain mRain) {
        this.mRain = mRain;
    }

    public Snow getSnow() {
        return mSnow;
    }

    public void setSnow(Snow mSnow) {
        this.mSnow = mSnow;
    }

    public Temperature getTemperature() {
        return mTemperature;
    }

    public void setTemperature(Temperature mTemperature) {
        this.mTemperature = mTemperature;
    }

    public Wind getWind() {
        return mWind;
    }

    public void setWind(Wind mWind) {
        this.mWind = mWind;
    }
}
