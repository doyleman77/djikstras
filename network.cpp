#include "network.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stack>
#include <process.h>
#include <Windows.h>



Network::Network()
{
    cityCount = 0;
	cityList = NULL;
    distanceArray = NULL;
    pathArray = NULL;
    includeArray = NULL;
}

//used in loadNetwork()
void Network::clearNetwork()
{
	CityNode* cityLocate = cityList;
	CityNode* cityTrav = cityLocate;
	RoadNode* roadLocate = NULL;
	RoadNode* roadTrav = NULL;
	while(cityLocate != NULL)
	{
		if(cityLocate->getConnection())
		{
			roadLocate = cityLocate->getConnection();
			roadTrav = roadLocate->getNextRoad();
		}
		while(roadLocate != NULL)
		{
			delete roadLocate;
			roadLocate = roadTrav;
			if(roadTrav != NULL)
				roadTrav = roadTrav->getNextRoad();

		}
		if(cityLocate->getNextCity() != NULL){
			cityLocate = cityLocate->getNextCity();}
		else{
			cityLocate = NULL;}
		delete cityTrav;
		cityTrav = NULL;
		cityTrav = cityLocate;
	}
	cityList = NULL;
};

bool Network::findPath(const std::string& cityA, const std::string& cityB)
{
	if(cityList == NULL)
	{
		std::cout << "No cities to navigate!" << std::endl;
		return false;
	}

	CityNode* cityATrav = cityList;
	CityNode* cityBTrav = cityList;
	RoadNode* roadTrav = cityList->getConnection();
	bool checkCityA(false), checkCityB(false);

	//make sure the cities exist at all.
	for(int x = 0; x < cityCount; x++)
	{
		if(cityATrav->getName() == cityA)
			checkCityA = true;
		if(cityATrav->getName() == cityB)
			checkCityB = true;
		cityATrav = cityATrav->getNextCity();
	}

	if(!checkCityA)
	{
		std::cout << cityA << " does not exist!" << std::endl;
		return false;
	}

	if(!checkCityB)
	{
		std::cout << cityB << " does not exist!" << std::endl;
		return false;
	}

	cityATrav = cityList;
	distanceArray = new unsigned int[cityCount];
	includeArray = new bool[cityCount];
	pathArray = new unsigned int[cityCount];
	cityArray = new CityNode*[cityCount];

	//initialize the cities
	for(int x = 0; x < cityCount; x++)
	{
		cityArray[x] = cityATrav;
		cityATrav = cityATrav->getNextCity();
	}

	//find the city we are starting off at
	cityATrav = cityList;
	while((cityATrav->getName() != cityA) && (cityATrav != NULL))
	{
		cityATrav = cityATrav->getNextCity();
	}

	//find the city we are going to
	cityBTrav = cityList;
	while((cityBTrav->getName() != cityB) && (cityBTrav != NULL))
	{
		cityBTrav = cityBTrav->getNextCity();
	}

	//initialize the pathfinding arrays
	for (int x = 0; x < cityCount; x++)
	{
		distanceArray[x] = 99999999;
		includeArray[x] = false;
		pathArray[x] = cityATrav->getID();
	}
	roadTrav = cityATrav->getConnection();
	while(roadTrav != NULL)
	{
		distanceArray[roadTrav->getCityID()] = roadTrav->getDistance();
		roadTrav = roadTrav->getNextRoad();
	}
	distanceArray[cityATrav->getID()] = 0;
	includeArray[cityATrav->getID()] = true;
	/*
	 *	Done Initializing the information required for proper path finding!
	 *	Now we can use this to hopefully find our shortest path. hopefully...
	*/
    while(includeArray[cityBTrav->getID()] != true)
    {
        int bestDistance = 999999999;
        int bestLocation = -1;
        for(int x = 0; x < cityCount; x++)
        {
            if((distanceArray[x] < bestDistance) && (includeArray[x] == false))
            {
                bestDistance = distanceArray[x];
                bestLocation = x;
            }
        }

		includeArray[bestLocation] = true;
		roadTrav = cityArray[bestLocation]->getConnection();
		int neighborDistance = 0;
		while(roadTrav != NULL)
		{
			neighborDistance = roadTrav->getDistance();
			if((distanceArray[bestLocation] + roadTrav->getDistance()) <  distanceArray[roadTrav->getCityID()])
			{
				distanceArray[roadTrav->getCityID()] = distanceArray[bestLocation] + roadTrav->getDistance();
				pathArray[roadTrav->getCityID()] = bestLocation;
			}
			roadTrav = roadTrav->getNextRoad();
		}
	}
	if(distanceArray[cityBTrav->getID()] == 99999999)
	{
		std::cout << "There was no path to " << cityB << "." << std::endl;
		std::cout << "Exiting the search..." << std::endl;
		return false;
	}
	else
	{
		std::cout << "The shortest way to " << cityB << " from " << cityA << " is:" << std::endl;
		std::cout << cityA << " to" << std::endl;
		std::string tempPathNames;
		int pathFlow = cityBTrav->getID();
		std::stack<std::string> completePath;

		while(pathFlow != cityATrav->getID())
		{
			completePath.push(cityArray[pathFlow]->getName());
			pathFlow = pathArray[pathFlow];
		}
		while(!completePath.empty())
		{
			tempPathNames = completePath.top();
			std::cout << tempPathNames;
			completePath.pop();
			if(!completePath.empty())
			{
				std::cout << " to " << std::endl;
			}
		}
		std::cout << std::endl << "For a total distance of: " <<
			distanceArray[cityBTrav->getID()] << " miles!" << std::endl;
	}
	
	//figure minutes
	for (int x = 0; x < cityCount; x++)
	{
		distanceArray[x] = 99999999;
		includeArray[x] = false;
		pathArray[x] = cityATrav->getID();
	}
	roadTrav = cityATrav->getConnection();
	while(roadTrav != NULL)
	{
		distanceArray[roadTrav->getCityID()] = roadTrav->getMinutes();
		roadTrav = roadTrav->getNextRoad();
	}
	distanceArray[cityATrav->getID()] = 0;
	includeArray[cityATrav->getID()] = true;
	/*
	 *	Done Initializing the information required for proper path finding!
	 *	Now we can use this to hopefully find our shortest path. hopefully...
	*/
    while(includeArray[cityBTrav->getID()] != true)
    {
        int bestMinutes = 999999999;
        int bestLocation = -1;
        for(int x = 0; x < cityCount; x++)
        {
            if((distanceArray[x] < bestMinutes) && (includeArray[x] == false))
            {
                bestMinutes = distanceArray[x];
                bestLocation = x;
            }
        }

		includeArray[bestLocation] = true;
		roadTrav = cityArray[bestLocation]->getConnection();
		int neighborDistance = 0;
		while(roadTrav != NULL)
		{
			neighborDistance = roadTrav->getMinutes();
			if((distanceArray[bestLocation] + roadTrav->getMinutes()) <  distanceArray[roadTrav->getCityID()])
			{
				distanceArray[roadTrav->getCityID()] = distanceArray[bestLocation] + roadTrav->getMinutes();
				pathArray[roadTrav->getCityID()] = bestLocation;
			}
			roadTrav = roadTrav->getNextRoad();
		}
	}
	std::cout << std::endl << "The quickest way to " <<
		cityB << " from " << cityA << " is:" << std::endl;

	std::cout << cityA << " to" << std::endl;
	std::string tempPathNames;
	int pathFlow = cityBTrav->getID();
	std::stack<std::string> completePath;

	while(pathFlow != cityATrav->getID())
	{
		completePath.push(cityArray[pathFlow]->getName());
		pathFlow = pathArray[pathFlow];
	}
	while(!completePath.empty())
	{
		tempPathNames = completePath.top();
		std::cout << tempPathNames;
		completePath.pop();
		if(!completePath.empty())
		{
			std::cout << " to " << std::endl;
		}
	}
	std::cout << std::endl << "For a total time of: " <<
		distanceArray[cityBTrav->getID()] << " minutes !" << std::endl;

	delete[] distanceArray;
	delete[] includeArray;
	delete[] pathArray;
	delete[] cityArray;
	return true;
};

