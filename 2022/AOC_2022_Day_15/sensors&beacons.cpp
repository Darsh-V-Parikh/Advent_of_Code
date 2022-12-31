#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define manhattanDistance(x1, y1, x2, y2)  ( abs(x1 - x2) + abs(y1 - y2) )

typedef long long Len_t;
typedef long long Num_Scans;

struct _2dPoint {
    Len_t x, y;
};
typedef _2dPoint point_t;

struct _sensor {
    point_t loc;        // coordinate of the sensor
    Len_t dist;     // distance to closest beacon
};
typedef _sensor sensor_t;

std::vector<sensor_t> Sensors;
Len_t Min_X = 0, Max_X = 0, Min_Y = 0, Max_Y = 0, X_dim, Y_dim;
std::vector<point_t> Unscanned;

void getSensorData(std::string sensor_str) {
    std::stringstream new_sensor_data(sensor_str);
    
    std::string s;
    char c;
    int x, y;
    sensor_t new_sensor;

    new_sensor_data >> s >> s >> c >> c >> x >> c >> c >> c >> y >> c;
    new_sensor.loc.x = x;
    new_sensor.loc.y = y;
    new_sensor_data >> s >> s >> s >> s >> c >> c >> x >> c >> c >> c >> y;
    new_sensor.dist = manhattanDistance(new_sensor.loc.x, new_sensor.loc.y, x, y);

    if (new_sensor.loc.x - new_sensor.dist < Min_X) {
        Min_X = new_sensor.loc.x - new_sensor.dist;
    }
    if (new_sensor.loc.x + new_sensor.dist > Max_X) {
        Max_X = new_sensor.loc.x + new_sensor.dist;
    }
    if (new_sensor.loc.y - new_sensor.dist < Min_Y) {
        Min_Y = new_sensor.loc.y - new_sensor.dist;
    }
    if (new_sensor.loc.y + new_sensor.dist > Max_Y) {
        Max_Y = new_sensor.loc.y + new_sensor.dist;
    }
    Sensors.push_back(new_sensor);

    std::cout << "Found Sensor at (" << new_sensor.loc.x << "," << new_sensor.loc.y 
              << ") which distance of " << new_sensor.dist << std::endl;
}

//------------------------------------------------------------------------------

int main(int argc, char **argv) {
    if (argc <= 1 || argc > 2) {
        std::cout << "enter the filename while calling the program" << std::endl;
        return 1;
    }
    std::ifstream sensor_data(argv[1], std::ios::in);
    if (!sensor_data.is_open()) {
        std::cout << "cannot open " << argv[1] << std::endl;
        return 1;
    }

    
    std::string new_sensor;
    while (std::getline(sensor_data, new_sensor)) {
        getSensorData(new_sensor);
    }

    sensor_data.close();
    std::cout << "==== DONE ====" << std::endl;
    return 0;
}

//------------------------------------------------------------------------------