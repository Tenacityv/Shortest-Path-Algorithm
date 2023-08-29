/*
Author: Hassan Zakery
Date: 03/23/2023
*/ 
#include <stdlib.h>
#include <stdbool.h>
#include <fstream>
#include <iostream>
#include <queue>
#include <iomanip>
#include <string.h>
using namespace std;

void PrintEnvironment(int** array, int width, int height){
        for(int i = 0; i < height; i++){
                for(int j = 0; j < width; j++){
                        printf("%4d", array[i][j]);
                }
                cout << endl;
        }
        // Print 2d integer array
}   

void Print_Parallel_Environment(char** array, int width, int height){
        for(int i = 0; i < height; i++){
                for(int j = 0; j < width; j++){
                        printf("%c", array[i][j]);
                }
                cout << endl;
        } // Basic nested for loop to print any 2d array, this is for the character array
} 

// ** make sure we are in bounds and we are not in a impassible index for both functions below **
bool validFloodindex(int i, int j, int height, int width, int ** array){
         // Check if this a valid index inside the 2d integer array and not a '-1'
	 return (i >= 0 && i < height && j >= 0 && j < width && array[i][j] != -1);
}
bool validParallelIndex(int i, int j, int height, int width, char ** parallel){
        // Check if this is a valid index and not a wall
	return (i >= 0 && i < height && j >= 0 && j < width && parallel[i][j] != '#');
}

void FloodFill(int** array, bool** visited, int i, int j, int height, int width){ // Breadth First Search

        queue< pair < int, int > > myqueue; // create queue to take each index as they come in order

        // we want to prepare to check all Moore neighbors of our center index
        int row_neighbors[] = {0 , 0, -1, 1, 1, -1, -1, 1};  
        int col_neighbors[] = {-1, 1, 0, 0, -1, -1,  1, 1};
	// We create different combinations of '-1' '0' and '1' to look at each moore neighbor

        for(int h = 0; h < height; h++){
                for(int w = 0; w < width; w++){
                        visited[h][w] = false;
			// We need to keep track of what index we visited so we do not
			// check the same index again
			// set all indicies to false because we haven't started BFS yet
                }
        }

        myqueue.push(make_pair(i, j));  // enqueue the first index

        while(!myqueue.empty()){
         	// we need a pair because we need to use 
         	// the x and y corrdinates in our integer array

                pair<int, int> center_indices = myqueue.front();   
                int center_value = array[center_indices.first][center_indices.second];
                myqueue.pop(); // pop the first value

                int X = center_indices.first;
                int Y = center_indices.second;
                visited[X][Y] = true;
                for(int i = 0; i < 8; i++){ // check all 8 adjacent neighbors 
          		// Add each combination to get a adjacent neighbor to the center		
	                int x = X + row_neighbors[i];
                        int y = Y + col_neighbors[i];
                        /*
                        North 
                        West
                        South
                        NorthEast
                        NorthWest
                        SouthWest
                        SouthWest
                        */
                        if(validFloodindex(x, y, height, width, array) && visited[x][y] == false){ 
                                // We are checking all adjacent neighbors if they are impassible or not
                                // Also, we need to check if we have already visited them before
                                myqueue.push(make_pair(x, y));
                                array[x][y] = (center_value) + 1;
                                visited[x][y] = true;
                        }
                }
                
        }
}