bool Network::addCity(const std::string& cityName)
{
	CityNode* newCity = new CityNode(cityName, cityCount);
	cityCount++; //this conveniently helps set new city IDs
	if(cityList == NULL) //First city created
	{
        cityList = newCity;
        return true;
	}

	CityNode* trav = cityList;
		/*move down the list of cities; checking to see if
		 *the city already exists or we hit the end.
		*/
	bool foundLocation = false;
	while(foundLocation == false)
	{
        if(trav->getName() == cityName)
        {
            foundLocation = true;
            return false;
        }
        else if(trav->getNextCity() == NULL) //found new locatoin
        {
            trav->setNextCity(newCity);
            return true;
        }
        else if(trav->getNextCity() != NULL)
        {
            trav = trav->getNextCity(); //Move down the list
        }
	}

};


bool Network::addConnection(const std::string& cityA, const std::string& cityB,
                            int distance, int time)
{
	//          /* City A Locate */
	CityNode*   cityAtrav = cityList;
	//navigate to find the city
	while((cityAtrav->getName() != cityA) && cityAtrav->getNextCity() != NULL)
	{
        cityAtrav = cityAtrav->getNextCity();
	}
	if((cityAtrav->getNextCity() == NULL) && (cityAtrav->getName() != cityA))
    {
        std::cout << "City does not exist!" << std::endl;
        return false;
    }

    //          /*  City B Locate */
	CityNode*   cityBtrav = cityList;

		//navigate to find the city
	while((cityBtrav->getName() != cityB) && cityBtrav->getNextCity() != NULL)
	{
        cityBtrav = cityBtrav->getNextCity();
	}
	if((cityBtrav->getNextCity() == NULL) && (cityBtrav->getName() != cityB))
    {
        std::cout << "City does not exist!" << std::endl;
        return false;
    }

    /*  Found the cities; now we need to see:
     *  1) if the connection exists already
     *  2) if it is; should we prompt to overwrite?
    */
    RoadNode*   connectionPoint = cityAtrav->getConnection();

	//first road  */
    if(connectionPoint == NULL)
    {
        RoadNode* newRoad = new RoadNode(cityBtrav);
        newRoad->setDistance(distance);
        newRoad->setMinutes(time);
        cityAtrav->setConnection(newRoad);
        std::cout << "Successfully added a road to " << cityB << "!" << std::endl;
        return true;
    }

    while((connectionPoint->getNextRoad() != NULL) &&
         (connectionPoint->getCityLink()->getName() != cityBtrav->getName()))
    {
        connectionPoint = connectionPoint->getNextRoad();
    }

	// City exists already!  */
    if(connectionPoint->getCityLink()->getName() == cityBtrav->getName())
    {
        std::cout << "Roads already exist from " << cityA <<
        " to " << cityB <<"!" << std::endl;

        return false;
    }
    else if(connectionPoint->getNextRoad() == NULL)
    {
        RoadNode* newRoad = new RoadNode(cityBtrav);
        newRoad->setDistance(distance);
        newRoad->setMinutes(time);

        connectionPoint->setNextRoad(newRoad);
        std::cout << "Successfully added a road to " << cityB << "!" << std::endl;
        return true;
    }


};

