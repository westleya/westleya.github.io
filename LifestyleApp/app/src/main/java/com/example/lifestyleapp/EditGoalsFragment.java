package com.example.lifestyleapp;

import android.arch.lifecycle.ViewModelProviders;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentTransaction;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.Toast;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.concurrent.TimeUnit;

public class EditGoalsFragment extends Fragment implements View.OnClickListener, AdapterView.OnItemSelectedListener {

    // Views
    private Spinner mSpTargetWeight;
    private Spinner mSpTargetYear;
    private Spinner mSpTargetMonth;
    private Spinner mTargetDay;
    private Button mButtonCalculate;

    private int mCurrentWeight;

    @Override
    public void onClick(View view) {
        switch(view.getId()){
            case R.id.button_calculate:{

                // Get values from spinners
                int goalWeight = Integer.parseInt(mSpTargetWeight.getSelectedItem().toString());
                int year = Integer.parseInt(mSpTargetYear.getSelectedItem().toString());
                int month = Integer.parseInt(mSpTargetMonth.getSelectedItem().toString());
                int day = Integer.parseInt(mTargetDay.getSelectedItem().toString());

                /**
                 New for Part 2. Gets the same instance of the ProfileViewModel as was created in Main.
                 */
                // Changed "this" to "getActivity()" - not sure which is correct right now
                ProfileViewModel profileViewModel = ViewModelProviders.of(getActivity()).get(ProfileViewModel.class);
                UserProfile userProfile = profileViewModel.getProfile().getValue();
                mCurrentWeight = userProfile.getWeight();

                // Figure out the user's weight goal
                String goal = "Maintain Weight";
                double poundsToLose = mCurrentWeight - goalWeight;
                double poundsPerWeek = 0; // Calculated further below if there's a change
                if(goalWeight < mCurrentWeight){
                    goal = "Lose Weight";
                }
                else if(goalWeight > mCurrentWeight) {
                    goal = "Gain Weight";
                }

                boolean leap = false;
                if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) {
                    leap = true;
                }
                int[] dates = new int[]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
                dates[1] += leap ? 1 : 0;
                if (day > dates[month - 1]) {
                    Toast.makeText(this.getContext(), "Please select correct date.", Toast.LENGTH_SHORT).show();
                    return;
                }

                String targetStr = day + "/" + month + "/" + year;
                SimpleDateFormat sdf = new SimpleDateFormat("dd/MM/yyyy");
                try {
                    // Calculate the time difference between now and goal date
                    Date targetDate = sdf.parse(targetStr);
                    Date c = Calendar.getInstance().getTime();
                    sdf.format(c);
                    long diff = targetDate.getTime() - c.getTime();

                    // test with invalid target date
                    if (MainActivity.debug) {
                        assert(diff < 0);
                    }

                    if (diff < 0) {
                        Toast.makeText(this.getContext(), "please select a valid target date", Toast.LENGTH_SHORT).show();
                        return;
                    }

                    int currentCal = FitnessUtils.calculateExpectedCaloricIntake(userProfile);

                    // Calculate the number of weeks between now and goal
                    long days = TimeUnit.DAYS.convert(diff, TimeUnit.MILLISECONDS);
                    double weeks = days / 7.0;
                    poundsPerWeek = -1 * poundsToLose / weeks;

                    if (Math.abs(poundsPerWeek) > 2) {
                        Toast.makeText(this.getContext(), "Warning, you are planning to lose / gain more than 2 lbs per week. Please select a different target.", Toast.LENGTH_LONG).show();
                        return;
                    }
                    if (mCurrentWeight > goalWeight) {
                        double cal = currentCal - (( poundsToLose * 3500) / days);

                        if (userProfile.getSex() && cal < 1200) {
                            Toast.makeText(this.getContext(), "Warning, your plan will result in low caloric intake. Please select a different target.", Toast.LENGTH_LONG).show();
                            return;
                        }
                        if (!userProfile.getSex() && cal < 1000) {
                            Toast.makeText(this.getContext(), "Warning, your plan will result in low caloric intake. Please select a different target.", Toast.LENGTH_LONG).show();
                            return;
                        }
                    }
                } catch (ParseException e) {
                    e.printStackTrace();
                }
                userProfile.setTargetWeight(goalWeight);
                userProfile.setPoundsPerWeek(poundsPerWeek);
                userProfile.setGoal(goal);
                profileViewModel.update(userProfile);

                GoalsFragment goalsFragment = new GoalsFragment();
                FragmentTransaction ftrans = getActivity().getSupportFragmentManager().beginTransaction();
                if(getResources().getBoolean(R.bool.isTablet)) {
                    ftrans.replace(R.id.fl_frag_itemdetail_container_tablet, goalsFragment, "Goals_Fragment" );
                }
                else {
                    ftrans.replace(R.id.fl_frag_masterlist_container_phone, goalsFragment, "Goals_Fragment");
                }
                ftrans.addToBackStack(null);
                ftrans.commit();
            }
        }
    }

    @Override
    public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {

    }

    @Override
    public void onNothingSelected(AdapterView<?> parent) {

    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        //Inflate the detail view
        View view = inflater.inflate(R.layout.edit_goals, container, false);

        //Get the text view
        mSpTargetWeight =  view.findViewById(R.id.spinner_targetWeight);
        mSpTargetYear =  view.findViewById(R.id.spinner_targetYear);
        mSpTargetMonth =  view.findViewById(R.id.spinner_targetMonth);
        mTargetDay =  view.findViewById(R.id.spinner_targetDay);

        mButtonCalculate = view.findViewById(R.id.button_calculate);
        mButtonCalculate.setOnClickListener(this);

        String[] weight = new String[351];
        for (int i = 0; i < 351; ++i) {
            weight[i] = Integer.toString(i + 50);
        }
        ArrayAdapter<String> weightAdapter = new ArrayAdapter<>(this.getActivity(), android.R.layout.simple_spinner_item, weight);
        weightAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mSpTargetWeight.setAdapter(weightAdapter);

        String[] year = new String[10];
        for (int i = 0; i < 10; ++i) {
            year[i] = Integer.toString(i + 2018);
        }
        ArrayAdapter<String> yearAdapter = new ArrayAdapter<>(this.getActivity(), android.R.layout.simple_spinner_item, year);
        yearAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mSpTargetYear.setAdapter(yearAdapter);

        String[] month = new String[12];
        for (int i = 0; i < 12; ++i) {
            month[i] = Integer.toString(i + 1);
        }
        ArrayAdapter<String> monthAdapter = new ArrayAdapter<>(this.getActivity(), android.R.layout.simple_spinner_item, month);
        monthAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mSpTargetMonth.setAdapter(monthAdapter);

        String[] day = new String[31];
        for (int i = 0; i < 31; ++i) {
            day[i] = Integer.toString(i + 1);
        }
        ArrayAdapter<String> dayAdapter = new ArrayAdapter<>(this.getActivity(), android.R.layout.simple_spinner_item, day);
        dayAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mTargetDay.setAdapter(dayAdapter);

        return view;
    }

    @Override
    public void onSaveInstanceState(Bundle outState){
        // Get all string and int values associated with this fragment
        int weight = Integer.parseInt(mSpTargetWeight.getSelectedItem().toString());
        int year = Integer.parseInt(mSpTargetYear.getSelectedItem().toString());
        int month = Integer.parseInt(mSpTargetMonth.getSelectedItem().toString());
        int day = Integer.parseInt(mTargetDay.getSelectedItem().toString());

        // Store all string and int values
        outState.putInt("WEIGHT", weight);
        outState.putInt("YEAR", year);
        outState.putInt("MONTH", month);
        outState.putInt("DAY", day);

        //Save the view hierarchy
        super.onSaveInstanceState(outState);
    }

    @Override
    public void onViewStateRestored (Bundle savedInstanceState){

        if(savedInstanceState != null){
            // Restore simple saved data
            mSpTargetWeight.setSelection(savedInstanceState.getInt("WEIGHT"));
            mSpTargetYear.setSelection(savedInstanceState.getInt("YEAR"));
            mSpTargetMonth.setSelection((savedInstanceState.getInt("MONTH")));
            mTargetDay.setSelection((savedInstanceState.getInt("DAY")));
        }

        super.onViewStateRestored(savedInstanceState);
    }
}
