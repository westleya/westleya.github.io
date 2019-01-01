//
//  petHotel.hpp
//  PetHotel
//
//  Created by Westley Kirkham on 3/27/18.
//  Copyright © 2018 Westley Kirkham. All rights reserved.
//
//  Cats are favored when there are none of one of the other
//  groups (Dogs xor Birds) or when there are only cats.
//  For any combination including cats, dogs and birds
//  dogs and birds are favored.
//      The sum of dogs and birds likely never runs down to 0.
//  So, the cat threads only get to run once, at the very end.
//  (The total cats typically adds up to the number of cat threads
//  I allocated in the product scheme)

#ifndef petHotel_h
#define petHotel_h

class PetHotel {
    int birds, cats, dogs;
    //sync primitives
    mutable std::mutex mutex;
    std::condition_variable no_cats, no_birdsordogs;
    void play() const;
public:
    PetHotel() {
        birds = 0;
        cats = 0;
        dogs = 0;
    }
    void cat();
    void bird();
    void dog();
    
};


#endif /* petHotel_h */

