//
// Created by louis on 26/03/2024.
//

#include "Excel_exporter.h"

Excel_exporter::Excel_exporter(MtiDataValues &dataValues, const std::string &fileName) {
    this->_accelerations    = dataValues.getAccelerations();
    this->_gyroscopes       = dataValues.getGyroscopes();
    this->_magnitudes       = dataValues.getMagnitudes();
    this->_fileName         = fileName;

    _book                   = xlCreateBook();
    if (_book) {

        _book->addSheet("Feuil1");

    }
}

Excel_exporter::~Excel_exporter() = default;

void Excel_exporter::saveFile() {
    if (_book) {
        _book->save(_fileName.c_str());
        _book->release();
    }
}

void Excel_exporter::writeData() {
    if (!_book) {
        return;
    }

//    int rowCount = _accelerations.size();
    int rowCount = 20;
    libxl::Sheet *sheet = _book->getSheet(0);
    if (!sheet) {
        sheet = _book->addSheet("Feuil1");
        std::cout << "Failed to retrieve or create sheet" << std::endl;
        return;
    }

    for (int indexRow = 0; indexRow < rowCount; indexRow++) {
        writeAcceleration(sheet, indexRow, rowCount);
//        writeGyroscope(sheet, indexRow, rowCount);
//        writeMagnitude(sheet, indexRow, rowCount);
    }

}

void Excel_exporter::writeAcceleration(libxl::Sheet *sheet, int indexRow, int rowCount) {

    std::cout << "ICI" << std::endl;
    sheet->writeNum(indexRow + 1, 2, indexRow + 1);
    std::cout << "LA" << std::endl;
//    sheet->writeNum(indexRow + 1, 3, _accelerations[rowCount][0]);  // TROIS PREMIERES COLONNES
//    sheet->writeNum(indexRow + 1, 4, _accelerations[rowCount][1]);
//    sheet->writeNum(indexRow + 1, 5, _accelerations[rowCount][2]);
}

void Excel_exporter::writeGyroscope(libxl::Sheet *sheet, int indexRow, int rowCount) {

    sheet->writeNum(indexRow + 1, 8, _gyroscopes[rowCount][0]);
    sheet->writeNum(indexRow + 1, 9, _gyroscopes[rowCount][1]);
    sheet->writeNum(indexRow + 1, 10, _gyroscopes[rowCount][2]);
}

void Excel_exporter::writeMagnitude(libxl::Sheet *sheet, int indexRow, int rowCount) {

    sheet->writeNum(indexRow + 1, 11, _magnitudes[rowCount][0]);
    sheet->writeNum(indexRow + 1, 12, _magnitudes[rowCount][1]);
    sheet->writeNum(indexRow + 1, 13, _magnitudes[rowCount][2]);
}

