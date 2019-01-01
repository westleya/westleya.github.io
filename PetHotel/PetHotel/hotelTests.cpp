#include <thread>
#include <vector>
#include <iostream>
#include <cassert>

//#include "petHotel.hpp"
#include "fairHotel.hpp"

//DECLARE THIS AS A PRIVATE METHOD IN YOUR HOTEL CLASS
/*
void PetHotel::play() const {
  for(volatile int i = 0; i < 10000; ++i) {} //use the CPU for a while
  std::this_thread::sleep_for(std::chrono::milliseconds(1)); //take a nap
	
}

void PetHotel::cat() {
    {
        std::unique_lock<std::mutex> lock(mutex);
        while(birds + dogs != 0){
            no_birdsordogs.wait(lock);
        }
        assert(birds + dogs == 0);
        cats += 1;
    }
    
    play();
    
    {
        std::unique_lock<std::mutex> lock(mutex);
        cats -= 1;
        if (cats == 0) {
            no_cats.notify_all();
        }
    }
}

void PetHotel::bird() {
    {
        std::unique_lock<std::mutex> lock(mutex);
        while(cats != 0 ){
            no_cats.wait(lock);
        }
        assert(cats == 0);
        birds += 1;
    }
    
    play();
    
    {
        std::unique_lock<std::mutex> lock(mutex);
        birds -= 1;
        if (dogs + birds == 0) {
            no_birdsordogs.notify_all();
        }
    }
}


void PetHotel::dog() {
    {
        std::unique_lock<std::mutex> lock(mutex);
        while(cats != 0 ){
            no_cats.wait(lock);
        }
        assert(cats == 0);
        dogs += 1;
    }
    
    play();
    
    {
        std::unique_lock<std::mutex> lock(mutex);
        dogs -= 1;
        if (dogs + birds == 0) {
            no_birdsordogs.notify_all();
        }
    }
}
*/
//UNCOMMENT ONCE YOU IMPLEMENT A FAIR HOTEL

void FairHotel::play() const {
  for(volatile int i = 0; i < 10000; ++i) {} //use the CPU for a while
  std::this_thread::sleep_for(std::chrono::milliseconds(1)); //take a nap
	
}

void FairHotel::cat() {
    {
        std::unique_lock<std::mutex> lock(mutex);
        while(birds + dogs != 0 || visitor == 0){
            no_birdsordogs.wait(lock);
        }
        assert(birds + dogs == 0);
        cats += 1;
    }
    
    play();
    
    {
        std::unique_lock<std::mutex> lock(mutex);
        cats -= 1;
        if (cats == 0) {
            no_cats.notify_all();
        }
        visitor = 0;
    }
}

void FairHotel::bird() {
    {
        std::unique_lock<std::mutex> lock(mutex);
        while(cats != 0 || visitor == 1){
            no_cats.wait(lock);
        }
        assert(cats == 0);
        birds += 1;
    }
    
    play();
    
    {
        std::unique_lock<std::mutex> lock(mutex);
        birds -= 1;
        if (birds == 0) {
            no_birdsordogs.notify_all();
        }
        visitor = 1;
    }
}


void FairHotel::dog() {
    {
        std::unique_lock<std::mutex> lock(mutex);
        while(cats != 0 || visitor == 1){
            no_cats.wait(lock);
        }
        assert(cats == 0);
        dogs += 1;
    }
    
    play();
    
    {
        std::unique_lock<std::mutex> lock(mutex);
        dogs -= 1;
        if (dogs == 0) {
            no_birdsordogs.notify_all();
        }
        visitor = 1;
    }
}

void birdThread(FairHotel& hotel, int& counter, std::atomic<bool>& done){
  while(!done){
	hotel.bird();
	++counter;
  }
}
void dogThread(FairHotel& hotel, int& counter, std::atomic<bool>& done){
  while(!done){
	hotel.dog();
	++counter;
  }
}
void catThread(FairHotel& hotel, int& counter, std::atomic<bool>& done){
  while(!done){
	hotel.cat();
	++counter;
  }
}

int main(int argc, char**argv){
  if(argc != 4){
	std::cout << "usage:  ./petHotel numBirds, numCats, numDogs" << std::endl;
	return 1;
  }
  int numBirds(std::atoi(argv[1]));
  int numCats(std::atoi(argv[2]));
  int numDogs(std::atoi(argv[3]));
  std::vector<int> birdCounts(numBirds), catCounts(numCats), dogCounts(numDogs);

  std::vector<std::thread> threads;
  threads.reserve(numBirds + numCats + numDogs);
  
  auto now = std::chrono::high_resolution_clock::now();
  auto stopTime = now + std::chrono::seconds(5);
  std::atomic<bool> done{false};
  //PetHotel hotel;
  FairHotel hotel;
  
  for(int i = 0; i < numBirds; ++i){
	threads.push_back(std::thread(birdThread,
			std::ref(hotel),
			std::ref(birdCounts[i]),
			std::ref(done)));
  }
  for(int i = 0; i < numCats; ++i){
	threads.push_back(std::thread(catThread,
			std::ref(hotel),
			std::ref(catCounts[i]),
			std::ref(done)));
  }
  for(int i = 0; i < numDogs; ++i){
	threads.push_back(std::thread(dogThread,
			std::ref(hotel),
			std::ref(dogCounts[i]),
			std::ref(done)));
  }

  std::this_thread::sleep_for(std::chrono::seconds(2));
  done = true;
  std::cout << "OK, waiting for threads to quit" << std::endl;
  for(auto& t : threads){
	t.join();
  }

  std::cout << "bird counts" << std::endl;
  int birdSum = 0;
  for(auto & c : birdCounts){
	std::cout << c << std::endl;
	birdSum += c;
  }
  std::cout << "cat counts" << std::endl;
  int catSum = 0;
  for(auto & c : catCounts){
	std::cout << c << std::endl;
	catSum += c;
  }

  std::cout << "dog counts" << std::endl;
  int dogSum = 0;
  for(auto & c : dogCounts){
	std::cout << c << std::endl;
	dogSum += c;
  }

  std::cout << "total birds " << birdSum << std::endl;
  std::cout << "total cats " << catSum << std::endl;
  std::cout << "total dogs " << dogSum << std::endl;
  
  
  
  return 0;
}

