#include "../csv/CsvRow.cpp"
#include <string>
#include <iostream>
#include <fstream>
//std::istream& operator>>(std::istream& str, CSVRow& data)
//{
//    data.readNextRow(str);
//    return str;
//}   
int main()
{
    std::ifstream       file("input_data.csv");

    CSVRow              row;
    while(file >> row)
    {
        std::cout << "4th Element(" << stof(row[3]) << ")\n";
    }
}