bool Network::loadNetwork(const std::string& networkFile)
{
	if(cityList != NULL)
	{
		std::cout << "You will lose this current network if you load now!" << std::endl;
		std::cout << "Are you sure?\ny for yes, anything else for no." << std::endl;
		std::string input;
		std::getline(std::cin, input);
		if(input != "y")
		{
			std::cout << "Cancelling load..." << std::endl;
			return false;
		}
	}
	cityCount = 0;
	clearNetwork();
	std::ifstream file;
	std::string delim = "\t";
	file.open(networkFile.c_str());
	std::string fileLine;
	std::string cityA;
	std::string cityB;
	std::string tempValues;
	unsigned int minutes = 0;
	unsigned int distance = 0;
	char junk[4] = "";
	while (!file.eof())
	{
		while(file.peek() != '*')
		{
			//file >> cityName;
			std::getline(file,fileLine);
			if(fileLine.find('\t') != std::string::npos)
			{
				int loc = fileLine.find('\t');
				fileLine.erase(loc, fileLine.size()-1);
			}
			//file.getline(junk, 10);
			addCity(fileLine);
		}
		std::getline(file, fileLine);

		while(file.peek() != '*')
		{
			std::getline(file, fileLine);
            unsigned int start = 0;
            unsigned int end = fileLine.find(delim);
            cityA = fileLine.substr(start, end);
			if(cityA.back() == ' ')
			{
				cityA.erase(cityA.size()-1);
			}


			fileLine.erase(start, end+1);

			start = 0;
            end = fileLine.find(delim);
            cityB = fileLine.substr(start, end);
			fileLine.erase(start, end+1);
			if(cityB.back() == ' ')
			{
				cityB.erase(cityB.size()-1);
			}
			start = 0;
            end = fileLine.find(delim);
            tempValues = fileLine.substr(start, end);
            std::stringstream(tempValues) >> minutes;
            fileLine.erase(start, end+1);

			start = 0;
            end = fileLine.find(delim);
            tempValues = fileLine.substr(start, end);
            std::stringstream(tempValues) >> distance;

			addConnection(cityA, cityB, distance, minutes);
            /*std::cout << cityA << " " << cityB << " " <<
            distance << " " << minutes << std::endl;*/
            //std::getline(file,fileLine);
		}
		return true;
	}
	file.close();
	return true;
};

