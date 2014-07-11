#ifndef CITYNODE_H_
#define CITYNODE_H_
#include "roadnode.hpp"
#include <string>

class RoadNode;

/*
 *	CityNode are linked lists to other CityNodes;
 *	Traverse until getID() is what we're looking for;
 *	or nextCity = NULL
*/

class CityNode
{
private:
	std::string		name;
	RoadNode*		roads;
	int				id; //index
	CityNode*		nextCity; //linked list form.
public:
	/*constr*/		CityNode(const std::string& name, int id);
	//void			getroad(){};
	//void			setName(const std::string& name);
	//int				getID(){return id;};
	//bool			addRoad(); //maybe pass a reference to the destination city?
	int				getID();
	std::string		getName();
	CityNode*		getNextCity();
	void			setNextCity(CityNode* whichCity);
	RoadNode*       getConnection();
	void            setConnection(RoadNode* newRoad);

};

#endif //CITYNODE_H_
