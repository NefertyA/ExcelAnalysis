#include "tree.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <map>
#include <set>
#include <tuple>
#include <vector>
#include <queue>

// Initializes some of the private values
tree::tree(){

    currSize = 0;

    root = nullptr;

    for (int i(0); i < 512; ++i){
        allCountries[i] = nullptr;
    }

}

// Erases the entire tree when the program exits
tree::~tree(){

    erase(root);

    // Delete country linked lists in the hash table
    for (int i(0); i < 512; ++i){
        delete allCountries[i];
    }

}

// Inserts each country from the hash table into the main map
void tree::initialize(){

    // Parse through the entire hash table
    for (int i(0); i < 512; ++i){

        // Skip any empty or invalid indices
        if (allCountries[i] == nullptr || allCountries[i] -> get_wasDeleted()){ 
            continue;
        }

        // Insert each countryLL address as a new key of the main map
        map[allCountries[i]];
        
    }

}   // End of the initialize function

// Finds every leaf in the binary tree and calls on the find_related function in the leaf class (only for maps)
void tree::find_leaf(leaf* node, double mean, std::string operation, std::vector< countryLL* >& countries){

    // Base case (empty subtree)
    if (node == nullptr){
        return;
    }

    // Go all the way left and then right
    find_leaf(node -> get_left(), mean, operation, countries);
    find_leaf(node -> get_right(), mean, operation, countries);

    // Once a leaf is found, push back its countries into the vector based on the desired mean and operation (in leaf function)
    if (node -> get_left() == nullptr && node -> get_right() == nullptr){
        node -> find_related(allCountries, mean, currSeriesCode, operation, countries);
    }

}   // End of the find_leaf function for maps

// Updates the edges of the map based on a time series, a mean value, and an operation with it (less, equal, greater)
bool tree::update_edges(std::string seriesCode, double threshold, std::string relation){

    // Builds the binary tree based on the series code
    // This is assigned to root
    build(seriesCode);

    // Place related values into a vector
    std::vector<countryLL*> relatedCountries;
    find_leaf(root, threshold, relation, relatedCountries);

    // If the list is empty just return a failure message
    if (relatedCountries.empty()){
        return false;
    }

    // Vector now has all relevant country addresses

    // Create tuple that will go into everything
    std::tuple<std::string, double, std::string> t(seriesCode, threshold, relation);

    // Will keep track if we inserted a new relationship or not
    bool isNewRelationship = false;

    /*
        CITATION:
        The next 2 lines were created after looking at the std::vector reference page: https://en.cppreference.com/w/cpp/container/vector/push_back.
        The example provided on this page used "auto&&" to iterate through the vector. I decided to use this to make it easier to iterate through 
        the vectors in my program. This is the reason why I wrote "auto&& [index] : [vector_name]" in the next 2 lines.
    */

    // Nested loops to search through vector
    for (auto&& i : relatedCountries){

        for (auto&& j : relatedCountries){

            // Don't compare the country with itself
            if (i == j){
                continue;
            }

            // If the tuple doesn't exist for this location in the map, insert it
            if (map[i][j].find(t) == map[i][j].end()){
                map[i][j].insert(t);
                isNewRelationship = true;
            }

            // Otherwise, continue comparing other countries

        }
    }

    // Return true or false if a new relationship was added or not
    return isNewRelationship;

}   // End of the update_edges function

// Outputs all countries a specific country shares an edge with
bool tree::adjacent(std::string countryCode){

    // Find index of country code
    int misc = 0;
    int index = find_index(countryCode, misc);

    // If it doesn't exist in the graph, return failure
    if (index == -1){
        std::cout << "failure";
        return false;
    }

    // Save the key
    countryLL* countryKey = allCountries[index];

    // If the index has no adjacent countries, return none
    if (map[countryKey].empty()){
        std::cout << "none";
        return false;
    }

    /*
        CITATION:
        The next line was created after looking at the std::map reference page: https://en.cppreference.com/w/cpp/container/map.
        The example provided on this page used "auto& [key, value] : m" to iterate through the map. I decided to use this to make 
        it easier to iterate through the map in my program. This is the reason why I wrote "auto& [key, value] : [map_name]" in 
        the next line.
    */

    // Otherwise, output each country name
    for (auto& [key, value] : map[countryKey]){

        std::cout << key -> get_countryName() << " ";
    }

    return true;

}   // End of the adjacent function