bool Network::saveNetwork(const std::string& networkFile)
{
	std::ofstream file(networkFile.c_str());
	CityNode* cityTrav = cityList;
	RoadNode* roadTrav = cityList->getConnection();

	while(cityTrav != NULL)
	{
		file << cityTrav->getName();
		file << std::endl;
		cityTrav = cityTrav->getNextCity();
	}
	file << "*" << std::endl;
	cityTrav = cityList;
	while(cityTrav != NULL)
	{
		while(roadTrav != NULL)
		{
			file << cityTrav->getName() <<
				"\t" << roadTrav->getCityLink()->getName();
			file << "\t" << roadTrav->getDistance();
			file << "\t" << roadTrav->getMinutes() << std::endl;
			roadTrav = roadTrav->getNextRoad();
		}
		cityTrav = cityTrav->getNextCity();
		if(cityTrav != NULL)
			roadTrav = cityTrav->getConnection();
	}
	file << "*";
	file.close();
	return true;
};

void Network::listCities()
{
    CityNode*   cityTrav = cityList;

    std::cout << "All cities within the network:" << std::endl;
    std::cout << "------------------------------" << std::endl;
    while(cityTrav != NULL)
    {
        std::cout << "ID: " << cityTrav->getID() << "";
        std::cout << "\t|\t" << cityTrav->getName() << std::endl;
        cityTrav = cityTrav->getNextCity();
    }
    std::cout << "------------------------------" << std::endl;

};

