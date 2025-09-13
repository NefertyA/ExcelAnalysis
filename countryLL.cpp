#include "countryLL.h"

#include <iostream>
#include <fstream>
#include <sstream>

// Constructor
countryLL::countryLL(){
    
    head = nullptr;
    tail = nullptr;

    seriesMean = 0;

    wasDeleted = 1;

}   // End of constructor

// Destructor
countryLL::~countryLL(){

    // Since erase will not be explicitly called, need to call it in destructor
    erase();

    // No need to delete any pointers; the erase function does that
    // Just set pointers to null to be safe

    head = nullptr;
    tail = nullptr;

    seriesMean = 0;

}   // End of destructor

double countryLL::get_mean(std::string seriesCode){

    timeSeries* tempSeries = find_time_series(seriesCode);

    if (tempSeries == nullptr){
        return 0;
    }

    return tempSeries -> mean();

}

bool countryLL::get_wasDeleted(){

    return wasDeleted;
    
}

bool countryLL::is_valid(std::string seriesCode){
    
    timeSeries* tempSeries = find_time_series(seriesCode);

    if (tempSeries == nullptr){
        return false;
    }

    return tempSeries -> is_valid();

}

std::string countryLL::get_countryName(){

    return countryName;

}

std::string countryLL::get_countryCode(){

    return countryCode;
}

// Erases the entire series
void countryLL::erase(){

    timeSeries* tempSeries = nullptr;

    // Point head towards the next time series (skipping the current one)
    // Then, delete the current time series
    while (head != nullptr) {

        tempSeries = head;
        
        head = head -> get_next();

        delete tempSeries;

    }

    // At this point, everything should have been deleted

}   // End of erase function

void countryLL::load(std::string line){

    std::stringstream parse(line);

    std::getline(parse, countryName, ',');
    std::getline(parse, countryCode, ',');

    // Create a new timeSeries node
    timeSeries* tempSeries = new timeSeries;

    // If it's the first time series, assign it to head
    if (head == nullptr){
        head = tempSeries;
        tail = tempSeries;

    } else {    // If not, assign it at the end
        tail -> set_next(tempSeries);
        tail = tempSeries;
    }

    // Initialize the timeSeries
    tempSeries -> initialize(line);

    // Set wasDeleted to false since this is a valid linked list now
    wasDeleted = 0;
}

// Loads all time series for a country from the file
void countryLL::load(std::string country, std::string fileName){

    // Initialize name of the country
    countryCode = country;

    // Open file
    std::ifstream fin;
    fin.open(fileName);

    std::string line;

    bool correctCountry(false);

    // Read each line from the file
    while(std::getline(fin, line)){

        // Parse the first element, the name of the country
        std::stringstream parse(line);
        std::string nameElement;
        std::getline(parse, nameElement, ',');

        // Parse in the second element, the country code
        std::string codeElement;
        std::getline(parse, codeElement, ',');

        // If we have not reached the country, skip
        // If we have finished searching the country, break out of the loop
        if (codeElement != countryCode){
            if (correctCountry) break;
            continue;
        }

        // We are now in the correct country
        correctCountry = true;

        // Set country name to the correct country
        countryName = nameElement;

        // Create a new timeSeries node
        timeSeries* tempSeries = new timeSeries;

        // If it's the first time series, assign it to head
        if (head == nullptr){
            head = tempSeries;
            tail = tempSeries;

        } else {    // If not, assign it at the end
            tail -> set_next(tempSeries);
            tail = tempSeries;
        }

        // Initialize the timeSeries
        tempSeries -> initialize(line);

    }

    // Set wasDeleted to false since this is a valid linked list now
    wasDeleted = 0;

}   // End of load function

// List all time series for the country
void countryLL::list(){

    // Print the country name and code
    std::cout << countryName << " " << countryCode << " ";

    // If nothing in the time series, exit
    if (head == nullptr) return;

    // Otherwise, create temp variable to parse through time series
    timeSeries* tempSeries = head;

    // Print the name of each time series until the last one
    while (tempSeries != nullptr){

        std::cout << tempSeries -> get_series_name() << " ";
        tempSeries = tempSeries -> get_next();

    }

    // Include an endline
    std::cout << std::endl;

}   // End of list function

