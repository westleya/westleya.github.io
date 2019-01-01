package com.example.lifestyleapp;

import android.arch.lifecycle.ViewModelProviders;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

public class GoalsFragment extends Fragment {

    private TextView mTvGoals, mTvCurrentWeight, mTvTargetWeight, mTvCurrentBMI, mTvTargetCalorie;
    public GoalsFragment(){}

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        // Inflate the detail view
        View view = inflater.inflate(R.layout.view_goals, container, false);

        // Get the Views
        mTvGoals = view.findViewById(R.id.tv_goal);
        mTvCurrentWeight = view.findViewById(R.id.tv_currentWeight);
        mTvTargetWeight = view.findViewById(R.id.tv_targetWeight);
        mTvCurrentBMI = view.findViewById(R.id.tv_currentBMI);
        mTvTargetCalorie = view.findViewById(R.id.tv_targetCalorie);

        /**
         New for Part 2. Gets the same instance of the ProfileViewModel as was created in Main.
         */
        // Changed "this" to "getActivity()" - not sure which is correct right now
        ProfileViewModel profileViewModel = ViewModelProviders.of(getActivity()).get(ProfileViewModel.class);
        UserProfile userProfile = profileViewModel.getProfile().getValue();

        // Get goal details
        int currentWeight = userProfile.getWeight();
        int targetWeight = userProfile.getTargetWeight();
        int BMI = FitnessUtils.calculateBMI(userProfile);
        int dailyCalorieTarget = FitnessUtils.calculateExpectedCaloricIntake(userProfile);

        String goal = userProfile.getGoal();

        if (MainActivity.debug) {
            assert(targetWeight == 60);
        }
        // If user has not yet defined a goal, we need filler data
        if(goal.equals("")){
            mTvGoals.setText("No Goal Set");
            mTvCurrentWeight.setText(Integer.toString(currentWeight));
            mTvTargetWeight.setText("");
            mTvCurrentBMI.setText(Integer.toString(BMI));
            mTvTargetCalorie.setText("");
        }
        else{ // Set the known details
            mTvGoals.setText(goal);
            mTvCurrentWeight.setText(Integer.toString(currentWeight));
            mTvTargetWeight.setText(Integer.toString(targetWeight));
            mTvCurrentBMI.setText(Integer.toString(BMI));
            mTvTargetCalorie.setText(Integer.toString(dailyCalorieTarget));
        }

        return view;
    }

}
