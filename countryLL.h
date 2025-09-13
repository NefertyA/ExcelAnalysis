#include "timeSeries.h"

#include <iostream>

#ifndef COUNTRYLL_H
#define COUNTRYLL_H

class countryLL{

    // All variables are private
    private:

        // Properties of country
        std::string countryName;
        std::string countryCode;

        // Head and tail pointer
        timeSeries* head;
        timeSeries* tail;

        // Mean for a specific series
        double seriesMean;

        // Specifies whether this linked list is valid or not (iniitalized or not)
        bool wasDeleted;

    // All functions are public
    public:

        // Constructor and destructor
        countryLL();
        ~countryLL();

        // Getter for seriesMean
        double get_mean(std::string);

        // Getter for wasDeleted
        bool get_wasDeleted();

        // Determines if this is valid
        bool is_valid(std::string);

        // Getters for country name and code
        std::string get_countryName();
        std::string get_countryCode();

        // Initializing and deleting time series
        void load(std::string);
        void load(std::string, std::string);
        void erase();

        // Lists the time series
        void list();

        // Helper function to find a desired time series
        timeSeries* find_time_series(std::string);

        // Functions for different commands
        bool add(std::string, int, double);
        bool update(std::string, int, double);
        bool print(std::string);
        bool del(std::string);
        std::string biggest();

};  // End of class

#endif
