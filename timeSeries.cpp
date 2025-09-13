#include "timeSeries.h"

#include <iostream>
#include <fstream>
#include <sstream>

// Constructor
timeSeries::timeSeries(){

    capacity = 2;           // Capacity is always at least 2
    currentCapacity = 0;    // No elements yet

    // Set aside memory for first 2 time series elements
    years = new int[capacity];
    data = new double[capacity];

    // Initialize next pointer to null
    next = nullptr;

}   // End of constructor

// Destructor
timeSeries::~timeSeries(){

    // Deallocates memory
    delete[] years;
    delete[] data;

}   // End of destructor

// Getter for next pointer (since it's private)
timeSeries* timeSeries::get_next(){
    return next;
}

// Setter for next pointer (since it's private)
void timeSeries::set_next(timeSeries* nextTimeSeries){
    next = nextTimeSeries;
}

// Getter for series name (since it's private)
std::string timeSeries::get_series_name(){
    return seriesName;
}

// Getter for series code (since it's private)
std::string timeSeries::get_series_code(){
    return seriesCode;
}

// Initializes time series with desired data from the file (just one line)
void timeSeries::initialize(std::string fileLine){

    std::stringstream parse(fileLine);
    std::string element;

    // Set count to -5 so it can skip the first few data elements in
    // the file (including country name, code, etc.)
    int count(-5);

    // Until the end of the data line, read each element separated by commas
    while (std::getline(parse, element, ',')){

        ++count;

        if (count == -2){           // When count is -2, we are at the series name element
            seriesName = element;
        } else if (count == -1) {   // When count is -1, we are at the series code element
            seriesCode = element;
        }

        // We can ignore the other previous elements for now
        if (count <= -1) continue;

        // Once count reaches 0, we will start adding elements. So:
            // Increase the initialized size of the array
            // Run the update function to determine if the maximum capacity of the array must be updated
        ++currentCapacity;
        capacity_update();

        /*
            CITATION:
            The next 3 lines were generated using chatgpt.com using the prompt: "how to turn string into integer c++".
            ChatGPT gave me several solutions but I picked the stringstream option since I was already familiar with it.
            This is the reason why I place the string variable (line) into a stringstream and then insert it into its
            respective index in the double array (data[index]).
        */

        // Place this string value into a stringstream so we can shove it into an array of type double
        std::stringstream dataElement(element);

        years[count] = 1960 + count;    // This slot of the years array will be 1960 + count
        dataElement >> data[count];     // This slot of the data array is the current element from the file

    }   // End of while loop

}   // End of initialize function

// Prints the current state of the time series
int timeSeries::print(){

    // If the time series has no valid data, return 0 (false)
    if (!is_valid()){
        return 0;
    }

    // Otherwise, cycle through and print all the initialized value pairs
    for (int i(0); i < currentCapacity; ++i){
        if (data[i] != -1){
            std::cout << "(" << years[i] << "," << data[i] << ") ";
        }
    }

    std::cout << std::endl;

    return 1;   // Though this is not used, return true to be safe

}   // End of print function

// Checks to see if the maximum capacity of the array should be updated
// If so, updates the arrays to match the maximum capacity
void timeSeries::capacity_update(){

    // Holds the current maximum capacity
    int tempCapacity(capacity);

    // Compare the number of initialized elements in the array with the max capacity
    if (currentCapacity == tempCapacity){                  // If equal, double the max capacity
        tempCapacity *= 2;
    } else if (currentCapacity <= 0.25*tempCapacity){      // If array is less than a quarter full, halve it down to a minimum of 2
        if (tempCapacity / 2 <= 2){
            tempCapacity = 2;
        } else {
            tempCapacity /= 2;
        }
    } else {        // If the array isn't full or three-quarters empty, just terminate the function
        return;
    }

    // At this point, the arrays need to be updated
    // Create temporary arrays to hold the current time series elements
    int tempYears[tempCapacity];
    double tempData[tempCapacity];

    // Initialize all the values in the temp arrays
    for (int i(0); i < tempCapacity; ++i){
        tempYears[i] = -1;
        tempData[i] = -1;
    }

    // Copy the time series elements into the temp arrays
    for (int i(0); i < currentCapacity; ++i){
        tempYears[i] = years[i];
        tempData[i] = data[i];
    }

    // Deallocate memory
    delete[] years;
    delete[] data;

    // Update max capacity
    capacity = tempCapacity;

    // Allocate new memory
    years = new int[capacity];
    data = new double[capacity];

    // Copy old data into the new time series arrays
    for (int i(0); i < capacity; ++i){
        years[i] = tempYears[i];
        data[i] = tempData[i];
    }
    
} // End of capacity_update function

