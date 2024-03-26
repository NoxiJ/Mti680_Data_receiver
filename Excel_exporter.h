//
// Created by louis on 26/03/2024.
//

#ifndef MTI680_DATA_RECEIVER_EXCEL_EXPORTER_H
#define MTI680_DATA_RECEIVER_EXCEL_EXPORTER_H

#include <vector>
#include <string>
#include <iostream>
#include <libxl.h>
#include "MtiDataValues.h"

class Excel_exporter {

public:
    Excel_exporter(MtiDataValues &dataValues, const std::string &fileName);
    ~Excel_exporter();


    void writeData();
    void saveFile();

private:

    void writeAcceleration(libxl::Sheet *sheet, int indexRow, int rowCount);
    void writeGyroscope(libxl::Sheet *sheet, int indexRow, int rowCount);
    void writeMagnitude(libxl::Sheet *sheet, int indexRow, int rowCount);

    libxl::Book *_book;
    std::string _fileName;

    std::vector<XsVector> _accelerations;
    std::vector<XsVector> _gyroscopes;
    std::vector<XsVector> _magnitudes;
};


#endif //MTI680_DATA_RECEIVER_EXCEL_EXPORTER_H
