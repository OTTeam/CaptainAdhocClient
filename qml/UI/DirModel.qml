// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1


ListModel{
    id: dirModelTest

    ListElement{
        folderPath: "C/toto/tata"
    }
    ListElement{
        folderPath: "U/Plop/Plip/ReallyLongFolderPath/ReallyReallyLong"
    }
    ListElement{
        folderPath: "D/ADirectory"
    }
}