// Determines if a path exists between 2 countries in the map
bool tree::path(std::string countryCode1, std::string countryCode2){

    // Find index of each country
    int misc = 0;
    int index1 = find_index(countryCode1, misc);
    int index2 = find_index(countryCode2, misc);

    // Find from and to keys in map
    countryLL* from = allCountries[index1];
    countryLL* to = allCountries[index2];

    // Create queue and set for path-finding
    std::queue<countryLL*> todo;
    std::set<countryLL*> visited;

    // Push the from key onto the stack
    todo.push(from);

    // Continue searching the map until the queue is empty (everything has been searched)
    while(!todo.empty()){
        
        // Pop the first element off the stack (FIFO)
        countryLL* currentNode = todo.front();
        todo.pop();

        // If it's the element we want to get to, return true
        if (currentNode == to){
            return true;
        }

        // Otherwise, if we haven't visited this node before
        if (visited.find(currentNode) == visited.end()){

            // Add it to the visited set
            visited.insert(currentNode);

            /*
                CITATION:
                The next line was created after looking at the std::map reference page: https://en.cppreference.com/w/cpp/container/map.
                The example provided on this page used "auto& [key, value] : m" to iterate through the map. I decided to use this to make 
                it easier to iterate through the map in my program. This is the reason why I wrote "auto& [key, value] : [map_name]" in 
                the next line.
            */

            // And add all of its country-edge pairs to the queue
            for (auto& [key, value] : map[currentNode]){
                todo.push(key);
            }

        }

    }

    // At this point, we never found the element, so return false
    return false;

}   // End of the path function

// Outputs all the relationships between 2 countries (every in their edge, which is a set)
bool tree::relationships(std::string countryCode1, std::string countryCode2){

    // Find indices for these countries
    int misc = 0;
    int index1 = find_index(countryCode1, misc);
    int index2 = find_index(countryCode2, misc);

    // Convert indices into keys for the map
    countryLL* key1 = allCountries[index1];
    countryLL* key2 = allCountries[index2];

    // If the countries do not share an edge, return false
    if (map[key1].find(key2) == map[key1].end()){
        return false;
    }

    /*
        CITATION:
        The next line was created after looking at the std::map reference page: https://en.cppreference.com/w/cpp/utility/tuple.
        The example provided on this page used "auto" to define a tuple. I decided to use this to define the index in my for-loop 
        to make it easier to iterate through the tuples in my program. This is the reason why I wrote "auto [index] : [tuple_name]" 
        in the next line.
    */

    // Otherwise, output all their relationships (all the tuples in their set)
    for (auto i : map[key1][key2]){
        std::cout << "(" << std::get<0>(i) << " " << std::get<1>(i) << " " << std::get<2>(i) << ") ";
    }

    // The countries shared an edge so return true
    return true;

}   // End of the relationships function

// Finds a valid index to put a country in the hash table
int tree::get_index(std::string countryCode){
    
    // Initially, the number of extra steps is 0
    int numSteps = 0;

    // Calculate W
    int W = (countryCode[0] - 65)*(26*26) + (countryCode[1] - 65)*26 + (countryCode[2] - 65);
    
    // Calculate the primary hash function index
    int W1 = W % 512;

    // Calculate the secondary hash function index
    int W2 = int( std::floor( (W / 512) ) ) % 512;

    // If the above index is even, add 1 to it
    if ((W2 % 2) == 0){
        ++W2;
    }

    // Loops that keeps searching for a valid index of the country
    while (true){

        // Compute overall hash index
        int index = W1 + numSteps*W2;
        index %= 512;

        // Increase the number of steps
        ++numSteps;

        // If the index is empty, we can put the country at this index
        if (allCountries[index] == nullptr){

            // Make sure to create a country here first
            allCountries[index] = new countryLL;

            return index;
        }

        // If the index was previously occupied, we can also put the country here
        if (allCountries[index] -> get_wasDeleted()){
            return index;
        }

        // If the country is already in the array, return an error index
        if (allCountries[index] -> get_countryCode() == countryCode){
            return -1;
        }

        // At this point, the index had a valid country, so we continue searching

    }   // End of the the while loop

}   // End of the get_index function

