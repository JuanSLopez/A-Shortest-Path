//Juan Lopez

//Shortest Path

//November 27,2018


#include <string>

#include <iostream>

#include <fstream>

#include <iomanip>

#include <vector>

#include <string>

#include <list>

#include <algorithm>

#include <cmath>

#include<stdio.h>


using namespace std;



struct connections;


void getConnections();



struct intersections

{

    int line;

    double longitude;

    double latitude;

    double dist;

    string state;

    string city;

    vector<connections*> paths;

    intersections* parent;

    double moveForward;

    double moveG;

    

    intersections();

    intersections(int line, double longitude, double latitude, double dist, string state, string city, vector<connections> &conMap);

    void getConnections(vector<connections*> &paths, vector<connections> &conMap, int line);

    void print() const;

    void printdistance() const;

    void printAllPaths() const;

    void setf(int f);

    void setg(int g);

};


struct connections

{

    string road;

    string type;

    int interA;

    int interB;

    double length;

    

    connections();

    connections(string road, string type, int interA, int interB, double length);

    void print() const;

    void print(int type) const;

    void printdist() const;

};


class map

{

    vector<intersections> interMap;

    vector<connections> conMap;

    

public:

    map();

    map(string intersF, string connectF);

    ~map();

    void createMap(string intersF, string connectF);

    intersections getintersec(int nodecode);

    void exploreMap(int nodecode);

    vector<connections*> getConnects(vector<intersections> interMap, int type);

    double discovery(int start, int end);

    intersections * startingPos(int start, int goal);

    int shortestPath(int s, int e);

    

};


connections::connections(){ }


connections::connections(string road, string type, int interA, int interB, double length)

{

    this->road = road;

    this->type = type;

    this->interA = interA;

    this->interB = interB;

    this->length = length;

}


void connections::print() const

{

    cout << this->road << ", " << this->length << " miles between location " << this->interA << " and location " << this->interB << ". (" << this->type << ")\n";

}


void connections::print(int type) const

{

    if (type == this->interA)

    {

        cout << this->road << ", " << this->length << " miles to " << this->interB << ". (" << this->type << ")\n";

    }

    else if (type == this->interB)

    {

        cout << this->road << ", " << this->length << " miles to " << this->interA << ". (" << this->type << ")\n";

    }

}


void connections::printdist() const

{

    cout << this->road << " " << this->length << " miles";

}


intersections::intersections(){ }


intersections::intersections(int line, double longitude, double latitude, double dist, string state, string city, vector<connections> &conMap)

{

    this->line = line;

    this->longitude = longitude;

    this->latitude = latitude;

    this->dist = dist;

    this->state = state;

    this->city = city;

    this->moveForward = -1;

    this->moveG = -1;

    this->parent = 0;

    //getConnections(this->paths, conMap, line);

}


void intersections::getConnections(vector<connections*> &paths, vector<connections> &conMap, int line)

{

    for (int i = 0; i < conMap.size(); i++)

    {

        if (conMap[i].interA == line || conMap[i].interB == line)

        {

            paths.push_back(&conMap[i]);

        }

    }

}


void intersections::print() const

{

    cout << "Location " << line << ", " << dist << " miles from " << city << ", " << state << "\n";

}


void intersections::printdistance() const

{

    cout << "Location " << line ;

}


void intersections::printAllPaths() const

{

    cout << "Location " << line << ", " << dist << " miles from " << city << ", " << state << "\n";

    for (int i = 0; i < paths.size(); i++)

    {

        paths[i]->print();

    }

}


void intersections::setf(int f)

{

    this->moveForward = f;

}


void intersections::setg(int g)

{

    this->moveG = g;

}


map::map(){ }


map::map(string intersF, string connectF)

{

    createMap(intersF, connectF);

}


map::~map(){ }


void map::createMap(string intersF, string connectF)

