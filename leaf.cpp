#include "leaf.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// Constructor
leaf::leaf(){

    currSize = 0;
    numCountries = 0;

    min = 0;
    max = 0;

    left = nullptr;
    right = nullptr;

}   // End of constructor

// Destructor
leaf::~leaf(){
    left = right = nullptr;
}

// Setter for left child
void leaf::set_left(leaf* leftLeaf){
    left = leftLeaf;
}

// Setter for right child
void leaf::set_right(leaf* rightLeaf){
    right = rightLeaf;
}

// Getter for left child
leaf* leaf::get_left(){
    return left;
}

// Getter for right child
leaf* leaf::get_right(){
    return right;
}

// Getter for the number of countries in the node
int leaf::get_numCountries(){
    return numCountries;
}

// Sets the range for the node
void leaf::set_min_max(countryLL* array[], int arrSize, std::string seriesCode){

    bool first = true;

    // Go through all the strings in this node
    for (int i(0); i < currSize; ++i){

        // Compare them to all the countries in the allCountries array
        for (int j(0); j < 512; ++j){

            // If invalid index, skip it
            if (array[j] == nullptr || array[j] -> get_wasDeleted()){
                continue;
            }

            // If the name matches, get the series mean
            if (array[j] -> get_countryName() == countryNames[i]){

                double newSeriesMean = array[j] -> get_mean(seriesCode);

                // For the first country, set min and max equal to the mean
                if (first){
                    min = max = newSeriesMean;
                    first = false;

                // Otherwise, update the variables if necessary
                } else {
                    
                    if (newSeriesMean < min){
                        min = newSeriesMean;
                    }

                    if (newSeriesMean >max){
                        max = newSeriesMean;
                    }

                }
                break;  // Found the correct country so we can break from the allCountries search loop now

            }

        }

    }

}   // End of the set_min_max function

// Loads the node based on the provided range
void leaf::load(double& minLeafMean, double& maxLeafMean, countryLL* array[], int arrSize, bool isLeft, std::string seriesCode){

    // Iterate through each country in the data array
    for (int i(0); i < 512; ++i){

        // If invalid index, skip it
        if (array[i] == nullptr || array[i] -> get_wasDeleted()){
            continue;
        }

        // Skip invalid means
        if (!(array[i] -> is_valid(seriesCode))){
            continue;
        }

        // Get the mean for each country
        double currSeriesMean = array[i] -> get_mean(seriesCode);

        // If this is a left node (meaning it's strictly LESS than half the range of the parent)...
        if (isLeft){

            // ...Only include countries less than the max mean
            if (currSeriesMean >= minLeafMean && currSeriesMean < maxLeafMean){
                countryNames[currSize] = array[i] -> get_countryName();
                ++currSize;
                ++numCountries;
            }

        // If this is a right node (meaning it can be equal to half the range of the parent)...
        } else {

            // ...include countries equal to the max mean
            if (currSeriesMean >= minLeafMean && currSeriesMean <= maxLeafMean){
                countryNames[currSize] = array[i] -> get_countryName();
                ++currSize;
                ++numCountries;
            }

        }

    }   // End of for-loop

    // Make sure to update the range for this node
    set_min_max(array, arrSize, seriesCode);

    // Make sure these new values are passed by reference to the tree
    // function so the next node knows the correct range of its parent
    minLeafMean = min;
    maxLeafMean = max;

}   // End of the load function

// Adds names of countries with invalid means (only occurs to the left-most leaf)
void leaf::add_invalid(countryLL* array[], int arrSize, std::string seriesCode){

    // Iterate through each country in the data array
    for (int i(0); i < 512; ++i){

        // If invalid index, skip it
        if (array[i] == nullptr || array[i] -> get_wasDeleted()){
            continue;
        }

        // If the mean for the time series of this country is invalid, add the name
        if (!(array[i] -> is_valid(seriesCode))){
            countryNames[currSize] = array[i] -> get_countryName();
            ++currSize;
            ++numCountries;
        }

    }

    // Update the range for this node
    set_min_max(array, arrSize, seriesCode);

}   // End of the add_invalid function

