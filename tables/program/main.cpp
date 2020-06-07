#include <iostream>
#include <fstream>
#include <cstdlib>
#include "cout.h"
using std::cout; using std::endl; using std::size_t;

void openFile(const std::string& fileName, Table& table)
{
    std::string line;
    std::ifstream file;
    file.open(fileName, std::ios_base::in);

    if(file.is_open())
    {
        while(std::getline(file, line))
        {
            size_t pos_found = line.find('\t');
            if(pos_found != std::string::npos)
            {
                std::string strData = line.substr(0, pos_found);
                std::string strNum = line.substr(pos_found + 1);
                size_t nNum = (size_t)atoi(strNum.c_str());
                table.addData(strData, nNum);
            }
        }
    }
    file.close();
}

int main()
{
    Table t1("Numbers", 1);
    Table t2("Alphabet", 2);
    Table t3("Keys", 3);
    Table t4("Months", 4);

    std::string filename_TableData1("table1.txt");
    std::string filename_TableData2("table2.txt");
    std::string filename_TableData3("table3.txt");
    std::string filename_TableData4("table4.txt");

    openFile(filename_TableData1, t1);
    openFile(filename_TableData2, t2);
    openFile(filename_TableData3, t3);
    openFile(filename_TableData4, t4);

    cout << t1 << endl;
    cout << t2 << endl;
    cout << t3 << endl;
    cout << t4 << endl;

    //system("pause");
    return 0;
}
