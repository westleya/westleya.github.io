package com.example.lifestyleapp;

import android.arch.lifecycle.ViewModelProviders;
import android.content.Context;
import android.graphics.Color;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

public class StepsFragment extends Fragment {

    private TextView mTvSteps;
    private SensorManager mSensorManager;
    private Sensor mLinearAccelerometer, mStepCounter;
    private final double mThreshold = 55.0;

    private double last_x, last_z;
    private double curr_x, curr_z;
    private float mCurrSteps=0, mPrevSteps=0;
    private boolean mNotFirstTime = false, mCountingSteps = false, mStartCount=true;
    ProfileViewModel mProfileViewModel;
    MediaPlayer mPlayer;
    public StepsFragment() {}

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        // Inflate the detail view
        View view = inflater.inflate(R.layout.step_counter, container, false);

        // Get the View
        mTvSteps = view.findViewById(R.id.tv_steps);

        // Get the Sensor Manager
        mSensorManager = (SensorManager) getActivity().getSystemService(Context.SENSOR_SERVICE);
        mProfileViewModel = ViewModelProviders.of(getActivity()).get(ProfileViewModel.class);
        mCurrSteps = mProfileViewModel.getProfile().getValue().getSteps();
        mTvSteps.setText("" + (int) mCurrSteps);

        if((int) mCurrSteps > 0) {
            mStartCount = false;
            mCountingSteps = true;
            mPrevSteps = (int) mProfileViewModel.getProfile().getValue().getStartSteps();
            mTvSteps.setTextColor(Color.GREEN);
        }
        // Get the default accelerometer
        mLinearAccelerometer = mSensorManager.getDefaultSensor(Sensor.TYPE_LINEAR_ACCELERATION);
        mStepCounter = mSensorManager.getDefaultSensor(Sensor.TYPE_STEP_COUNTER);
        mPlayer = MediaPlayer.create(this.getContext(), R.raw.sound);

        return view;
    }

    private SensorEventListener mListener = new SensorEventListener() {
        @Override
        public void onSensorChanged(SensorEvent sensorEvent) {

            // Check which kind of sensor triggered the event
            if(sensorEvent.sensor.getType() == Sensor.TYPE_LINEAR_ACCELERATION) {
                // Get the acceleration rates along the x and z axes
                curr_x = sensorEvent.values[0];
                curr_z = sensorEvent.values[2];

                if(mNotFirstTime) {
                    double dx = Math.abs(last_x - curr_x);
                    double dz = Math.abs(last_z - curr_z);

                    // Check if the acceleration has changed on any of the horizontal axes
                    if(dx > mThreshold){
                        mCountingSteps = true;
                        // Left - right. Start counting
                        Toast.makeText(getActivity(), "Step counter started", Toast.LENGTH_SHORT).show();
                        mTvSteps.setTextColor(Color.GREEN);
                        mPlayer.start();
                    }
                    else if (dz > mThreshold) {
                        // Front - back. Stop counting
                        mCountingSteps = false;
                        Toast.makeText(getActivity(), "Step counter stopped", Toast.LENGTH_SHORT).show();
                        mCurrSteps = 0;
                        mPrevSteps=0;
                        mStartCount=true;
                        mTvSteps.setTextColor(Color.GRAY);

                    }

                }
                last_x = curr_x;
                last_z = curr_z;
                mNotFirstTime = true;
            }
            else if(sensorEvent.sensor.getType() == Sensor.TYPE_STEP_COUNTER && mCountingSteps) {
                if(mStartCount) {
                    mPrevSteps = sensorEvent.values[0];
                    mStartCount = false;
                }
                else {
                    mCurrSteps += sensorEvent.values[0] - mPrevSteps;
                    mPrevSteps = sensorEvent.values[0];
                }
                mTvSteps.setText("" + String.valueOf((int) mCurrSteps));
            }
        }

        @Override
        public void onAccuracyChanged(Sensor sensor, int accuracy) {

        }
    };

    @Override
    public void onResume() {
        super.onResume();
        if(mLinearAccelerometer!=null) {
            mSensorManager.registerListener(mListener, mLinearAccelerometer, SensorManager.SENSOR_DELAY_NORMAL);
        }
        if(mStepCounter!=null) {
            mSensorManager.registerListener(mListener, mStepCounter,SensorManager.SENSOR_DELAY_NORMAL);
        }
    }

    @Override
    public void onSaveInstanceState(Bundle outState){
        //Save the view hierarchy
        super.onSaveInstanceState(outState);
    }

    @Override
    public void onPause() {
        super.onPause();
        if(mLinearAccelerometer!=null || mStepCounter!=null) {
            mSensorManager.unregisterListener(mListener);
        }
        mProfileViewModel.getProfile().getValue().setSteps(mCurrSteps);
        mProfileViewModel.getProfile().getValue().setStartSteps(mPrevSteps);
    }

}
