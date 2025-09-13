#include "countryLL.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#ifndef LEAF_H
#define LEAF_H

class leaf{

    private:

        // Contains all the names of the countries for the node
        std::string countryNames[512];
        int currSize;
        int numCountries;

        // Left and right children
        leaf* left;
        leaf* right;

        // Range of the node
        double min, max;

    public:

        // Constructor and destructor
        leaf();
        ~leaf();

        // Getter for the number of countries in the node
        int get_numCountries();

        // Getters and setters for left and right pointers
        void set_left(leaf* node);
        void set_right(leaf* node);
        leaf* get_left();
        leaf* get_right();

        // Setter for the range of the node
        void set_min_max(countryLL* array[], int arrSize, std::string seriesCode);

        // Loads the node with country names based on the range
        void load(double& minLeafMean, double& maxLeafMean, countryLL* array[], int arrSize, bool isLeft, std::string seriesCode);
        void add_invalid(countryLL* array[], int arrSize, std::string seriesCode);

        // Print functions for the find and limits command
        void print(countryLL* array[], int arrSize, std::string seriesCode, std::string operation);
        void print(countryLL* array[], int arrSize, double mean, std::string seriesCode, std::string operation);

        // Deletes the country name from the node
        bool delete_countryName(countryLL* array[], int arrSize, std::string seriesCode, std::string countryName);

        // Finds related countries for the map
        void find_related(countryLL* array[], double mean, std::string seriesCode, std::string operation, std::vector< countryLL* >& countries);

};  // End of the leaf class

#endif
