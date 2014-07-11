#ifndef NETWORK_H_
#define NETWORK_H_
#include "citynode.hpp"
#include <string>

class Network
{
private:
	CityNode*		cityList;
	int				cityCount;
    unsigned int*   distanceArray;
    unsigned int*   pathArray;
	CityNode**		cityArray;
    bool*           includeArray;
    void            printMenu();
    bool			addCity(const std::string& cityName);
	void			clearNetwork();
                    /*
					/*	Think of CityA as the first city to recieve the connection, and B as the 'destination'
					/*	of this connection.
					/*
					/*	Using the first addConnection sets equal time and distance to both cityA and cityB
					*/
    bool			addConnection(const std::string& cityA, const std::string& cityB,
                                  int distance, int time);
    bool			loadNetwork(const std::string& networkFile);
	bool			saveNetwork(const std::string& networkFile);
    bool            findPath(const std::string& cityA,
                             const std::string& cityB);
    void            listCities();
    void            showNetwork();

public:
    Network();

	int				getCityCount(){return cityCount;};


	int				runNetwork();
};

#endif //NETWORK_H_
