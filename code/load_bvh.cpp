#include <iostream>
#include <string>
#include <fstream>
#include <Eigen>
#include <cmath>
#include <vector>
#include <algorithm>
#include "data.hpp"

class Joint {
  public:
    std::string name;
    int isRoot;
    int isEnd;
    Joint *parent;
    Eigen::Matrix<double, 3, 1> rotation;
    Eigen::Matrix<double, 4, 1> position;
    Eigen::Matrix<double, 4, 4> local_matrix;
    Eigen::Matrix<double, 4, 4> global_matrix;    

    Joint(const std::string set_name, int set_root, int set_end, Joint *set_parent);
    void calculate_position();
    void calculate_local_matrix();
};

Joint::Joint(const std::string set_name, int set_root, int set_end, Joint *set_parent){
    name = set_name;
    isRoot = set_root;
    isEnd = set_end;
    parent = set_parent;
    rotation << 0, 0, 0;
    position << 0, 0, 0, 0;
    local_matrix  << 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1;
    global_matrix << 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1;
}

void Joint::calculate_position(){
    if(!isEnd)
        Joint::calculate_local_matrix();
    if(isRoot)
        global_matrix = local_matrix;
    else
        global_matrix = (parent -> global_matrix) * local_matrix;

    position << global_matrix(0,3) , global_matrix(1,3) , global_matrix(2,3) , global_matrix(3, 3); 
}

void Joint::calculate_local_matrix(){
    double a = std::cos(M_PI * rotation(1,0)/180);
    double b = std::sin(M_PI * rotation(1,0)/180);
    double c = std::cos(M_PI * rotation(2,0)/180);
    double d = std::sin(M_PI * rotation(2,0)/180);
    double e = std::cos(M_PI * rotation(0,0)/180);
    double f = std::sin(M_PI * rotation(0,0)/180);

    local_matrix(0,0) = c*e - b*d*f;
    local_matrix(0,1) = -a*f; 
    local_matrix(0,2) = d*e + b*c*f; 
    local_matrix(1,0) = c*f + b*d*e; 
    local_matrix(1,1) = a*e; 
    local_matrix(1,2) = d*f - b*c*e; 
    local_matrix(2,0) = -a*d; 
    local_matrix(2,1) = b; 
    local_matrix(2,2) = a*c;
}

size_t string_split(std::string &input, std::string delimiter, std::vector<std::string> &output){
    size_t current;
    size_t next = -1;
    
    output.clear();
    do{
        current = next + 1;
        next = input.find_first_of(delimiter, current);
        output.push_back(input.substr(current, next - current));
    } while(next != std::string::npos);
}

int find_in_line(std::string &line, const std::string &s){
    size_t found;
    found = line.find(s);
    return found != std::string::npos ? 1 : 0;
}

int load_bvh_file(std::ifstream &f, motion *m){

    std::vector<Joint> parents;
    std::vector<Joint> joints;
    std::string line;
    std::string delimiter = " ";
    std::vector<std::string> line_splited;
    std::vector<std::string>::iterator index;

    while(!f.eof()){
        std::getline(f, line);
        if( find_in_line(line, "ROOT") ){
            string_split(line, delimiter, line_splited);
            index = find(line_splited.begin(), line_splited.end(), "ROOT");
            Joint TmpJoint(*++index, 1, 0, 0); // name=root, isRoot = 1, isEnd = 0, parent -> void
            joints.push_back(TmpJoint);
        }
    }
    
    std::cout << "Root name: " << joints.back().name << std::endl;

    return 1;
}

int main(int argc, char *argv[]){

    motion user1;
    std::ifstream file(argv[1], std::ifstream::in);
    
    load_bvh_file(file, &user1);

    return 0;
}