// Determines if the time series has any valid data
bool timeSeries::is_valid(){

    // Runs through all elements in the time series
    for (int i(0); i < currentCapacity; ++i){
        if (data[i] != -1) return true;     // If even one of them is valid, immediately return true
    }

    return false;   // If function hasn't terminated by now, the dataset is invalid

}   // End of is_valid function

// Finds the index of a desired year
int timeSeries::find_year(int Y){
    
    // Cycles through the entire array and returns the index once found
    for (int i(0); i < currentCapacity; ++i){
        if (years[i] == Y) return i;
    }

    return -1;  // If not found, returns an invalid index

}   // End of find_year function

// Shifts the entire array to the right after some index
void timeSeries::shift(int index){

    // Working backwards, shifts all elements to the right until a certain point
    for (int i(currentCapacity-1); i > index; --i){
        years[i] = years[i-1];
        data[i] = data[i-1];
    }

}   // End of shift function

// Adds a desired data element to the time series
int timeSeries::add_data(int Y, double D){

    // Tries to find the year in the time series
    int yearIndex(find_year(Y));

    if (yearIndex != -1){              // If the year already exists:
        if (data[yearIndex] != -1){
            return 0;                   // 1. With VALID data, do nothing and terminate function
        } else {
            data[yearIndex] = D;
            return 1;                   // 2. With INVALID data, enter new value and terminate function
        }
    }

    // If year doesn't exist, we will be adding a new one
    // So, update number of elements and the max capacity of the time series
    ++currentCapacity;
    capacity_update();

    // The index where we will put the new data element
    int newYearIndex;

    // If the new year is the smallest one so far
    if (Y < years[0]){
        newYearIndex = 0;
    }

    // If year is the largest one so far
    if (Y > years[currentCapacity-2]){
        newYearIndex = currentCapacity-2;
    }

    // If the year is somewhere inbetween
    for (int i(0); i < currentCapacity-1; ++i){
        // Find the index where the year fits as per the increasing order of the array
        if (Y > years[i] && Y < years[i+1]){
            newYearIndex = i+1;
            break;
        }
    }

    // Shift arrays right from desired index
    shift(newYearIndex);
    
    // Update arrays with the new values
    years[newYearIndex] = Y;
    data[newYearIndex] = D;

    return 1;   // Return true as shift was successful

}   // End of add_data function

// Updates the data for a valid, existing year in the time series
int timeSeries::update_data(int Y, double D){

    // Finds the index of the year in the time series
    int yearIndex(find_year(Y));

    // Terminate function if the year does not already exist
    if (yearIndex == -1){
        return 0;
    }

    // Terminate function if the data at this year is invalid
    if (data[yearIndex] == -1){
        return 0;
    }

    // Otherwise, update data for the desired year
    data[yearIndex] = D;

    return 1;   // Return true

}   // End of update_data function

// Computes the mean (average) of all valid data in the time series
double timeSeries::mean(){

    // Keep track of the sum and the number of valid elements
    double sum(0);
    int numValidElements(0);

    if (!is_valid()){   // If there is no valid data, terminate the function
        return 0;
    } else {            // Otherwise, cycle through the function
        for (int i(0); i < currentCapacity; ++i){
            if (data[i] == -1) continue;        // If the data is invalid, skip it
            sum += data[i];
            ++numValidElements;
        }
    }

    return sum/numValidElements;    // Return the computed mean

}   // End of mean function

