package com.example.lifestyleapp;

import android.arch.lifecycle.LiveData;
import android.arch.persistence.room.Dao;
import android.arch.persistence.room.Insert;
import android.arch.persistence.room.OnConflictStrategy;
import android.arch.persistence.room.Query;
import android.arch.persistence.room.Update;

/**
 * In the DAO (data access object), you specify SQL queries and associate them with method calls.
 * The compiler checks the SQL and generates queries from convenience annotations for common
 * queries, such as @Insert.
 *
 * The DAO must be an interface or abstract class.
 *
 * By default, all queries must be executed on a separate thread.
 *
 * Room uses the DAO to create a clean API for your code.
 */
@Dao
public interface ProfileDao {

    @Insert(onConflict = OnConflictStrategy.REPLACE) // If try to insert existing item, replaces it
    void insert(UserProfile profile);

    // Deletes all profiles from database
    @Query("DELETE FROM profile_data_table")
    void deleteAll();

    // Updates an existing profile
    @Update
    void update(UserProfile profile);

    // Selects a UserProfile by their name
    @Query("SELECT * FROM profile_data_table where name = :name")
    LiveData<UserProfile> getProfile(String name);

    // Gets the number of rows in the table
    @Query("SELECT COUNT(*) FROM profile_data_table ")
    int getNumberOfProfilesInDatabase();

    // Get the first entry from the table
    @Query("SELECT * FROM profile_data_table LIMIT 1")
    LiveData<UserProfile> getFirstProfile();

}
