#include <vector>
#include <map>
#include <string>

typedef struct {
    double v1, v2, v3; 
} data3D; 

typedef struct {
    std::map<std::string, double> parameters;
    std::map<std::string, std::vector<data3D> > data;
} motion;

