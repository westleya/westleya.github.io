package com.example.lifestyleapp;

import android.arch.persistence.db.SupportSQLiteDatabase;
import android.arch.persistence.room.Database;
import android.arch.persistence.room.Room;
import android.arch.persistence.room.RoomDatabase;
import android.content.Context;
import android.os.AsyncTask;
import android.support.annotation.NonNull;

@Database(entities = { UserProfile.class }, version = 1)
public abstract class ProfileRoomDatabase extends RoomDatabase{

    // Defines the DAOs that work with this database.
    public abstract ProfileDao profileDao();

    // Code below prevents having multiple instances of the database opened at the same time.
    private static volatile ProfileRoomDatabase INSTANCE;
    static ProfileRoomDatabase getDatabase(final Context context) {
        if (INSTANCE == null) {
            synchronized (ProfileRoomDatabase.class) {
                if (INSTANCE == null) {
                    INSTANCE = Room.databaseBuilder(context.getApplicationContext(),
                            ProfileRoomDatabase.class, "profile_db")
                            .addCallback(sRoomDatabaseCallback)
                            .build();
                }
            }
        }
        return INSTANCE;
    }


    // Callback for when the database is initialized.
    private static RoomDatabase.Callback sRoomDatabaseCallback =
            new RoomDatabase.Callback(){

                @Override
                public void onOpen (@NonNull SupportSQLiteDatabase db){
                    super.onOpen(db);
                    new PopulateDbAsync(INSTANCE).execute();
                }
            };

    // Task associated with the database callback
    private static class PopulateDbAsync extends AsyncTask<Void, Void, Void> {

        private final ProfileDao mDao;

        PopulateDbAsync(ProfileRoomDatabase db) {
            mDao = db.profileDao();
        }

        @Override
        protected Void doInBackground(final Void... params) {

            // SET DELETE ALL FOR TESTING PURPOSES. CLEARS OUT DATABASE SO IT'S EMPTY
            //mDao.deleteAll();

            // Insert a profile
            //UserProfile profile = new UserProfile("Doug Test", 32, 190, 72, "Sedentary", true, "United States", "Salt Lake City");
            //mDao.insert(profile);

            return null;
        }
    }
}