bool GreedyAlgorithm(int** array, char** parallelArray, bool** visited, int width, int height, int startX, int startY){

	// similar to the BFS algorithm, instead we are looking for the least most value and adding it to our queue
        queue < pair <int, int> > myqueue;

	// All possible moore neighbors that are in bounds
        int row_neighbors[] = {0 , 0, -1, 1, 1, -1, -1, 1};
        int col_neighbors[] = {-1, 1, 0, 0, -1, -1,  1, 1};
        
	for(int h = 0; h < height; h++){
		for(int w = 0; w < width; w++){
			visited[h][w] = false;
		}
	} // preparation to visit all indicies by setting them to false
       

       myqueue.push(make_pair(startX, startY));

       while(!myqueue.empty()){
                pair<int, int> center = myqueue.front();
                myqueue.pop();

		// we use pairs to keep index of x and y coordinates
                int X = center.first;
                int Y = center.second;

		// if user chose start path on the same index of goal, then we return
                if(parallelArray[X][Y] == '$') return true;

                visited[X][Y] = true;

                int smallest = array[X][Y]; // initialize to smallest

                for(int i = 0; i < 8; i++){
                        int x = X + row_neighbors[i];
                        int y = Y + col_neighbors[i];
                        if(validParallelIndex(x, y, height, width, parallelArray) && visited[x][y] == false && array[x][y] < smallest){ // South East
                               // is it a valid index? Did we visit it before? Is it smaller than what we already have?
					
			        if(parallelArray[x][y] == '$') return true;
                                parallelArray[x][y] = '*';
                                myqueue.push(make_pair(x, y));
                                smallest = array[x][y]; // we change the smaller value to the new value
                                visited[x][y] = true; // mark it as visited
                        }
                }
        }


        return false; // if we never find the goal, return false

}