// Finds the index of a given country in the hash table and returns the number of steps to get there
int tree::find_index(std::string countryCode, int& numSteps){

    // Initially, the number of extra steps is 0
    numSteps = 0;

    // Calculate W
    int W = (countryCode[0] - 65)*(26*26) + (countryCode[1] - 65)*26 + (countryCode[2] - 65);
    
    // Calculate the primary hash function index
    int W1 = W % 512;

    // Calculate the secondary hash function index
    int W2 = int( std::floor( (W / 512) ) ) % 512;

    // If even, add 1 to index
    if ((W2 % 2) == 0){
        ++W2;
    }

    // Loop that keeps searching for the index of the country
    while (true) {

        // Compute overall hash index
        int index = W1 + numSteps*W2;
        index %= 512;

        // Increase the number of steps
        ++numSteps;

        // If we find an empty index, the country was never in the array so return an error
        if (allCountries[index] == nullptr){
            return -1;
        }

        // If the index has an invalid country or the wrong one, continue searching
        if (allCountries[index] -> get_wasDeleted() || allCountries[index] -> get_countryCode() != countryCode){
            continue;
        }

        // If we find the correct index, return the index
        if (allCountries[index] -> get_countryCode() == countryCode){
            return index;
        }

    }   // End of the while loop

}   // End of the find_index function

// Loads the data array of linked lists
void tree::load(std::string fileName){

    // Opens the desired file
    std::fstream fopen;
    fopen.open(fileName);

    std::string line;
    std::string prevCountry;

    int W = 0;              // Hash table index
    bool isValid = true;    // Determines if index is true

    // Reads each line of the file
    while (getline(fopen, line)){

        // Parse the line
        std::stringstream parse(line);

        // Read the country name
        std::string nameElement;
        getline(parse, nameElement, ',');

        // Read the country code
        std::string currCountry;
        getline(parse, currCountry, ',');

        // If we are reading a new country, find the new index
        if (currCountry != prevCountry){

            // Find index for this country code
            int tempW = get_index(currCountry);

            // If we get a valid index, set W to new index
            if (tempW != -1){
                W = tempW;
                isValid = true;

            } else {    // If it's invalid, set isValid to false
                isValid = false;
            }
            
        }

        // If index is valid, load/append this index with the file line
        if (isValid){
            allCountries[W] -> load(line);
        }

        // If we got an invalid index, then the country already exists in the array, so we will skip it

        // Make sure to update prevCountry
        prevCountry = currCountry;

    }

    // Close the file
    fopen.close();

    /* int a = 1;

    for (int i(0); i < 512; ++i){
        
        if (allCountries[i] == nullptr || allCountries[i] -> get_wasDeleted()){
            continue;
        }

        std::cout << a << ": " << allCountries[i] -> get_countryName() << ", ";
        ++a;

    }

    std::cout << std::endl; */

}   // End of the load function

// For the LOOKUP command (finds the hash table index of a country)
bool tree::lookup(std::string countryCode){

    int numSteps = 0;

    int index = find_index(countryCode, numSteps);

    // If index is invalid, it doesn't exist in the array so return failure
    if (index == -1){
        return false;
    }

    // At this point, the index was valid, so we can output the result
    std::cout << "index " << index << " searches " << numSteps;

    // Return success
    return true;

}   // End of the lookup function

// For the REMOVE command (removes a country from the hash table)
bool tree::remove(std::string countryCode){

    int numSteps = 0;

    int index = find_index(countryCode, numSteps);

    // If index is invalid, country is not in the array so return failure
    if (index == -1){
        return false;
    }

    // At this point country was in the array so delete it

    // Delete country from the tree (if possible)
    del(allCountries[index] -> get_countryName());

    // Delete the country
    delete allCountries[index];

    // Add un-loaded country to the index (will have wasDeleted = 1)
    allCountries[index] = new countryLL;

    // Return success
    return true;

}   // End of the remove function

// Function for the INSERT command (inserts a new country into the array)
bool tree::insert(std::string countryCode, std::string fileName){

    int numSteps = 0;

    int index = find_index(countryCode, numSteps);

    // If we get a valid index, then the country already exists so return failure
    if (index != -1){
        return false;
    }

    // At this point, country doesn't exist, so we can continue

    // At this point, we did not find the country in the array, so we need to find the correct index
    index = get_index(countryCode);

    // If the index is valid
    if (index != -1){

        // Load the country at this index
        allCountries[index] -> load(countryCode, fileName);

        // Return a success
        return true;
    }

    // If the index was invalid, the country already existed, so return a failure
    return false;

}   // End of the insert function

