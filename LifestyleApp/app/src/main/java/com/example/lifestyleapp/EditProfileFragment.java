package com.example.lifestyleapp;

import android.arch.lifecycle.ViewModelProviders;
import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.provider.MediaStore;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentTransaction;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.Spinner;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.concurrent.ExecutionException;

import static android.widget.AdapterView.OnItemSelectedListener;

/**
 * create an instance of this fragment.
 */
public class EditProfileFragment extends Fragment implements View.OnClickListener, OnItemSelectedListener {

    // ID's for the sex buttons
    private static final int FEMALE = 0;
    private static final int MALE = 1;

    // Define a request code for the camera
    private static final int REQUEST_IMAGE_CAPTURE = 1;

    // View objects
    private EditText m_etName;
    private Spinner m_spAge ;
    private Spinner m_spCountry;
    private Spinner m_spCity;
    private Spinner m_spHeight;
    private Spinner m_spWeight;
    private Spinner m_spActivityLevel;
    private RadioGroup m_sexRadioButtons;
    private RadioButton m_rbMale;
    private RadioButton m_rbFemale;
    private Button mSubmitButton;
    private Button mGetPictureButton;
    private ImageView m_ivProfilePic;

    // Profile data
    private UserProfile mProfile;
    private String mName;
    private int mAge;
    private String mCountry;
    private String mCity;
    private String mActivityLevel;
    private int mHeight; // In inches
    private int mWeight; // In pounds
    private int mSex; // 0 = female, 1 = male, 2 = none selected
    private Bitmap mProfilePic;

    ProfileViewModel mProfileViewModel;

    /**
     * Required empty constructor
     */
    public EditProfileFragment() {
        // Required empty public constructor
    }


    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {

        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment_edit_profile, container, false);

        // Set up the buttons and register them for clicks
        mSubmitButton = view.findViewById(R.id.button_profile_edit_submit);
        mGetPictureButton = view.findViewById(R.id.button_get_profile_pic);
        mSubmitButton.setOnClickListener(this);
        mGetPictureButton.setOnClickListener(this);

        // Assign all Views as members for easy access within class
        m_etName = view.findViewById(R.id.et_name);
        m_spAge = view.findViewById(R.id.spinner_age);
        m_spCountry = view.findViewById(R.id.spinner_country);
        m_spCountry.setOnItemSelectedListener(this); // So we can get country to populate cities
        m_spCity = view.findViewById(R.id.spinner_city);
        m_spHeight = view.findViewById(R.id.spinner_height);
        m_spWeight = view.findViewById(R.id.spinner_weight);
        m_sexRadioButtons = view.findViewById(R.id.rg_sex);
        m_rbMale = view.findViewById(R.id.radio_male);
        m_rbFemale = view.findViewById(R.id.radio_female);
        m_ivProfilePic = view.findViewById(R.id.iv_profile_pic_profile_edit);
        m_spActivityLevel = view.findViewById(R.id.spinner_activityLevel);

        // Sets the ID's of the radio buttons so that later it can be determined which was clicked.
        m_rbMale.setId(MALE);
        m_rbFemale.setId(FEMALE);

        mProfileViewModel = ViewModelProviders.of(getActivity()).get(ProfileViewModel.class);

        // If user already exists, set their data accordingly
        mProfile = mProfileViewModel.getProfile().getValue();

        // FILL THE SPINNERS WITH DATA
        fillSpinnersWithData();


        if(mProfile != null){


        }