{

    fstream fin1;

    string str;

    int i = 0;

    int j = 0;

  /*  cout << connectF.c_str() << "\n"; */

    fin1.open(connectF.c_str());

    if (fin1.fail())

    {

        cerr << "Failed to open connections file." << endl;

    }

    else

    {

        while (getline(fin1, str))

        {

            while (str[i + j] != 32)

            {

                j++;

            }

            string road = str.substr(i, j);

            i = i + j + 1;

            j = 0;

            while (str[i + j] != 32)

            {

                j++;

            }

            string type = str.substr(i, j);

            i = i + j + 1;

            j = 0;

            while (str[i + j] != 32)

            {

                j++;

            }

            int interA = atoi(str.substr(i, j).c_str());

            i = i + j + 1;

            j = 0;

            while (str[i + j] != 32)

            {

                j++;

            }

            int interB = atoi(str.substr(i, j).c_str());

            i = i + j + 1;

            double length = atof(str.substr(i, string::npos).c_str());

            conMap.push_back(connections(road, type, interA, interB, length));

            i = 0;

            j = 0;

        }

    }

    

    fin1.close();

    

    

    fstream fin2;

    /*cout << "Read in all roads" << "\n";

    cout << intersF.c_str() << "\n";*/

    fin2.open(intersF.c_str());

    if (fin2.fail())

    {

        cerr << "Failed to open intersections file." << endl;

    }

    else

    {

        while (getline(fin2, str))

        {

            int line = i;

            double longitude = atof(str.substr(0, 9).c_str());

            double latitude = atof(str.substr(10, 9).c_str());

            double dist = atof(str.substr(20, 8).c_str());

            string state = str.substr(28, 2);

            string city = str.substr(31, string::npos);

            intersections inter = intersections(line, longitude, latitude, dist, state, city, conMap);

          

            interMap.push_back(inter);

            i++;

        }

        for(int i = 0; i < conMap.size(); i++)

        {

            interMap[conMap[i].interA].paths.push_back(&conMap[i]);

            interMap[conMap[i].interB].paths.push_back(&conMap[i]);

        }

    }

    fin2.close();

}


intersections map::getintersec(int nodecode)

{

    return interMap[nodecode];

}


void map::exploreMap(int nodecode){

    int x;

    int path;

    for (int i = 0; ; i++)

    {

        if (nodecode == interMap[i].line)

        {

            interMap[i].print();

            for (int j = 0; j < interMap[i].paths.size(); j++)

            {

                cout << j + 1 << ". ";

                interMap[i].paths[j]->print(interMap[i].line);

            }

            x = i;

            break;

        }

    }

    cout << "Select Path: ";

    cin >> path;

    if (path <= interMap[x].paths.size() && nodecode == interMap[x].paths[path - 1]->interA && path > 0)

    {

        path = interMap[x].paths[path - 1]->interB;

        exploreMap(path);

    }

    else if (path <= interMap[x].paths.size() && nodecode == interMap[x].paths[path - 1]->interB && path > 0)

    {

        path = interMap[x].paths[path - 1]->interA;

        exploreMap(path);

    }

    else

    {

        cout << "Invalid Path! Select Starting Location:  ";

        cin >> path;

        exploreMap(path);

    }

}


vector<connections*> map::getConnects(vector<intersections> interMap, int type)

{

    return interMap[type].paths;

}


double map::discovery(int start, int end)

{

    intersections s = getintersec(start);

    intersections e = getintersec(end);

    return sqrt(pow(s.longitude - e.longitude, 2) + pow(s.latitude - e.latitude, 2));

}


int contained(vector<intersections*> closedSet, int x)

{

    for(int i = 0; i < closedSet.size(); i++)

    {

        if(closedSet[i]->line == x)

        {

            return 1;

        }

    }

    return 0;

}


intersections * map::startingPos(int start, int goal)