// Recursively deletes each node in the binary tree
void tree::erase(leaf* remove){

    // Base case (if the subtree is empty)
    if (remove == nullptr){
        return;
    }

    // Remove both the right and left child of each node
    erase(remove -> get_left());
    erase(remove -> get_right());

    // Once at a leaf node, delete it
    delete remove;
    remove = nullptr;

}   // End of the erase function

// Builds the entire binary tree based on a time series
void tree::build(std::string seriesCode){

    // Save the series code
    currSeriesCode = seriesCode;

    // Erase everything first (in case a tree already exists)
    erase(root);

    // Initialize range for this binary tree
    double minMean = 0.0;
    double maxMean = 0.0;

    // Compute root node range
    set_range(seriesCode, minMean, maxMean);

    // Call on help_build using range (this will create the tree and assign it to the root pointer)
    root = help_build(minMean, maxMean, 0, seriesCode);

    // Put all the invalid countries in the last node
    add_invalid(seriesCode);

}   // End of the build function

// Sets the range for the binary tree based on a time series
void tree::set_range(std::string seriesCode, double& newMin, double& newMax){

    // Initialize min and max mean
    double currMean;
    double max = -1.0;
    double min = -1.0;

    // Iterate through the entire data array
    for (int i(0); i < 512; ++i){
    
        // If invalid index, skip it
        if (allCountries[i] == nullptr || allCountries[i] -> get_wasDeleted()){
            continue;
        }

        // If data is invalid, skip it. We don't want it in any nodes until the very last leaf one
        if (!allCountries[i] -> is_valid(seriesCode)){
            continue;
        }

        // Calculate the time series mean for this country
        currMean = allCountries[i] -> get_mean(seriesCode);

        // If this is the first valid mean, let max and min equal it
        if (min < 0 && max < 0){
            min = max = currMean;
        }

        // Otherwise, update min and max accordingly
        if (currMean < min){
            min = currMean;
        }

        if (currMean > max){
            max = currMean;
        }

    }

    // Update final values of the range
    newMin = min;
    newMax = max;

}   // End of the set_range function

// Computes and outputs range given a specific time series
void tree::get_range(std::string seriesCode){

    // Initialize min and max of the range
    double rangeMin = 0.0;
    double rangeMax = 0.0;

    // Computes the range
    set_range(seriesCode, rangeMin, rangeMax);

    // Outputs the range
    std::cout << rangeMin << " " << rangeMax << std::endl;

}   // End of the get_range function

// Recursive function that builds the binary tree
leaf* tree::help_build(double min, double max, bool isLeftNode, std::string seriesCode){

    // Make a new tree node (new subtree)
    leaf* tempLeaf = new leaf;

    // Load the node using a leaf function. Compute the range of this node (or subtree)
    tempLeaf -> load(min, max, allCountries, currSize, isLeftNode, seriesCode);

    // If the node has a range of almost 0, it will be a leaf node
    if (max - min <= 0.001){

        tempLeaf -> set_left(nullptr);
        tempLeaf -> set_right(nullptr);

    } else {

        // Initializes the left pointer with the minimum to half the maximum
        tempLeaf -> set_left(help_build(min, (max + min)/2, 1, seriesCode));

        // Initializes the right pointer with half the maximum to the minimum
        tempLeaf -> set_right(help_build((max + min)/2, max, 0, seriesCode));

    }

    // For the first call (and last return) of this function
    // Points root to the root node (that has all the countries in it)
    return tempLeaf;

}   // End of the help_build function

// Adds the invalid countries to the left-most node
void tree::add_invalid(std::string seriesCode){

    // Finds the left-most leaf
    leaf* leftMostLeaf = find_left_leaf(root);

    // Adds invalid means to the left-most leaf
    leftMostLeaf -> add_invalid(allCountries, currSize, seriesCode);

}   // End of the add_invalid function

// Lists all the time series names for a country
void tree::list(std::string countryName){

    // Iterate through entire data array
    for (int i(0); i < 512; ++i){

        // If invalid index, skip it
        if (allCountries[i] == nullptr || allCountries[i] -> get_wasDeleted()){
            continue;
        }

        // Once the correct country is found, output all the time series names
        if (allCountries[i] -> get_countryName() == countryName){
            allCountries[i] -> list();
            //std::cout << i << std::endl;
            break;
        }

    }

}   // End of the list function

