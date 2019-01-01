package com.example.lifestyleapp;

import android.arch.persistence.room.ColumnInfo;
import android.arch.persistence.room.Entity;
import android.arch.persistence.room.PrimaryKey;
import android.support.annotation.NonNull;

import java.io.Serializable;

@Entity(tableName = "profile_data_table") // Needed for Room database
public class UserProfile implements Serializable{

    // Because our app only supports one user right now, just use custom, constant key
    @PrimaryKey
    @NonNull
    @ColumnInfo(name = "id")
    private int id;

    // User Stats Data
    @NonNull
    @ColumnInfo(name = "name")
    private String name;
    @NonNull
    @ColumnInfo(name = "age")
    private int age;
    @NonNull
    @ColumnInfo(name = "city")
    private String city;
    @NonNull
    @ColumnInfo(name = "country")
    private String country;
    @NonNull
    @ColumnInfo(name = "height")
    private int height; // in inches
    @NonNull
    @ColumnInfo(name = "weight")
    private int weight; // in lbs
    @NonNull
    @ColumnInfo(name = "gender")
    private boolean sex; // M=true, F=false
    @NonNull
    @ColumnInfo(name = "activityLevel")
    private String activityLevel; // Sedentary (1.53), Moderate (1.76), Active (2.25)
    @NonNull
    @ColumnInfo(name = "picture")
    private byte[] image;

    // User Goal Related Data
    @NonNull
    @ColumnInfo(name = "goal")
    private String goal; // Lose, maintain, or gain weight
    @NonNull
    @ColumnInfo(name = "targetWeight")
    private int targetWeight; // in lbs
    @NonNull
    @ColumnInfo(name = "poundsPerWeek")
    private double poundsPerWeek; // In lbs per week
    @NonNull
    @ColumnInfo(name = "steps")
    private float steps; // an int
    @NonNull
    @ColumnInfo(name = "startSteps")
    private float startSteps; // an int

    // Constructor for getting data from EditProfile
    public UserProfile(String name, int age, int weight, int height, String activityLevel, boolean sex, String country,
                       String city, byte[] image){

        id = 1;

        this.name = name;
        this.age = age;
        this.city = city;
        this.country = country;
        this.height = height;
        this.weight = weight;
        this.sex = sex;
        this.activityLevel = activityLevel;
        this.image = image;

        // Set these values later once the user has defined a health/fitness goal.
        // Will be initially set to 0/empty values to denote that no goal has been defined
        goal = "";
        targetWeight = 0;
        poundsPerWeek = 0;
        steps = 0;
        startSteps = 0;

    }


    // Getters
    public int getId() { return id; }
    public String getActivityLevel() { return activityLevel; }
    public double getPoundsPerWeek() { return poundsPerWeek; }
    public int getAge() { return age; }
    public int getHeight() { return height; }
    public int getWeight() { return weight; }
    public int getTargetWeight(){return targetWeight; }
    public String getCity() { return city; }
    public String getCountry() { return country; }
    public String getName() { return name; }
    public boolean getSex() { return sex;}
    public String getGoal(){ return goal; }
    public byte[] getImage() { return image; }
    public float getSteps() {return steps;}
    public float getStartSteps() { return startSteps; }

    // Setters
    public void setId(int id ) {this.id = id; }
    public void setName(String name) {this.name = name; }
    public void setAge(int age) {this.age = age; }
    public void setCity(String city) {this.city = city; }
    public void setCountry(String country) {this.country = country; }
    public void setHeight(int height) {this.height = height; }
    public void setWeight(int weight) {this.weight = weight; }
    public void setSex(boolean gender) {this.sex = gender; }
    public void setGoal(String goal) { this.goal = goal; }
    public void setTargetWeight(int weight){ targetWeight = weight; }
    public void setPoundsPerWeek(double poundsPerWeek) { this.poundsPerWeek = poundsPerWeek; }
    public void setActivityLevel(String activityLevel) { this.activityLevel = activityLevel; }
    public void setImage(byte[] image) {this.image = image; }
    public void setSteps( float steps) { this.steps = steps; }
    public void setStartSteps( float startSteps) { this.startSteps = startSteps; }
}