// Determines if the time series is monotonic
bool timeSeries::is_monotonic(){

    //Function shouldn't run if it only has invalid data (as per main function) but I have this here just in case
    if (!is_valid()){
        return false;
    }
    
    // If there are 1 or 2 elements in the series, it MUST be monotonic so immediately return true
    if (currentCapacity <= 2){
        return true;
    }

    // Assume the series is montonic in 2 ways and then check if this assumption was wrong
    bool isIncreasing(true), isDecreasing(true);

    // Check if the series is increasing
    for (int i(0); i < currentCapacity-1; ++i){
        if (data[i] == -1) continue;    // If the data is invalid, skip the element
        
        for (int j(i+1); j < currentCapacity; ++j){                // Otherwise, check if the next valid element is less than the current element
            if (data[j] == -1) continue;
            if (data[i] > data[j]) isIncreasing = false;    // If so, the time series is not increasing and we can stop checking
            break;
        }

        if (!isIncreasing) break;
    }

    // Check if the series is decreasing
    for (int i(0); i < currentCapacity-1; ++i){
        if (data[i] == -1) continue;    // If the data is invalid, skip the element

        for (int j(i+1); j < currentCapacity; ++j){                // Otherwise, check if the next vaild element is greater than the current element
            if (data[j] == -1) continue;
            if (data[i] < data[j]) isDecreasing = false;    // If so, the time series is not decreasing and we can stop checking
            break;
        }

        if (!isDecreasing) break;
    }

    return (isIncreasing || isDecreasing);  // If either or both are true, returns true--time series is monotonic

}   // End of is_montonic function

// Determines the line of best fit for the time series
void timeSeries::best_fit(double &m, double &b){

    //Function shouldn't run if it only has invalid data (as per main function) but I have this here just in case
    // Set slope and y-intercept to 0 and terminate function
    if (!is_valid()){
        m = 0;
        b = 0;
        return;
    }

    // Keep track of the valid elements for the series
    double N(0);

    // Computes the valid data in the series
    for (int i(0); i < currentCapacity; ++i){
        if (data[i] == -1) continue;    // If data is invalid, skip it
        ++N;
    }

    // Computes the first sum in the slope equation
    double sum1(0);

    for (int i(0); i < currentCapacity; ++i){
        if (data[i] == -1) continue;    // If data is invalid, skip it
        sum1 += years[i]*data[i];
    }

    sum1 *= N;  // The first sum is multiplied by N in the equation

    // Computes the second sum in the slope equation
    double sum2(0);

    for (int i(0); i < currentCapacity; ++i){
        if (data[i] == -1) continue;    // If data is invalid, skip it
        sum2 += years[i];
    }

    // Computes the third sum in the slope equation
    double sum3(0);

    for (int i(0); i < currentCapacity; ++i){
        if (data[i] == -1) continue;    // If data is invalid, skip it
        sum3 += data[i];
    }

    // Computes the fourth sum in the slope equation
    double sum4(0);

    for (int i(0); i < currentCapacity; ++i){
        if (data[i] == -1) continue;    // If data is invalid, skip it
        sum4 += years[i]*years[i];
    }

    sum4 *= N;  // The fourth sum is also multiplied by N in the equation

    // Computes the fifth sum in the slope equation
    double sum5(0);

    for (int i(0); i < currentCapacity; ++i){
        if (data[i] == -1) continue;    // If data is invalid, skip it
        sum5 += years[i];
    }

    sum5 *= sum5;   // The fifth sum is squared in the equation

    m = (sum1 - sum2*sum3) / (sum4 - sum5); // Compute the slope

    // Computes the first sum in the y-intercept equation
    double sum6(0);

    for (int i(0); i < currentCapacity; ++i){
        if (data[i] == -1) continue;    // If data is invalid, skip it
        sum6 += data[i];
    }

    // Computes the second sum in the y-intercept equation
    double sum7(0);

    for (int i(0); i < currentCapacity; ++i){
        if (data[i] == -1) continue;    // If data is invalid, skip it
        sum7 += years[i];
    }

    sum7 *= m;  // The second sum is mulitplied by the slope in the equation

    b = (sum6 - sum7) / N;  // Compute the y-intercept

    // References to the slope and y-intercept are returned; modified values are applied in main function

}   // End of best_fit function
