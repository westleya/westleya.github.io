package com.example.lifestyleapp;

import android.os.AsyncTask;

/**
 * The sole purpose of this task is to be able to call .execute on a rowsInDatabaseTask
 * from within the MainActivity. Explination here:
 *
 * https://stackoverflow.com/questions/20455644/object-cannot-be-cast-to-void-in-asynctask
 *
 * Other Helpful sources:
 * https://stackoverflow.com/questions/10005353/can-onpostexcecute-method-in-asynctask-return-values
 * https://stackoverflow.com/a/10115881/10305189
 *
 * @param <T>
 */
public abstract class VoidAsyncTask<T> extends AsyncTask<Void, Void, T> {
    public void execute() {
        super.execute();
    }
}