{

    intersections test;

    vector<intersections*> closedSet;

    vector<intersections*> openSet;

    openSet.push_back(&interMap[start]);

    openSet[0]->moveG = 0;

    openSet[0]->moveForward = discovery(start, goal);

    double tempmoveG;

     intersections * current = NULL;

    while (!openSet.empty())

    {

    

        double ftest = 100000;

        for(int i = 0; i < openSet.size(); i++)

        {

            if(ftest > openSet[i]->moveForward && openSet[i]->moveForward >= 0)

            {

                ftest = openSet[i]->moveForward;

                current = openSet[i];

            }

        }

        if(current->line == goal)

        {

            return current;

        }

        openSet.erase(remove(openSet.begin(), openSet.end(), current ), openSet.end() );

        closedSet.push_back(current);

        for(int i = 0; i < current->paths.size(); i++)

        {

            if(contained(closedSet, current->paths[i]->interA) && contained(closedSet, current->paths[i]->interB))

            {

                continue;

            }

            else

            {

                tempmoveG = current->moveG + current->paths[i]->length;

            }

            

            int flag = -1;

            if(!contained(openSet, current->paths[i]->interA) && contained(closedSet, current->paths[i]->interB))

            {

                openSet.push_back(&interMap[current->paths[i]->interA]);

                flag = 1;

            }

            else if(!contained(openSet, current->paths[i]->interB) && contained(closedSet, current->paths[i]->interA))

            {

                openSet.push_back(&interMap[current->paths[i]->interB]);

                flag = 2;

            }

            else if(tempmoveG >= interMap[current->paths[i]->interA].moveG && interMap[current->paths[i]->interA].moveG >= 0 && flag == 1)

            {

                continue;

            }

            else if(tempmoveG >= interMap[current->paths[i]->interB].moveG && interMap[current->paths[i]->interB].moveG >= 0 && flag == 2)

            {

                continue;

            }

            if(flag == 1)

            {

                interMap[current->paths[i]->interA].parent = current;

                interMap[current->paths[i]->interA].moveG = tempmoveG;

                interMap[current->paths[i]->interA].moveForward = interMap[current->paths[i]->interA].moveG + discovery(current->paths[i]->interA, goal);

            }

            if(flag == 2)

            {

                interMap[current->paths[i]->interB].parent = current;

                interMap[current->paths[i]->interB].moveG = tempmoveG;

                interMap[current->paths[i]->interB].moveForward = interMap[current->paths[i]->interB].moveG + discovery(current->paths[i]->interB, goal);

            }

        }

    }

    return 0;

}


void printangle(intersections * a, intersections * b)

{

    double x = a->longitude - b->longitude;

    double y = a->latitude - b->latitude;

    double angle = 180 * atan(y / x);

    if(angle <= 22.5 && angle > 347.5)

    {

        cout << " E ";

    }

    else if(angle <= 67.5 && angle > 22.5)

    {

        cout << " NE ";

    }

    else if(angle <= 112.5 && angle > 67.5)

    {

        cout << " N ";

    }

    else if(angle <= 157.5 && angle > 112.5)

    {

        cout << " NW ";

    }

    else if(angle <= 202.5 && angle > 157.5)

    {

        cout << " W ";

    }

    else if(angle <= 247.5 && angle > 202.5)

    {

        cout << " SW ";

    }

    else if(angle <= 292.5 && angle > 247.5)

    {

        cout << " S ";

    }

    else

    {

        cout << " SE ";

    }

}


int map::shortestPath(int start, int end)

{

    intersections * path = startingPos(start, end);

    while(1)

    {

        cout << "From ";

        path->printdistance();

        cout << " take ";

        for(int i = 0; i < path->paths.size(); i++)

        {

            if(path->paths[i]->interA == path->line && path->paths[i]->interB == path->parent->line)

            {

                path->paths[i]->printdist();

                printangle(path, path->parent);

            }

            if(path->paths[i]->interB == path->line && path->paths[i]->interA == path->parent->line)

            {

                path->paths[i]->printdist();

                printangle(path, path->parent);

            }

        }

        cout << "to ";

        path->parent->printdistance();

        cout << "\n";

        path = path->parent;

        if(path->parent == 0)

        {

            break;

        }

    }

    

    return 0;

}


int main(void)

{

    

    cout << "USA map" << "\n";

    map * USA = new map("intersections.txt", "connections.txt");

  

    int start;

    int end;

    cout << "Select Starting Location:  ";

    cin >> start;

    cout << "Select Ending Location:  ";

    cin >> end;

    

    USA->shortestPath(end, start);

    

    

    return 0;

}
