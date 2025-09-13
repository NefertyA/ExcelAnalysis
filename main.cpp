#include "timeSeries.h"
#include "countryLL.h"
#include "tree.h"

#include <iostream>

// Function definitions
int main();
void program();

// Main function runs the program
int main()
{
    program();

    return 0;

}   // End of main

// Function that runs the program
void program(){

    std::string command;
    tree dataTree;

    do {

        // Take in the first command
        std::cin >> command;

        // For the load command
        if (command == "LOAD"){

            // Take in the filename
            std::string fileName;
            std::cin >> fileName;

            // Load the tree
            dataTree.load(fileName);

            // Always output success message
            std::cout << "success" << std::endl;

        // For the initialize command
        } else if (command == "INITIALIZE"){

            // Create empty map
            dataTree.initialize();

            // Always output success message
            std::cout << "success" << std::endl;

        // For the update_edges command
        } else if (command == "UPDATE_EDGES"){

            std::string seriesCode;
            double threshold;
            std::string relation;

            // Take in the series code, threshold, and desired relationship
            std::cin >> seriesCode;
            std::cin >> threshold;
            std::cin >> relation;

            // Update the edges for the map
            if (dataTree.update_edges(seriesCode, threshold, relation)){
                std::cout << "success" << std::endl;
            } else {
                std::cout << "failure" << std::endl;
            }

        // For the adjacent command
        } else if (command == "ADJACENT"){

            // Take in the time series code
            std::string countryCode;
            std::cin >> countryCode;

            // Finds adjacent countries to a country in the map
            dataTree.adjacent(countryCode);

            // For formatting
            std::cout << std::endl;

        // For the path command
        } else if (command == "PATH"){

            std::string countryCode1;
            std::string countryCode2;

            // Take in the 2 country codes
            std::cin >> countryCode1;
            std::cin >> countryCode2;

            // If a path exists, output true, and output false otherwise
            if (dataTree.path(countryCode1, countryCode2)){
                std::cout << "true" << std::endl;
            } else {
                std::cout << "false" << std::endl;
            }

        // For the relationships command
        } else if (command == "RELATIONSHIPS"){

            std::string countryCode1;
            std::string countryCode2;

            // Take in the country codes
            std::cin >> countryCode1;
            std::cin >> countryCode2;

            // Output all relationships between the countries
            // If they do not share an edge, output none
            if (!dataTree.relationships(countryCode1, countryCode2)){
                std::cout << "none";
            }

            // For formatting
            std::cout << std::endl;

        // For the exit command
        } else if (command == "EXIT"){

            // User wants to end program, so break out of the loop
            break;

        }

    } while (true);     // Loop continues until user calls exit command

}   // End of the program function
