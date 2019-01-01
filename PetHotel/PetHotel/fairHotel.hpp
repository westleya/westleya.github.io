//
//  fairHotel.hpp
//  PetHotel
//
//  Created by Westley Kirkham on 3/27/18.
//  Copyright © 2018 Westley Kirkham. All rights reserved.
//

#ifndef fairHotel_h
#define fairHotel_h

class FairHotel {
    int birds, cats, dogs, visitor;
    //sync primitives
    mutable std::mutex mutex;
    std::condition_variable no_cats, no_birds, no_dogs, no_birdsordogs;
    void play() const;
public:
    FairHotel() {
        birds = 0;
        cats = 0;
        dogs = 0;
        visitor = 0;
    }
    void cat();
    void bird();
    void dog();
    
};

#endif /* fairHotel_h */
