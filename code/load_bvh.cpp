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
//    Eigen::Matrix<double, 4, 1> position;
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
//    position << 0, 0, 0, 0;
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

//    position << global_matrix(0,3) , global_matrix(1,3) , global_matrix(2,3) , global_matrix(3, 3); 
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

size_t string_split(const std::string & input, const std::string & delimiter, std::vector<std::string> & output){
    size_t current;
    size_t next = -1;
    
    output.clear();
    do{
        current = next + 1;
        next = input.find_first_of(delimiter, current);
        output.push_back(input.substr(current, next - current));
    } while(next != std::string::npos);

    return current;
}

inline std::vector<std::string>::iterator string_element(std::vector<std::string> & s, const std::string & match){
    return find(s.begin(), s.end(), match);
}

inline int find_in_line(const std::string &line, const std::string &s){
    return line.find(s) != std::string::npos ? 1 : 0;
}

// In C++11 exists the stod functions, which converts strings to doubles. My current version of g++ 4.8.4 does not supports it. 

inline double convertToDouble(const std::string & s){
    std::istringstream i(s);
    double x;
    if(!(i >> x))
        std::cout << "Wrong convertion to double for " << s << ";" << std::endl;
    return x;
}

int load_bvh_file(std::ifstream &f, motion *m){

    std::vector<Joint *> parents;
    std::vector<Joint> joints;
    std::string line;
    std::vector<std::string> line_splited;
    std::vector<std::string>::iterator index;

    // This loop loads joints structure in memory
    while( std::getline(f, line) ){
        if( find_in_line(line, "ROOT") ){
            string_split(line, " ", line_splited);
            index = string_element(line_splited, "ROOT");
            // name, isRoot = 1, isEnd = 0, parent -> void
            Joint TmpJoint(*++index, 1, 0, 0); 
            joints.push_back(TmpJoint);
        }else if( find_in_line(line, "JOINT") ){
            string_split(line, " ", line_splited);
            index = string_element(line_splited, "JOINT");
            // name, isRoot = 0, isEnd = 0, parent -> last parent
            Joint TmpJoint(*++index, 0, 0, parents.back() );
            joints.push_back(TmpJoint);
        }else if( find_in_line(line, "End") ){
            // End + parent_name, isRoot = 0, isEnd = 1, parent -> last parent 
            Joint TmpJoint("End" + parents.back() -> name, 0, 1, parents.back() );
            joints.push_back(TmpJoint);
        }else if( find_in_line(line, "{") ){
            parents.push_back( &joints.back() );
        }else if( find_in_line(line, "}") ){
            parents.pop_back();
        }else if( find_in_line(line, "OFFSET") ){
            string_split(line, " ", line_splited);
            index = string_element(line_splited, "OFFSET");
            (joints.back().local_matrix) (0, 3) = convertToDouble(*++index);
            (joints.back().local_matrix) (1, 3) = convertToDouble(*++index);
            (joints.back().local_matrix) (2, 3) = convertToDouble(*++index);
        }else if( find_in_line(line, "MOTION") )
            break;
    }

    // Creates new entries for each joint
    for(std::vector<Joint>::iterator it = joints.begin(); it != joints.end(); ++it)
        (m -> data)[(*it).name];
        
    // Saves frames and frame_time parametres
    std::getline(f, line);
    string_split(line, "\t", line_splited);
    index = string_element(line_splited, "Frames:");
    (m -> parameters)["frames"] =  convertToDouble(*++index);

    std::getline(f, line);
    string_split(line, "\t", line_splited);
    index = string_element(line_splited, "Frame Time:");
    (m -> parameters)["frame_time"] = convertToDouble(*++index);

    std::vector<Joint>::iterator j;
    while( std::getline(f, line) ){
        string_split(line, " ", line_splited);
        line_splited.pop_back();
        index = line_splited.begin();

        for(j = joints.begin(); j != joints.end(); ++j){
            if( (*j).isRoot ){
                (*j).local_matrix(0, 3) = convertToDouble( *index );
                (*j).local_matrix(1, 3) = convertToDouble( *++index);
                (*j).local_matrix(2, 3) = convertToDouble( *++index);
            }
        }
    }

    return 1;
}

int main(int argc, char *argv[]){

    motion user1;
    std::ifstream file(argv[1], std::ifstream::binary);
    
    load_bvh_file(file, &user1);

    return 0;
}