// Outputs the country names for the limits function
void leaf::print(countryLL* array[], int arrSize, std::string seriesCode, std::string operation){

    // Iterate through all the country names in this node
    for (int i(0); i < currSize; ++i){

        // Skip empty spots
        if (countryNames[i] == ""){
            continue;
        }

        // Iterate through each country in the data array
        for (int j(0); j < 512; ++j){

            // If invalid index, skip it
            if (array[j] == nullptr || array[j] -> get_wasDeleted()){
                continue;
            }

            // Once the correct linked list is found...
            if (array[j] -> get_countryName() == countryNames[i]){

                // ...If we are in the left-most leaf...
                if (operation == "lowest"){

                    // ...Print values that are equal to the minimum of the node
                    if (std::abs(array[j] -> get_mean(seriesCode) - min) <= 0.001){
                        std::cout << countryNames[i] << " ";
                    }
                
                // Otherwise...
                } else if (operation == "highest"){

                    // ...Output values that are equal to the maximum of the node AND valid
                    if (std::abs(array[j] -> get_mean(seriesCode) - max) <= 0.001 && array[j] -> is_valid(seriesCode)){
                        std::cout << countryNames[i] << " ";
                    }

                }

                break;  // Skip the rest of the linked lists in the data array

            }

        }   // End of the for-loop for the data array

    }   // End of the for-loop for the node's array

}   // End of the print function for the limits command

// Outputs the country names for the find function
void leaf::print(countryLL* array[], int arrSize, double mean, std::string seriesCode, std::string operation){

    // Iterate through all the country names in this node
    for (int i(0); i < currSize; ++i){

        // Skip empty spots
        if (countryNames[i] == ""){
            continue;
        }

        // Iterate through each country in the data array
        for (int j(0); j < 512; ++j){

            // If invalid index, skip it
            if (array[j] == nullptr || array[j] -> get_wasDeleted()){
                continue;
            }

            // Once the correct linked list is found...
            if (countryNames[i] == array[j] -> get_countryName()){
                
                // Calculate the mean of the time series
                double currMean = array[j] -> get_mean(seriesCode);

                // Ouput the country name if we want less than the mean (and it is less)
                if (operation == "less"){
                    if (currMean < mean){
                        std::cout << countryNames[i] << " ";
                    }
                
                // Output the country name if we want equal to the mean (and it is almost equal)
                } else if (operation == "equal"){
                    if (std::abs(currMean - mean) <= 0.001){
                        std::cout << countryNames[i] << " ";
                    }

                // Output the country name if we want greater than the mean (and it is greater)
                } else if (operation == "greater"){
                    if (currMean > mean){
                        std::cout << countryNames[i] << " ";
                    }
                }

                break;  // Skip the rest of the linked lists in the data array

            }

        }   // End of the for-loop for the data array

    }   // End of the for-loop for the node's array

}   // End of the print function for the find command

// Deletes the country from the node
bool leaf::delete_countryName(countryLL* array[], int arrSize, std::string seriesCode, std::string countryName){

    // Iterate through all the country names in this node
    for (int i(0); i < currSize; ++i){

        // If the desired country is found, delete it
        if (countryNames[i] == countryName){

            countryNames[i] = "";
            --numCountries;

            // Make sure to update the range
            set_min_max(array, arrSize, seriesCode);

            // The country was found in the node so return a success
            return true;

        }

    }

    // At this point the country was not found in the node, so return a failure
    return false;

}   // End of the print command for the find function

// Finds all countries that meet a certain operation on a mean (less, equal, greater) and returns them in a vector
void leaf::find_related(countryLL* array[], double mean, std::string seriesCode, std::string operation, std::vector< countryLL* >& countries){

    // Iterate through all the country names in this node
    for (int i(0); i < currSize; ++i){

        // Skip empty spots
        if (countryNames[i] == ""){
            continue;
        }

        // Iterate through each country in the data array
        for (int j(0); j < 512; ++j){

            // If invalid index, skip it
            if (array[j] == nullptr || array[j] -> get_wasDeleted()){
                continue;
            }

            // Once the correct linked list is found...
            if (countryNames[i] == array[j] -> get_countryName()){
                
                // Calculate the mean of the time series
                double currMean = array[j] -> get_mean(seriesCode);

                // Push back the country address if we want less than the mean (and it is less)
                if (operation == "less"){
                    if (currMean < mean){
                        countries.push_back(array[j]);
                    }
                
                // Output the country name if we want equal to the mean (and it is almost equal)
                } else if (operation == "equal"){
                    if (std::abs(currMean - mean) <= 0.001){
                        countries.push_back(array[j]);
                    }

                // Output the country name if we want greater than the mean (and it is greater)
                } else if (operation == "greater"){
                    if (currMean > mean){
                        countries.push_back(array[j]);
                    }
                }

                break;  // Skip the rest of the linked lists in the data array

            }

        }   // End of the for-loop for the data array

    }   // End of the for-loop for the node's array

}   // End of the find_related function