        return view;
    }


    /**
     * Handles when the camera intent returns a profile picture to this fragment.
     */
    @Override
    public void onActivityResult(int requestCode, int resultCode, @Nullable Intent data){
        Bundle extras = data.getExtras();
        mProfilePic = (Bitmap) extras.get("data");
        m_ivProfilePic.setImageBitmap(mProfilePic);
    }


    /**
     * Helper method that sets all of the data into the following Spinners:
     * Age, Height, Weight, and Country.
     */
    private void fillSpinnersWithData() {

        // Set age spinner
        ArrayList<Integer> ageValues = new ArrayList<>();
        for(int i = 12; i < 111; i++){
            ageValues.add(i);
        }
        ArrayAdapter<Integer> ageAdapter = new ArrayAdapter<>(this.getActivity(), android.R.layout.simple_spinner_item, ageValues);
        ageAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        m_spAge.setAdapter(ageAdapter);

        // Set Height spinner (
        ArrayList<String> heightValues = new ArrayList<>();
        for(int feet = 4; feet < 9; feet++){ // account for feet
            String height = feet + "\'";
            heightValues.add(height);
            for(int inches = 1; inches < 12; inches++){ // account for inches
                heightValues.add(height + inches + "\"");
            }
        }
        ArrayAdapter<String> heightAdapter = new ArrayAdapter<>(this.getActivity(), android.R.layout.simple_spinner_item, heightValues);
        heightAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        m_spHeight.setAdapter(heightAdapter);

        // Set Activity spinner
        ArrayList<String> activityLevels = new ArrayList<>();
        activityLevels.add("Sedentary");
        activityLevels.add("Moderate");
        activityLevels.add("Active");
        ArrayAdapter<String> activityAdapter = new ArrayAdapter<>(this.getActivity(), android.R.layout.simple_spinner_item, activityLevels);
        heightAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        m_spActivityLevel.setAdapter(activityAdapter);


        // Set Weight spinner (
        ArrayList<Integer> weightValues = new ArrayList<>();
        for(int i = 50; i < 400; i++){ // account for feet
            weightValues.add(i);
        }
        ArrayAdapter<Integer> weightAdapter = new ArrayAdapter<>(this.getActivity(), android.R.layout.simple_spinner_item, weightValues);
        heightAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        m_spWeight.setAdapter(weightAdapter);

        // Set Country spinner
        String[] countries = getResources().getStringArray(R.array.countries_array); // Countries are in res folder
        ArrayAdapter<String> countryAdapter = new ArrayAdapter<>(this.getActivity(), android.R.layout.simple_spinner_item, countries);
        countryAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        m_spCountry.setAdapter(countryAdapter);
        m_spCountry.setSelection(227); // Sets United States as default country

        // If the user profile isn't null, then set their previous values
        if(mProfile != null){
            // Set the profile picture
            mProfilePic = GeneralUtils.convertImage(mProfile.getImage());
            m_ivProfilePic.setImageBitmap(mProfilePic);

            // Set Name
            m_etName.setText(mProfile.getName());

            // Set Age (-12 because 12 is first possible value)
            m_spAge.setSelection(mProfile.getAge() - 12);

            // Set Country
            int country = Arrays.binarySearch(countries, mProfile.getCountry());
            m_spCity.setSelection(country);

            // Set City
            setCity(mProfile.getCountry());

            // Set Height
            m_spHeight.setSelection(mProfile.getHeight() - 48);

            // Set Weight
            m_spWeight.setSelection(mProfile.getWeight() - 50);

            // Set Activity Level
            if(mProfile.getActivityLevel().equals("Sedentary")){
                m_spActivityLevel.setSelection(0);
            }
            else if(mProfile.getActivityLevel().equals("Moderate")){
                m_spActivityLevel.setSelection(1);
            }
            else{
                m_spActivityLevel.setSelection(2);
            }

            // Set Sex
            if(mProfile.getSex()){
                m_rbMale.setChecked(true);
            }
            else{
                m_rbFemale.setChecked(true);
            }
        }
    }

    /**
     * When a user clicks on the submit button, all of the following data is collected and stored
     * in the member variables: Name, Age, Weight, Height, Country, City, Sex.
     *
     * When the user clicks the profile picture button, switches to the whatever Android OS has
     * determined should be used with a camera to get a picture. Then stores the picture as
     * the user's profile picture.
     */
    @Override
    public void onClick(View view) {

        // Handle both the Image and submit buttons
        switch(view.getId()){

            // Handle the submit button
            case R.id.button_profile_edit_submit:{
                // GET NAME
                mName = m_etName.getText().toString();
                if(mName.equals("")){
                    Toast.makeText(this.getContext(), "Please enter a name.", Toast.LENGTH_SHORT).show();
                    return;
                }
                if (!mName.trim().contains(" ")) {
                    Toast.makeText(this.getContext(), "Please both first and last name.", Toast.LENGTH_SHORT).show();
                    return;
                }
                // Spinners come with a default value, so can't check for user input errors
                mAge = Integer.parseInt(m_spAge.getSelectedItem().toString());
                mWeight = Integer.parseInt((m_spWeight.getSelectedItem().toString()));
                mCountry = m_spCountry.getSelectedItem().toString();
                mCity = m_spCity.getSelectedItem().toString();
                mActivityLevel = m_spActivityLevel.getSelectedItem().toString();

                if (MainActivity.debug) {
                    assert(mAge == 13);
                }
                // GET HEIGHT DATA
                String heightAsString = m_spHeight.getSelectedItem().toString();
                // Replace all non number characters with a space
                heightAsString = heightAsString.replaceAll("[^-?0-9]+", " ");
                // Split all numbers into their own string
                String[] splitHeight = heightAsString.split(" ");
                mHeight = Integer.parseInt(splitHeight[0].trim()) * 12; // feet*inches
                if(splitHeight.length > 1){
                    mHeight += Integer.parseInt(splitHeight[1].trim()); // Add extra inches
                }

                // GET SEX DATA
                // If neither is checked, no assignment made
                boolean sexBoolean;
                if(!m_rbFemale.isChecked() && !m_rbMale.isChecked()){
                    Toast.makeText(this.getContext(), "Please enter your sex.", Toast.LENGTH_SHORT).show();
                    return;
                }
                else{
                    int sexAsInteger = m_sexRadioButtons.getCheckedRadioButtonId();
                    sexBoolean =  (sexAsInteger == 1) ? true : false;
                }

                // Make sure the user has a profile picture
                if(mProfilePic == null){
                    Toast.makeText(this.getContext(), "Please create a profile picture.", Toast.LENGTH_SHORT).show();
                    return;
                }

                // SAVES THIS UPDATE TO THE DATABASE
                UserProfile profile = new UserProfile(mName.trim(), mAge, mWeight, mHeight, mActivityLevel, sexBoolean,
                        mCountry, mCity, GeneralUtils.convertImage(mProfilePic));
                // Response to editing the profile will be different depending on if it's the user's
                // first use of the app or not. We'll check the database for a profile to confirm.
                boolean firstTime = true;
                int ProfilesInDatabase = 0;
                VoidAsyncTask rowsInDB = mProfileViewModel.getNumberOfProfilesInDatabase();
                rowsInDB.execute();
                try {
                     ProfilesInDatabase = (int) rowsInDB.get();
                } catch (ExecutionException e) {
                    e.printStackTrace();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                if( ProfilesInDatabase > 0) {
                    firstTime = false;
                }
                mProfileViewModel.update(profile);

                if(firstTime) {
                    // For the tablet we'll want to set the menu as well as bring up edit goals
                    // For the phone we'll just go to menu.
                    FragmentTransaction ftrans = getActivity().getSupportFragmentManager().beginTransaction();
                    MasterFragment masterFragment = new MasterFragment();
                    if(getResources().getBoolean(R.bool.isTablet)){
                        EditGoalsFragment editGoalsFragment = new EditGoalsFragment();
                        ftrans.replace(R.id.fl_frag_masterlist_container_tablet, masterFragment,"Menu_Fragment");
                        ftrans.replace(R.id.fl_frag_itemdetail_container_tablet, editGoalsFragment,"Edit_Goals_Fragment");
                    }
                    else {
                        ftrans.replace(R.id.fl_frag_masterlist_container_phone, masterFragment, "Menu_Fragment");
                    }
                    ftrans.commit();
                }
                else {
                    // Return to the profile fragment
                    ProfileFragment profileFragment = new ProfileFragment();
                    FragmentTransaction ftrans = getActivity().getSupportFragmentManager().beginTransaction();
                    if(getResources().getBoolean(R.bool.isTablet)) {
                        ftrans.replace(R.id.fl_frag_itemdetail_container_tablet, profileFragment, "Profile_Fragment");
                    }
                    else {
                        ftrans.replace(R.id.fl_frag_masterlist_container_phone, profileFragment, "Profile_Fragment");
                    }
                    ftrans.addToBackStack(null);
                    ftrans.commit();

                }

            } // End submit button case
            break;

            // Handle the profile picture button
            case R.id.button_get_profile_pic:{
                Intent cameraIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);

                if (cameraIntent.resolveActivity(getActivity().getPackageManager()) != null) {
                    startActivityForResult(cameraIntent, REQUEST_IMAGE_CAPTURE);
                }
            } // End profile picture button case
        } // End switch statement
    }

    /**
     * Triggered when a user selects their country. The City Spinner is then
     * populated with the cities that correspond to the selected country.
     */
    @Override
    public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
        // Get the name of the selected country
        String country = (String) parent.getItemAtPosition(position);
        setCity(country);


    }

    /**
     * Sets the city list based on the selected country
     *
     * @param country
     */
    private void setCity(String country) {
        // Set City spinner Based on the selected country
        final int countriesAndCities = R.raw.countries_cities; // ID for json file with cities
        try {
            // Read the JSON file with an InputStream
            InputStream is = getResources().openRawResource(countriesAndCities);
            int size = is.available();
            byte[] buffer = new byte[size];
            is.read(buffer);
            is.close();

            // Get a JSONObject/JSONArray of the needed cities
            String countriesCitiesJSON = new String(buffer, "UTF-8");
            JSONObject jObject = new JSONObject(countriesCitiesJSON);
            JSONArray citiesInJSON = (JSONArray) jObject.get(country);

            // Convert JSONArray to a basic array
            String[] citiesAsArray = new String[citiesInJSON.length()];
            for(int i = 0; i < citiesInJSON.length(); i++){
                citiesAsArray[i] = citiesInJSON.getString(i);
            }
            Arrays.sort(citiesAsArray);

            // Set the city Spinner
            ArrayAdapter<String> cityAdapter = new ArrayAdapter<>(this.getActivity(), android.R.layout.simple_spinner_item, citiesAsArray);
            cityAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
            m_spCity.setAdapter(cityAdapter);

            // Set the correct city if the user already exists
            if(mProfile != null){
                int cityIndex = Arrays.binarySearch(citiesAsArray, mProfile.getCity());
                m_spCity.setSelection(cityIndex);
            }

        } catch (IOException e) { // Reading the file failed
            e.printStackTrace();
        } catch (JSONException e) { // JSON stuff failed
            e.printStackTrace();
        }
    }

    @Override
    public void onNothingSelected(AdapterView<?> parent) {
        return;
    }


    /**
     * Handles lifecycle awareness when the user moves away from this fragment.
     */
    @Override
    public void onSaveInstanceState(Bundle outState){
        // Get all string and int values associated with this fragment
        String name = m_etName.getText().toString();
        int height = m_spHeight.getSelectedItemPosition();
        int country = m_spCountry.getSelectedItemPosition();
        int city = m_spCity.getSelectedItemPosition();
        int age = m_spAge.getSelectedItemPosition();
        int weight = m_spWeight.getSelectedItemPosition();
        int activityLevel = m_spActivityLevel.getSelectedItemPosition();
        int sex;  // 0 = female, 1 = male, 2 = none selected
        if(m_rbFemale.isChecked()){
            sex = 0;
        }
        else if (m_rbMale.isChecked()){
            sex = 1;
        }
        else{
            sex = 2;
        }

        // Store all string and int values
        outState.putString("NAME", name);
        outState.putInt("HEIGHT", height);
        outState.putInt("COUNTRY", country);
        outState.putInt("CITY", city);
        outState.putInt("AGE", age);
        outState.putInt("WEIGHT", weight);
        outState.putInt("SEX", sex);
        outState.putInt("ACTIVITY", activityLevel);

        // Store image if there is one
        if(mProfilePic != null){
            outState.putParcelable("PROFILE_PIC", mProfilePic);
//            ByteArrayOutputStream os = new ByteArrayOutputStream();
//            mProfilePic.compress(Bitmap.CompressFormat.PNG, 100, os);
//            byte[] bArray = os.toByteArray();
//            outState.putByteArray("PROFILE_PIC", bArray);
        }

        //Save the view hierarchy
        super.onSaveInstanceState(outState);
    }


    /**
     * Handles lifecycle awareness when the user returns to this fragment.
     */
    @Override
    public void onViewStateRestored (Bundle savedInstanceState){

        if(savedInstanceState != null){
            // Restore simple saved data
            m_etName.setText(savedInstanceState.getString("NAME"));
            m_spAge.setSelection(savedInstanceState.getInt("AGE"));
            m_spCountry.setSelection(savedInstanceState.getInt("COUNTRY"));
            m_spCity.setSelection(savedInstanceState.getInt("CITY"));
            m_spHeight.setSelection(savedInstanceState.getInt("HEIGHT"));
            m_spWeight.setSelection(savedInstanceState.getInt("WEIGHT"));
            m_spActivityLevel.setSelection(savedInstanceState.getInt("ACTIVITY"));

            // Restore user's sex, 0 = female, 1 = male, 2 = none selected
            int sexValue = savedInstanceState.getInt("SEX");
            if(sexValue == 0){
                m_rbFemale.setChecked(true);
                m_rbMale.setChecked(false);
            }
            else if(sexValue == 1){
                m_rbMale.setChecked(true);
                m_rbFemale.setChecked(false);
            }

            // Restore image if there was one
            if(savedInstanceState.containsKey("PROFILE_PIC")) {
                mProfilePic= (Bitmap) savedInstanceState.getParcelable("PROFILE_PIC");
            }
//            byte[] imageArray = savedInstanceState.getByteArray("PROFILE_PIC");
//            if(imageArray != null){
//                mProfilePic = BitmapFactory.decodeByteArray(imageArray, 0, imageArray.length);
//                m_ivProfilePic.setImageBitmap(mProfilePic);
//            }
        }

        super.onViewStateRestored(savedInstanceState);
    }

}