// Recursive function that finds every leaf in the tree
void tree::find_leaf(leaf* node, double mean, std::string operation){

    // Base case (empty subtree)
    if (node == nullptr){
        return;
    }

    // Go all the way left and then right
    find_leaf(node -> get_left(), mean, operation);
    find_leaf(node -> get_right(), mean, operation);

    // Once a leaf is found, print its countries based on the desired mean and operation
    if (node -> get_left() == nullptr && node -> get_right() == nullptr){
        node -> print(allCountries, currSize, mean, currSeriesCode, operation);
    }

}   // End of the find_leaf function

// Non-recursive function for the find command
bool tree::find(double mean, std::string operation){

    // If the tree doesn't exist, return a failure
    if (root == nullptr){
        return false;
    }

    // Otherwise, find and output the countries based on the input
    find_leaf(root, mean, operation);

    // Return a success
    return true;

}   // End of the find function

// Recursive function that deletes a specific country from the entire binary tree
void tree::delete_country(leaf* node, std::string countryName, bool& inTree){

    // Base case (empty subtree)
    if (node == nullptr){
        return;
    }

    // Delete the country from each node. If the country was in the node, keep note of it
    if (node -> delete_countryName(allCountries, currSize, currSeriesCode, countryName)){
        inTree = true;
    }

    // Do this for each left and right subtree
    delete_country(node -> get_left(), countryName, inTree);
    delete_country(node -> get_right(), countryName, inTree);

    // If this node has a right child
    if (node -> get_right() != nullptr){
        // And the right child has no more countries in it
        if (node -> get_right() -> get_numCountries() == 0){
            // Delete the entire node
            leaf* tempLeaf = node -> get_right();
            node -> set_right(node -> get_right() -> get_right());
            delete tempLeaf;
        }
    }

    // If this node has a left child
    if (node -> get_left() != nullptr){
        // And the left child has no more countries in it
        if (node -> get_left() -> get_numCountries() == 0){
            // Delete the entire node
            leaf* tempLeaf = node -> get_left();
            node -> set_left(node -> get_left() -> get_left());
            delete tempLeaf;
        }
    }

}   // End of the delete_country function

// Non-recursive function for the delete command
bool tree::del(std::string countryName){

    // Base case (empty subtree)
    if (root == nullptr){
        return false;
    }

    // Assume the desired country to delete is not in the tree (might change after calling delete_country)
    bool isInTree(false);
    delete_country(root, countryName, isInTree);

    // If user deleted the last country in the tree, make sure to delete the root node too
    if (root -> get_numCountries() == 0){
        delete root;
        root = nullptr;
    }

    // Return either success or failure message
    return isInTree;

}   // End of the del function

// Recursive function that finds the left-most leaf
leaf* tree::find_left_leaf(leaf* node){

    // Until the left-most leaf node is found
    while (node -> get_left() != nullptr || node -> get_right() != nullptr){
        // Go to the right subtree if the left subtree is empty. Otherwise, go to the left
        if (node -> get_left() == nullptr){
            node = node -> get_right();
        } else {
            node = node -> get_left();
        }
    }

    // Return the current node, which should be the left-most leaf
    return node;

}   // End of the find_left_leaf function

// Recursive function that finds the right-most leaf
leaf* tree::find_right_leaf(leaf* node){

    // Until the right-most leaf is found
    while (node -> get_left() != nullptr || node -> get_right() != nullptr){
        // Go to the left subtree if the right subtree is empty. Otherwise, go to the right
        if (node -> get_right() == nullptr){
            node = node -> get_left();
        } else {
            node = node -> get_right();
        }
    }

    // Return the current node, which should be the right-most leaf
    return node;
}   // End of the find_right_leaf function

// Finds the lowest or highest limit based on the command
bool tree::limits(std::string condition){

    // Base case (empty subtree). Return a failure
    if (root == nullptr){
        return false;
    }

    leaf* tempLeaf = nullptr;

    // Find the left-most or right-most leaf
    if (condition == "lowest"){
        tempLeaf = find_left_leaf(root);
    } else if (condition == "highest"){
        tempLeaf = find_right_leaf(root);
    }

    // Print all the countries in this leaf
    tempLeaf -> print(allCountries, currSize, currSeriesCode, condition);

    // Return a success
    return true;

}   // End of the limits function
