package com.example.lifestyleapp;

import android.content.Context;
import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.ArrayList;

public class RCViewAdapter extends RecyclerView.Adapter<RCViewAdapter.ViewHolder>{

    // Member variables
    private Context mContext;
    private DataPasser mDataPasser;
    private ArrayList<String> mMenuItem, mMenuDescriptions;

    /**
     * Constructor
     * @param mMenuTitle - List of fragment_detail items
     * @param menuDescriptions - Description of fragment_detail items
     */
    public RCViewAdapter(ArrayList<String> mMenuTitle, ArrayList<String> menuDescriptions) {
        this.mMenuItem = mMenuTitle;
        this.mMenuDescriptions = menuDescriptions;
    }


    @NonNull
    @Override
    public RCViewAdapter.ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        mContext = parent.getContext();

        // Associate the callback with this Fragment
        try{
            mDataPasser = (DataPasser) mContext;
        }
        catch(ClassCastException e){
            throw new ClassCastException(mContext.toString()+ " must implement OnDataPass");
        }

        LayoutInflater layoutInflater = LayoutInflater.from(mContext);
        View myView = layoutInflater.inflate(R.layout.fragment_detail, parent,false);
        return new ViewHolder(myView);
    }


    public class ViewHolder extends RecyclerView.ViewHolder{
        protected View itemLayout;
        protected TextView itemTvMenu, itemTvDes;
        protected ImageView itemIvImage;

        public ViewHolder(View view){
            super(view);
            itemLayout = view;
            itemTvMenu = view.findViewById(R.id.tv_menu);
            itemTvDes = view.findViewById(R.id.tv_menu_description);
            itemIvImage = view.findViewById(R.id.menu_item_icon);
        }
    }


    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, final int position) {

        // Set the menu items
        holder.itemTvMenu.setText(mMenuItem.get(position));
        holder.itemTvDes.setText(mMenuDescriptions.get(position));
        // Set the images for each menu item
        if(position == 0){
            holder.itemIvImage.setImageResource(R.drawable.goals);
        }
        else if (position == 1){
            holder.itemIvImage.setImageResource(R.drawable.weather);
        }
        else if (position == 2){
            holder.itemIvImage.setImageResource(R.drawable.hiking);
        }
        else {
            holder.itemIvImage.setImageResource(R.drawable.steps);
        }


        // Controls what happens when an item in the list has been clicked
        holder.itemLayout.setOnClickListener(new View.OnClickListener(){
                                                 @Override
                                                 public void onClick(View view) {
                                                     mDataPasser.passData(position);
                                                 }
                                             }
        );
    }


    /**
     * Gets the number of items in the fragment_detail
     */
    @Override
    public int getItemCount() {
        return mMenuItem.size();
    }

    /**
     * Interface for the MainActivity
     */
    public static interface DataPasser{
        public void passData(int position);
    }
}