void Network::showNetwork()
{
	if(cityList == NULL)
	{
		std::cout << "No cities to show!" << std::endl;
		return;
	}
    CityNode*   cityTrav = cityList;
    RoadNode*   roadTrav = cityTrav->getConnection();
    std::cout << "Rough map of the cities and roads within the network:" << std::endl;
    std::cout << "_____________________________________________________" << std::endl;
    std::cout << "From \t To \t    Miles \t    Minutes" << std::endl;
    std::cout << "_____________________________________________________" << std::endl;
    while(cityTrav != NULL)
    {
        std::cout << cityTrav->getName() << std::endl;
        while(roadTrav != NULL)
        {
            std::cout << "\t" << roadTrav->getCityLink()->getName();
            std::cout << "    " << roadTrav->getDistance() << " miles";
            std::cout << "    " << roadTrav->getMinutes() << " mins";
            std::cout << std::endl;
            roadTrav = roadTrav->getNextRoad();
        }
        std::cout << "--------------------------" << std::endl;
        cityTrav = cityTrav->getNextCity();
        if(cityTrav != NULL)
            roadTrav = cityTrav->getConnection();
    }
    std::cout << "_____________________________________________________" << std::endl;
};

void Network::printMenu()
{
	std::cout << std::endl;
    std::cout << "What would you like to do?" << std::endl;
    std::cout << "1)    Add city to network" << std::endl;
    std::cout << "2)    Add a connection between two cities" << std::endl;
    std::cout << "3)    Save Network to file" << std::endl;
    std::cout << "4)    Load Network from file" << std::endl;
    std::cout << "5)    Find shortest path" << std::endl;
    std::cout << "6)    List cities" << std::endl;
    std::cout << "7)    Show Network" << std::endl;
    std::cout << "8)    Quit" << std::endl;
};

int Network::runNetwork()
{
    bool running = true;
    unsigned int menuChoice = 0;

    while(running)
    {
        printMenu();
        std::cin >> menuChoice;
        switch(menuChoice)
        {
            case 1:     //add city
                {
                std::cout << "What is the name?" << std::endl;
                std::string tempName;
                std::cin.ignore();
                std::getline(std::cin, tempName);// >> tempName;
                if(addCity(tempName))
                    std::cout << "City added!" << std::endl;
                else
                    std::cout << "City exists already!" << std::endl;
                }
                break;
            case 2:     //add connection
                {
                std::cout << "What city would you like to connect?" << std::endl;
                std::string cityA, cityB;
                std::cin.ignore();
                std::getline(std::cin, cityA);
                std::cout << "And to which city?" << std::endl;
                std::getline(std::cin, cityB);
                std::cout << "How long (minutes) does it take to get there?" << std::endl;
                unsigned int time = 0;
                std::cin >> time;
                std::cout << "And how long is it, in miles?" << std::endl;
                unsigned int distance = 0;
                std::cin >> distance;
                if(addConnection(cityA, cityB, distance, time))
                    std::cout << "Connection added!" << std::endl;
                else
                    std::cout << "Problem creating connection!" << std::endl;
                }
                break;
            case 3:     //save network
				{
				std::string filename;
				std::cout << "What would you like to name the file?" << std::endl;
				std::cout << "files are automatically put in .txt format" << std::endl;
				std::cin.ignore();
				std::getline(std::cin, filename);
				filename += ".txt";
				saveNetwork(filename);
				}
                break;
            case 4:     //load network
				{
				std::string filename;
				std::cout << "What file would you like to load?" << std::endl;
				std::cout << "files are automatically loaded from .txt format" << std::endl;
				std::cin.ignore();
				std::getline(std::cin, filename);
				filename += ".txt";
				loadNetwork(filename);
				}
				break;
            case 5:     //find path
				{
				std::cout << "From what city would you like to start?" << std::endl;
				std::string cityA;
				std::string cityB;
				std::cin.ignore();
				std::getline(std::cin, cityA);
				std::cout << "And to which city would you like to navigate?" << std::endl;
				//std::cin.ignore();
				std::getline(std::cin, cityB);

				findPath(cityA, cityB);

				}
				break;
            case 6:     //list cities
                listCities();
                break;
            case 7:     //show network
                showNetwork();
                break;
            case 8:
                running = false;
                break;
            default:
                std::cout << "Invalid input!" << std::endl;
                std::cin.ignore();
                break;
        }
    }
	return 0;
};
