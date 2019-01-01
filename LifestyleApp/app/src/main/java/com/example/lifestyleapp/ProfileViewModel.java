package com.example.lifestyleapp;

import android.app.Application;
import android.arch.lifecycle.AndroidViewModel;
import android.arch.lifecycle.LiveData;
import android.os.AsyncTask;

public class ProfileViewModel extends AndroidViewModel {

    private ProfileRepository mRepository;
    private LiveData<UserProfile> mProfile;

    // constructor that gets a reference to the repository and UserProfile from repository.
    public ProfileViewModel (Application application){
        super(application);
        mRepository = new ProfileRepository(application);
        mProfile = mRepository.getUserProfile();
    }

    // Get number of rows in repository
    public VoidAsyncTask getNumberOfProfilesInDatabase(){
        return mRepository.getNumberOfProfilesInDatabase();
    }

    // Gets the user profile
    public LiveData<UserProfile> getProfile() {
        return mProfile;
    }

    // Inserts a user into the database
    public void insert(UserProfile profile){
        mRepository.insert(profile);
    }

    // Updates the current user
    public void update(UserProfile profile){
        mRepository.update(profile);
    }

}
