#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// boost
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

int main() {
    //std::cout << "Hello World!\n";

    std::fstream infile("test.lvl");
    std::string line;
    int lineCount = 0;
    bool isLVL = false;

    while (std::getline(infile, line)) {
        if (line == "lvl1") {
            isLVL = true;
        }

        if (line[0] == '#') {
            continue;
        }

        if (lineCount > 0 && isLVL) {
            //std::cout << line << std::endl;

            // parsePos
            std::string model, name, tag; //0, 1, 2
            float x, y, z; //3,4,5
            float rx, ry, rz; //6,7,8
            float sx, sy, sz; //9,10,11
            int parsePos = 0;

            std::vector<std::string> tmp;
            boost::split(tmp, line, boost::is_any_of(","));

            model = tmp[0];
            name = tmp[1];
            tag = tmp[2];
            x = boost::lexical_cast<float>(tmp[3]);
            y = boost::lexical_cast<float>(tmp[4]);
            z = boost::lexical_cast<float>(tmp[5]);
            rx = boost::lexical_cast<float>(tmp[6]);
            ry = boost::lexical_cast<float>(tmp[7]);
            rz = boost::lexical_cast<float>(tmp[8]);
            sx = boost::lexical_cast<float>(tmp[9]);
            sy = boost::lexical_cast<float>(tmp[10]);
            sz = boost::lexical_cast<float>(tmp[11]);

            std::cout << "OUTPUT:\n";
            std::cout << " model: " << model << "\n name: " << name << "\n tag: " << tag << std::endl;
            std::cout << " pos: " << x << " " << y << " " << z << std::endl;
            std::cout << " rot: " << rx << " " << ry << " " << rz << std::endl;
            std::cout << " scale: " << sx << " " << sy << " " << sz << std::endl;
        }
        lineCount++;
    }
}