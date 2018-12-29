//
//  simulator.cpp
//  Discrete_Event_Simulator
//
//  Created by Westley Kirkham on 5/1/18.
//  Copyright © 2018 Westley Kirkham. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <math.h>

//./simulator bank
//./simulator supermarket

bool BANK = true;
int RUNTIME = 100000000; //100,000,000 for more stable stats. Use -O3 to g++ or clang++
int NUM_TELLERS = 10;
int ARRIVAL_INTERVAL = 32;
int IDLE_TIME = 0;

// Used to indicate what type of event is occurring
enum Action {
    arrival = 0,
    waiting = 1,
    finish = 2,
};

struct Customer {
    int wait_time;
    int time_to_handle;
    int arrival_time;
    bool served;
};

// Only the tellers for the supermarket utilize the line.
// For the bank there's a line declared as part of its implementation.
struct Teller {
    bool occupied;
    Customer customer;
    int finish_time;
    std::deque<Customer> line; // used for push_back and pop_front capabilities
};

struct Event {
    int firing_time;
    int type;
    Customer customer;
    int num_teller;
public:
    bool operator<(const Event& rhs) const {
        return firing_time > rhs.firing_time;
    }
};

// Services customers using one line with 10 tellers operating that single line.
void runBank(std::vector<Customer> customers, std::vector<Teller> tellers) {
    int current_time = 0;
    
    std::priority_queue<Event, std::vector<Event>, std::less<Event> > queue;
    // Prefill queue with all expected customers.
    for(Customer customer: customers) {
        Event new_event;
        new_event.firing_time = customer.arrival_time;
        new_event.type = arrival;
        new_event.customer = customer;
        queue.push(new_event);
    }
    
    std::deque<Customer> line; // used for push_back and pop_front capabilities
    std::vector<Customer> customers_served; // Easier to store changes to customers this way
    
    // Run the bank
    while(current_time <= RUNTIME && !queue.empty()) {
        Event curr_event = queue.top();
        
        if (curr_event.type == arrival) {
            int soonest = 600;
            // Check for a free teller when a customer arrives.
            for(int i = 0; i < NUM_TELLERS; i++) {
                if(!tellers[i].occupied) {
                    
                    tellers[i].occupied = true;
                    tellers[i].customer = curr_event.customer;
                    tellers[i].customer.wait_time = 0;
                    tellers[i].customer.served = true;
                    tellers[i].finish_time = current_time + tellers[i].customer.time_to_handle;
                    curr_event.customer = tellers[i].customer;
                    
                    Event new_event;
                    new_event.firing_time = tellers[i].finish_time;
                    new_event.type = finish;
                    new_event.customer = tellers[i].customer;
                    new_event.num_teller = i;
                    queue.push(new_event);
                    
                    break;
                }
                soonest = (tellers[i].finish_time - current_time) < soonest ? (tellers[i].finish_time - current_time): soonest;
            }
            // If all the tellers are occupied the customer will have to be added to a line and wait until
            // they can be served, the soonest being when the next teller finishes.
            if (!curr_event.customer.served) {
                line.push_back(curr_event.customer);
                Event new_event;
                new_event.firing_time = current_time + soonest;
                new_event.type = waiting;
                queue.push(new_event);
            }
        }
        else if(curr_event.type == waiting) {
            int soonest = 600;
            Customer customer = line.front();
            // Check for a free teller once the expected wait time expires.
            for(int i = 0; i < NUM_TELLERS; i++) {
                if(!tellers[i].occupied) {
                    
                    tellers[i].occupied = true;
                    tellers[i].customer = customer;
                    tellers[i].customer.wait_time = current_time - customer.arrival_time;
                    tellers[i].customer.served = true;
                    tellers[i].finish_time = current_time + customer.time_to_handle;
                    curr_event.customer = tellers[i].customer;
                    
                    Event new_event;
                    new_event.firing_time = tellers[i].finish_time;
                    new_event.type = finish;
                    new_event.customer = customer;
                    new_event.num_teller = i;
                    queue.push(new_event);
                    
                    break;
                }
                soonest = (tellers[i].finish_time - current_time) < soonest ? (tellers[i].finish_time - current_time): soonest;
            }
            // This is necessary because it's possible that a 'waiting' event queued for a
            // teller finishing might fire before the code has run to clear that teller of
            // its customer.
            if (!curr_event.customer.served) {
                Event new_event;
                new_event.firing_time = current_time + soonest;
                new_event.type = waiting;
                queue.push(new_event);
            }
            else {
                line.pop_front();
            }
        }
        else { // A teller finishes
            tellers[curr_event.num_teller].occupied = false;
            customers_served.push_back(curr_event.customer);
        }
        
        queue.pop();
        bool tellers_all_occ = true;
        for (Teller teller: tellers) {
            tellers_all_occ &= teller.occupied;
        }
        if(!tellers_all_occ){
            IDLE_TIME += queue.top().firing_time - current_time;
        }
        current_time = queue.top().firing_time;
    }
    
    // Calculate report values
    int num_cust_served = 0;
    float percent_idle_time = ((float) IDLE_TIME / (float) RUNTIME) * 100;
    int sum_times = 0;
    for(Customer customer: customers_served) {
        if(customer.served) {
            num_cust_served++;
            sum_times += customer.time_to_handle + customer.wait_time;
        }
    }
    float avg_service_time = (float) sum_times / (float) num_cust_served;
    float variance = 0;
    float sum_var = 0;
    for(Customer customer: customers_served) {
        sum_var += pow((customer.time_to_handle + customer.wait_time - avg_service_time), 2);
    }
    variance = sum_var / (num_cust_served - 1);
    
    std::cout << "------BANK------\n";
    std::cout << "The following report is for a work day extending " << RUNTIME / 3600 << " hours.\n";
    std::cout << "It also assumes " << NUM_TELLERS << " tellers and customers arriving every \n";
    std::cout << ARRIVAL_INTERVAL << " seconds. Only customers who received service are counted.\n\n";
    std::cout << "Report: \n";
    std::cout << "Average customer service time (enter to exit): " << avg_service_time << "\n";
    std::cout << "Variance in customer service time: " << variance << "\n";
    std::cout << "Standard deviation in service time: " << sqrt(variance) << "\n";
    std::cout << "Total number of customers served: " << num_cust_served << "\n";
    std::cout << "Percent of the day that at least one teller was idle: " << percent_idle_time << "%\n";
    std::cout << "END\n";
    
}