// Finds the time series for a specific code
timeSeries* countryLL::find_time_series(std::string seriesCode){
    
    // If no data, return null
    if (head == nullptr) return nullptr;

    // Otherwise, run through all time series
    timeSeries* tempSeries = head;

    while (tempSeries -> get_next() != nullptr){

        // If not the time series, skip
        if (tempSeries -> get_series_code() != seriesCode){
            tempSeries = tempSeries -> get_next();
            continue;
        }

        // Otherwise return correct time series
        return tempSeries;

    }

    // Check the last time series as well and return if correct one
    if (tempSeries -> get_series_code() == seriesCode){
        return tempSeries;
    }

    // At this point, could not find time series, so return null
    return nullptr;

}   // End of find_time_series function

// Adds a new entry to a time series
bool countryLL::add(std::string seriesCode, int Y, double D){

    // Find the correct time series
    timeSeries* tempSeries = find_time_series(seriesCode);

    // Exit if the series could not be found
    if (tempSeries == nullptr) return 0;

    // Otherwise, attempt to add element for time series
    return tempSeries -> add_data(Y, D);

}   // End of add function

// Updates a pre-existing entry in a time series
bool countryLL::update(std::string seriesCode, int Y, double D){

    // Find the correct time series
    timeSeries* tempSeries = find_time_series(seriesCode);

    // Exit if the series could not be found
    if (tempSeries == nullptr) return 0;

    // Otherwise, attempt to update element for time series
    return tempSeries -> update_data(Y, D);

}   // End of update function

// Prints all elements of a specific time series
bool countryLL::print(std::string seriesCode){
    
    // Find the correct time series
    timeSeries* tempSeries = find_time_series(seriesCode);

    // Exit if the series could not be found
    if (tempSeries == nullptr) return 0;

    // Otherwise, attempt to print all elements for time series
    return tempSeries -> print();

}   // End of print function

// Deletes a specific time series
bool countryLL::del(std::string seriesCode){

    // Find the correct time series
    timeSeries* tempSeries = find_time_series(seriesCode);

    // Exit if series could not be found
    if (tempSeries == nullptr) return false;

    timeSeries* prevTempSeries = head;

    // If first in time series
    if (head -> get_series_code() == seriesCode){

        // If there's only one element, delete the head
        if (head -> get_next() == nullptr){
            delete head;
            head = nullptr;
        // Otherwise, let head skip current time series
        // Then, delete current time series
        } else {
            head = head -> get_next();
            delete prevTempSeries;
        }

        return true;    // Exit function

    }

    // If not first in time series, find the previous element
    while(prevTempSeries -> get_next() -> get_series_code() != seriesCode){
        prevTempSeries = prevTempSeries -> get_next();
    }

    // Now, prevTempSeries points to the time series before the one to be deleted

    // Edge case: if the time series to be deleted is the last one, make sure to update tail
    if (tempSeries == tail){
        tail = prevTempSeries;  // Tail now points to previous time series
    }

    // Previous time series points at series after the one being deleted
    prevTempSeries -> set_next(tempSeries -> get_next());

    delete tempSeries;  // Safe to delete time series

    return true;        // Exit function

}   // End of del function

// Finds the time series with the biggest mean
std::string countryLL::biggest(){

    timeSeries* tempSeries = head;          // Parses through time series
    timeSeries* maxMeanSeries = nullptr;    // Points to time series with biggest mean

    double tempMean(0);                     // Holds the mean of each time series
    double maxMean(0);                      // Holds the biggest mean

    // Go through all time series
    while (tempSeries != nullptr){
        
        // Calculate the mean of each one
        tempMean = tempSeries -> mean();

        // If a new max mean has been found, update accordingly
        if (tempMean > maxMean){
            maxMean = tempMean;
            maxMeanSeries = tempSeries;
        }

        // Go to the next time series
        tempSeries = tempSeries -> get_next();

    }

    // If maxMeanSeries is still null, then there were no valid series
    if (maxMeanSeries == nullptr) return "failure";

    // Otherwise, return the code of the series with the biggest mean
    return maxMeanSeries -> get_series_code();

}   // End of biggest function