int main(int argc, char** argv)
{

        /*
        	Main should not have any logic in it, 
        	our only focus is to gather user input for height, width, start position, end goal, etc.
        	Some parts of the code should be self-explanatory and doesn't need commenting
        */



	// if user uses '-s' command then we ask for seed value
        int seed;
        if(argc == 2 && strcmp(argv[1], "-s") == 0){
                cout << "Enter the seed value: ";
                cin >> seed;
                srand(seed); 
        }
        else{
                srand(time(0)); 
        }

        int width, height, percentage;
        int impassible;
        char decision;
        int Goal_X_coordinate;
        int Goal_Y_coordinate;
        int positionX;
        int positionY;

	// Beginning of program
        cout << "Welcome to CSE240 WaveFront Pather" << endl;

        do{
                cout << "Enter the width (at least 10): ";
                cin >> width;
                cout << "Enter the height (at least 10): ";
                cin >> height;
        }while(width < 10 || height < 10);
	// we reprompt the user if they give us a value less than 10


        int **TwoDArr = new int*[height];
        for(int i = 0; i < height; i++){
                TwoDArr[i] = new int[width];
                for(int j = 0; j < width; j++){
                        TwoDArr[i][j] = 0;
                }
        }
	// create dynamically allocated 2d integer and boolean array (to keep track of visited indicies)
	bool **visited = new bool*[height];
        for(int i = 0; i < height; i++){
                visited[i] = new bool[width];
                for(int j = 0; j < width; j++){
                        visited[i][j] = false;
                }
        }


        
        do{
                cout << "Enter the percent of impassible terrain (0 - 100): ";
                cin >> percentage;
        }while(percentage < 0 || percentage  > 100);
	
	//this is simple user input with boundary checking, we ask and reprompt the user to get a valid value 
        while(percentage > 35){
                cout << "Having a value greater than 35% might create poor results, do you wish to continue? (y/n): ";
                cin >> decision;

                if(decision == 'n'){
                        cout << "Enter the percent of impassible terrain (0 - 100): ";
                        cin >> percentage; // if user keeps chosing 'n', then we we reprompt
                }
                else{
                        break; // user choses yes then we agree on it and create that many impassible objects
                }
        }
        
        int sizeOf2DArr = width * height;
        impassible = ((float)percentage / 100) * sizeOf2DArr; // calculates percentage

        for(int i = 0; i < impassible; i++){

		// we create a for loop to generate a '-1' n amount of times where n is the percentage
	
                int X_coordinate = (rand() % (width)) + 0;
                int Y_coordinate = (rand() % (height)) + 0;

                if(TwoDArr[X_coordinate][Y_coordinate] != -1){
                        TwoDArr[X_coordinate][Y_coordinate] = -1;
                }
                else{
                        impassible++;
                        // we increment "impassible" because we may generate a "-1" where there already is one
                        // This will waste an iteration so we increment "impassible" to compensate for this inconvenience
                }
        }

        PrintEnvironment(TwoDArr, width, height);

        do{
        cout << "Please enter the Goal Position X: ";
        cin >> Goal_X_coordinate;
        cout << "Please enter the Goal Position Y: ";
        cin >> Goal_Y_coordinate;
        }while(Goal_X_coordinate > height || Goal_X_coordinate < 0 ||
                Goal_Y_coordinate > width || Goal_Y_coordinate < 0);

	// user input and basic checking if we are inbounds

        if(TwoDArr[Goal_X_coordinate][Goal_Y_coordinate] < 0){
                do{
                        cout << "Sorry, that position is inside an obstacle\n";
                        cout << "Please enter the Goal Position X: ";
                        cin >> Goal_X_coordinate;
                        cout << "Please enter the Goal Position Y: ";
                        cin >> Goal_Y_coordinate;
                }while(TwoDArr[Goal_X_coordinate][Goal_Y_coordinate] < 0 || Goal_X_coordinate >= height || Goal_Y_coordinate >= width);
        }
        TwoDArr[Goal_X_coordinate][Goal_Y_coordinate] = 1; // Start of our wave


	FloodFill(TwoDArr, visited, Goal_X_coordinate, Goal_Y_coordinate, height, width); // Begin Breadth First Search based off of start of the wave

        PrintEnvironment(TwoDArr, width, height);

        char **pathof_TwoDArr = new char*[height];
        for(int i = 0; i < height; i++){
        // This creates a parallel array that reflects the integer array 
	       pathof_TwoDArr[i] = new char[width];
                for(int j = 0; j < width; j++){
			// finds all '-1's and make them to '#' in parallel array
                        if(TwoDArr[i][j] == -1){
                                pathof_TwoDArr[i][j] = '#';
                        }
                        else{
                                pathof_TwoDArr[i][j] = ' ';
                        }
                }
        }

        cout << "Result array: " << endl;
        Print_Parallel_Environment(pathof_TwoDArr, height, width);


        do{
                cout << "Please enter the Start Position X: ";
                cin >> positionX;
                cout << "Please enter the start Position Y: ";
                cin >> positionY;
        }while(positionX >= height || positionX < 0 || positionX >= width || positionX < 0);

	// Gather a starting point to go find the goal. Greedy algorithm should take care of this		

        if(TwoDArr[positionX][positionY] == -1){
                do{
                cout << "Sorry, that position is inside an obstacle" << endl;
                cout << "Please enter the Start Position X: ";
                cin >> positionX;
                cout << "Please enter the Start Position Y: ";
                cin >> positionY;
                }while(TwoDArr[positionX][positionY] == -1);
        }

        pathof_TwoDArr[positionX][positionY] = '@'; // initial start where we begin the greedy algorithm
									
        pathof_TwoDArr[Goal_X_coordinate][Goal_Y_coordinate] = '$'; // The goal

	if(GreedyAlgorithm(TwoDArr, pathof_TwoDArr, visited,  width, height, positionX, positionY) == false) {
                cout << "Path not found :C \n";
                Print_Parallel_Environment(pathof_TwoDArr, width, height);
        }
        else{
                Print_Parallel_Environment(pathof_TwoDArr, width, height);
        }



	// output results to the "wave.txt" file
	// Basic file output based off our results
        ofstream outFile;

        outFile.open("wave.txt"); // opens wave.txt file

        if(!outFile.is_open()){
                cout << "Error opening the file" << endl;
        }

        for(int i  = 0; i < height; i++){
                for(int j = 0; j < width; j++){
		      outFile << setw(4) << TwoDArr[i][j];
                }
                outFile << endl;
        }

        outFile << endl << endl;

        for(int i = 0; i < height; i++){
                for(int j = 0; j < width; j++){
			outFile << setw(2) << pathof_TwoDArr[i][j];
                }
                outFile << endl;
        }

        //close any files
        outFile.close();

        // destroy all dynamically allocated arrays
        for(int i = 0; i < height; i++){
                delete[] pathof_TwoDArr[i];
		delete[] TwoDArr[i];
		delete[] visited[i];
        }

	delete[] visited;
        delete[] TwoDArr;
        delete[] pathof_TwoDArr;
        return 0;
}
