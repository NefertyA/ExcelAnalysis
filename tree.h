#include "leaf.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <tuple>
#include <vector>

#ifndef TREE_H
#define TREE_H

class tree{

    private:

        // Array that holds all the country linked lists
        countryLL* allCountries[512];
        int currSize;

        // Pointer to the root node
        leaf* root;

        // Contains the series code the binary tree was made for
        std::string currSeriesCode;

        // Our massive map
        std::map<countryLL*, std::map<countryLL*, std::set<std::tuple<std::string, double, std::string>>>> map;

    public:

        // Constructor and destructor
        tree();
        ~tree();

        // Finds an index to place a country given its country code
        int get_index(std::string countryCode);

        // Finds where the index should be for a country given its country code
        int find_index(std::string countryCode, int& numSteps);

        // Loads all countries into array
        void load(std::string fileName);

        // Deletes the binary tree
        void erase(leaf* node);

        // Initializes the map
        void initialize();
        
        // Updates the edges of the map
        bool update_edges(std::string seriesCode, double threshold, std::string relation);
        void find_leaf(leaf* node, double mean, std::string operation, std::vector< countryLL* >& countries);
        
        // Outputs adjacent countries to a node in the map
        bool adjacent(std::string countryCode);

        // Determines if a path exists between two countries
        bool path(std::string countryCode1, std::string countryCode2);

        // Outputs all relationships between two countries if any exist
        bool relationships(std::string countryCode1, std::string countryCode2);

        // Builds the binary tree
        void build(std::string seriesCode);
        leaf* help_build(double min, double max, bool isLeftNode, std::string seriesCode);
        void add_invalid(std::string seriesCode);

        // Getter and setter for range
        void set_range(std::string seriesCode, double& newMin, double& newMax);
        void get_range(std::string seriesCode);

        // Find or modify values in the hash table
        bool lookup(std::string countryCode);
        bool remove(std::string countryCode);
        bool insert(std::string countryCode, std::string fileName);

        // Lists all time series for a country
        void list(std::string countryName);

        // Finds a specific leaf in the tree based on a mean and operation
        bool find(double mean, std::string operation);
        void find_leaf(leaf* node, double mean, std::string operation);

        // Deletes a country from the binary tree
        bool del(std::string countryName);
        void delete_country(leaf* node, std::string countryName, bool& inTree);

        // Finds the left-most or right-most leaf
        bool limits(std::string condition);
        leaf* find_left_leaf(leaf*);
        leaf* find_right_leaf(leaf*);

};

#endif