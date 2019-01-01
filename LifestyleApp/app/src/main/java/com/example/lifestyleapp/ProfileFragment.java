package com.example.lifestyleapp;

import android.arch.lifecycle.Observer;
import android.arch.lifecycle.ViewModelProviders;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

public class ProfileFragment extends Fragment {
    private TextView mTvName, mTvAge, mTvCountry, mTvCity, mTvHeight, mTvWeight, mTvSex, mTvActivity;
    private ImageView mIvPic;
    private Bitmap mProfilePic;
    public ProfileFragment(){}

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        //Inflate the detail view
        View view = inflater.inflate(R.layout.view_profile, container, false);

        //Get the text view
        mTvName = view.findViewById(R.id.tv_name);
        mTvAge = view.findViewById(R.id.tv_age);
        mTvCountry = view.findViewById(R.id.tv_country);
        mTvCity = view.findViewById(R.id.tv_city);
        mTvHeight = view.findViewById(R.id.tv_height);
        mTvWeight = view.findViewById(R.id.tv_weight);
        mTvSex = view.findViewById(R.id.tv_sex);
        mTvActivity = view.findViewById(R.id.tv_activityLevel);
        mIvPic = view.findViewById(R.id.iv_profile_fragment_pic);

        /**
         New for Part 2. Gets the same instance of the ProfileViewModel as was created in Main.
         */
        // Changed "this" to "getActivity()" - not sure which is correct right now
        ProfileViewModel profileViewModel = ViewModelProviders.of(getActivity()).get(ProfileViewModel.class);
        profileViewModel.getProfile().observe(this, profileObserver);

        // Set the current profile (this helps handle the back button. Without this it goes
        // to default text that's unrepresentative of any profile).
        UserProfile profile = profileViewModel.getProfile().getValue();
        if(profile != null){
            mTvName.setText(profile.getName());
            mTvAge.setText(Integer.toString(profile.getAge()));
            mTvCountry.setText(profile.getCountry());
            mTvCity.setText(profile.getCity());
            mTvHeight.setText(GeneralUtils.inchesToHeight(profile.getHeight()));
            mTvWeight.setText(Integer.toString(profile.getWeight()));
            mTvSex.setText(GeneralUtils.sexToString(profile.getSex()));
            mTvActivity.setText(profile.getActivityLevel());
            byte[] imageAsBytes = profile.getImage();
            Bitmap image = GeneralUtils.convertImage(imageAsBytes);
            mIvPic.setImageBitmap(image);
        }


        return view;
    }

    /**
     https://developer.android.com/topic/libraries/architecture/viewmodel#sharing used the
     another format to observe, while the examples from class created this separate 'observer'
     method. They seem to be comparable to me, but I couldn't get the web's format to work.
     */
    // Create an observer that watches the LiveData<UserProfile> object.
    final Observer<UserProfile> profileObserver = new Observer<UserProfile>() {
        @Override
        public void onChanged(@Nullable UserProfile profile) {
            // Update the UI if the data variable changes
            if(profile != null) {
                mTvName.setText(profile.getName());
                mTvAge.setText(Integer.toString(profile.getAge()));
                mTvCountry.setText(profile.getCountry());
                mTvCity.setText(profile.getCity());
                mTvHeight.setText(GeneralUtils.inchesToHeight(profile.getHeight()));
                mTvWeight.setText(Integer.toString(profile.getWeight()));
                mTvSex.setText(GeneralUtils.sexToString(profile.getSex()));
                mTvActivity.setText(profile.getActivityLevel());
                mIvPic.setImageBitmap(GeneralUtils.convertImage(profile.getImage()));
            }
        }

    };


    @Override
    public void onSaveInstanceState(Bundle outState){
        // Get all string and int values associated with this fragment
        String name = mTvName.getText().toString();
        String age = mTvAge.getText().toString();
        String weight = mTvWeight.getText().toString();
        String height = mTvHeight.getText().toString();
        String activityLevel = mTvActivity.getText().toString();
        String sex = mTvSex.getText().toString();
        String country = mTvCountry.getText().toString();
        String city = mTvCity.getText().toString();

        if(mProfilePic != null) {
            outState.putParcelable("PROFILE_PIC", mProfilePic);
        }
//        ByteArrayOutputStream os = new ByteArrayOutputStream();
//        mProfilePic.compress(Bitmap.CompressFormat.PNG, 100, os);
//        byte[] bArray = os.toByteArray();
//        outState.putByteArray("PROFILE_PIC", bArray);

        // Store all string and int values
        outState.putString("NAME", name);
        outState.putString("AGE", age);
        outState.putString("WEIGHT", weight);
        outState.putString("HEIGHT", height);
        outState.putString("ACTIVITY_LEVEL", activityLevel);
        outState.putString("SEX", sex);
        outState.putString("COUNTRY", country);
        outState.putString("CITY", city);

        //Save the view hierarchy
        super.onSaveInstanceState(outState);
    }

    @Override
    public void onViewStateRestored (Bundle savedInstanceState){

        if(savedInstanceState != null){
            // Restore simple saved data
            mTvName.setText(savedInstanceState.getString("NAME"));
            mTvAge.setText(savedInstanceState.getString("AGE"));
            mTvCountry.setText(savedInstanceState.getString("COUNTRY"));
            mTvCity.setText(savedInstanceState.getString("CITY"));
            mTvHeight.setText(savedInstanceState.getString("HEIGHT"));
            mTvWeight.setText(savedInstanceState.getString("WEIGHT"));
            mTvSex.setText(savedInstanceState.getString("SEX"));
            mTvActivity.setText(savedInstanceState.getString("ACTIVITY_LEVEL"));

            if(savedInstanceState.containsKey("PROFILE_PIC")) {
                mProfilePic= (Bitmap) savedInstanceState.getParcelable("PROFILE_PIC");
            }
//            byte[] imageArray = savedInstanceState.getByteArray("PROFILE_PIC");
//            if(imageArray != null){
//                mProfilePic = BitmapFactory.decodeByteArray(imageArray, 0, imageArray.length);
//                mIvPic.setImageBitmap(mProfilePic);
//            }
        }

        super.onViewStateRestored(savedInstanceState);
    }
}