//Services customers using ten lines with one teller operating each line.
void runSuperMarket(std::vector<Customer> customers, std::vector<Teller> tellers) {
    
    int current_time = 0;
    
    std::priority_queue<Event, std::vector<Event>, std::less<Event> > queue;
    // Prefill queue with all expected customers.
    for(Customer customer: customers) {
        Event new_event;
        new_event.firing_time = customer.arrival_time;
        new_event.type = arrival;
        new_event.customer = customer;
        queue.push(new_event);
    }
    
    std::deque<Customer> lines[NUM_TELLERS];
    std::vector<Customer> customers_served; // Easier to store changes to customers this way
    
    // Run the supermarket
    while(current_time <= RUNTIME && !queue.empty()) {
        Event curr_event = queue.top();
        
        if (curr_event.type == arrival) {
            int soonest = 600;
            // Check for a free teller when a customer arrives.
            for(int i = 0; i < NUM_TELLERS; i++) {
                if(!tellers[i].occupied) {
                    
                    tellers[i].occupied = true;
                    tellers[i].customer = curr_event.customer;
                    tellers[i].customer.wait_time = 0;
                    tellers[i].customer.served = true;
                    tellers[i].finish_time = current_time + tellers[i].customer.time_to_handle;
                    curr_event.customer = tellers[i].customer;
                    
                    Event new_event;
                    new_event.firing_time = tellers[i].finish_time;
                    new_event.type = finish;
                    new_event.customer = tellers[i].customer;
                    new_event.num_teller = i;
                    queue.push(new_event);
                    
                    break;
                }
                soonest = (tellers[i].finish_time - current_time) < soonest ? (tellers[i].finish_time - current_time): soonest;
            }
            // If all the tellers are occupied the customer will have to be added to a line and wait until
            // they can be served, the soonest being when the next teller finishes.
            if (!curr_event.customer.served) {
                int line = 0;
                for (int i = 1; i < NUM_TELLERS; i++) {
                    if (lines[i].size() < lines[line].size()) {
                        line = i;
                    }
                }
                lines[line].push_back(curr_event.customer);
                Event new_event;
                new_event.firing_time = current_time + soonest;
                new_event.type = waiting;
                new_event.num_teller = line;
                queue.push(new_event);
            }
        }
        else if(curr_event.type == waiting) {
            int soonest = 600;
            Customer customer = lines[curr_event.num_teller].front();
            // Check for a free teller once the expected wait time expires.
            for(int i = 0; i < NUM_TELLERS; i++) {
                if(!tellers[i].occupied) {
                    
                    tellers[i].occupied = true;
                    tellers[i].customer = customer;
                    tellers[i].customer.wait_time = current_time - customer.arrival_time;
                    tellers[i].customer.served = true;
                    tellers[i].finish_time = current_time + customer.time_to_handle;
                    curr_event.customer = tellers[i].customer;
                    
                    Event new_event;
                    new_event.firing_time = tellers[i].finish_time;
                    new_event.type = finish;
                    new_event.customer = customer;
                    new_event.num_teller = i;
                    queue.push(new_event);
                    
                    break;
                }
                soonest = (tellers[i].finish_time - current_time) < soonest ? (tellers[i].finish_time - current_time): soonest;
            }
            // This is necessary because it's possible that a 'waiting' event queued for a
            // teller finishing might fire before the code has run to clear that teller of
            // its customer.
            if (!curr_event.customer.served) {
                Event new_event;
                new_event.firing_time = current_time + soonest;
                new_event.type = waiting;
                queue.push(new_event);
            }
            else {
                lines[curr_event.num_teller].pop_front();
            }
        }
        else { // A teller finishes
            tellers[curr_event.num_teller].occupied = false;
            customers_served.push_back(curr_event.customer);
        }
        
        queue.pop();
        bool tellers_all_occ = true;
        for (Teller teller: tellers) {
            tellers_all_occ &= teller.occupied;
        }
        if(!tellers_all_occ){
            IDLE_TIME += queue.top().firing_time - current_time;
        }
        current_time = queue.top().firing_time;
    }
    
    // Calculate report values
    int num_cust_served = 0;
    float percent_idle_time = ((float) IDLE_TIME / (float) RUNTIME) * 100;
    int sum_times = 0;
    for(Customer customer: customers_served) {
        if(customer.served) {
            num_cust_served++;
            sum_times += customer.time_to_handle + customer.wait_time;
        }
    }
    float avg_service_time = (float) sum_times / (float) num_cust_served;
    float variance = 0;
    float sum_var = 0;
    for(Customer customer: customers_served) {
        sum_var += pow((customer.time_to_handle + customer.wait_time - avg_service_time), 2);
    }
    variance = sum_var / (num_cust_served - 1);
    
    std::cout << "------SUPERMARKET------\n";
    std::cout << "The following report is for a work day extending " << RUNTIME / 3600 << " hours.\n";
    std::cout << "It also assumes " << NUM_TELLERS << " tellers and customers arriving every \n";
    std::cout << ARRIVAL_INTERVAL << " seconds. Only customers who received service are counted.\n\n";
    std::cout << "Report: \n";
    std::cout << "Average customer service time (enter to exit): " << avg_service_time << "\n";
    std::cout << "Variance in customer service time: " << variance << "\n";
    std::cout << "Standard deviation in service time: " << sqrt(variance) << "\n";
    std::cout << "Total number of customers served: " << num_cust_served << "\n";
    std::cout << "Percent of the day that at least one teller was idle: " << percent_idle_time << "%\n";
    std::cout << "END\n";
    
}

