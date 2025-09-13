#include <iostream>

#ifndef TIMESERIES_H
#define TIMESERIES_H

// Class that contains the data and functions for a desired dataset
class timeSeries{

    // All variables are private
    private:

        // Holds info about series
        std::string seriesName;
        std::string seriesCode;

        // Dynamic arrays that carry time series values
        int* years;
        double* data;

        // Capacity of arrays
        int capacity;
        int currentCapacity;

        // Pointer to point to the next timeSeries for the country
        timeSeries* next;

    // All member functions are public
    public:

        // Constructor and destructor
        timeSeries();
        ~timeSeries();

        // Setter and getter for next pointer
        void set_next(timeSeries*);
        timeSeries* get_next();

        // Getters for series name and code
        std::string get_series_name();
        std::string get_series_code();

        // Initializes the time series using desired file
        void initialize(std::string);

        // Prints time series
        int print();

        // Helper functions for modifying the dynamic arrays
        int find_year(int);
        void shift(int);
        void capacity_update();

        // Changes values in the array based on
        int add_data(int, double);
        int update_data(int, double);

        // Helper function to check if time series has any valid elements
        bool is_valid();

        // Statistical functions that analyze the time series
        double mean();
        bool is_monotonic();
        void best_fit(double &, double &);

};  // End of timeSeries class

#endif