int main(int argc, const char * argv[]) {

    if (argc < 2 ) {
        std::cout << "Error! Execution of the form ./a.out bank or ./a.out supermarket expected.\n";
        std::cout << "Did not receive command.\n";
        return 0;
    }
    std::string bankOrMarket = argv[1];
    if (bankOrMarket.compare("bank") != 0 && bankOrMarket.compare("supermarket") != 0) {
        std::cout << "Error! Execution of the form ./a.out bank or ./a.out supermarket expected.\n";
        std::cout << "received errant command: ./a.out " << bankOrMarket << "\n";
        return 0;
    }
    
    BANK = (bankOrMarket.compare("bank") == 0);
    std::vector<Customer> customers;
    srand(time(NULL));
    
    // Fill the vector of customers.
    for(int i = 0; i <= RUNTIME; i += ARRIVAL_INTERVAL) {
        Customer new_customer;
        new_customer.wait_time = 0;
        new_customer.time_to_handle = (rand() % 571) + 30; // Change ...+ NUM2; to change the minimum time_to_handle.
        new_customer.arrival_time = i;                     // Change ...% NUM1) to change maximum to NUM1 + NUM2 - 1.
        new_customer.served = false;
        customers.push_back(new_customer);
    }
    
    std::vector<Teller> tellers;
    for(int i = 0; i < NUM_TELLERS; i++) {
        Teller new_teller;
        new_teller.occupied = false;
        new_teller.finish_time = 0;
        tellers.push_back(new_teller);
    }
    
    if (BANK) {
        runBank(customers, tellers);
    }
    else {
        runSuperMarket(customers, tellers);
    }
    
    return 0;
}
