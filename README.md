# Pathfinder_QML
Scalable Pathfinder V2 using QML &amp; backend C++ logic

Enviornment update: This is a Qt pro file based project. Created with Qt 6.5, uses C++ 17.

This is a Version 2 implementation of the original Pathfinder with following changes (please see original version Readme for details related to assumptions & observations)
1. During the data driven test drive, few issues with the algorithm were observed. Those issues are fixed.  Simple data set used for narrowing down issues & later using for the fix with additional test cases is included here (files in Testifies folder)
2. In this version UI is moved to QML & separeted well from backend C++ logic. UI also shows meaningful data when iterating with the app. It can be improved to show legend indicating colors used by grid cells to show status. PS : this grid gets populated first by design described below
Visual details for each cell:
- cell shows location index in format (X position : Y position) 
- Value -1 (grey) indicates walkable cell & 3 (Red) indicates
- Upon successful path search - green shows starting location, blue shows destination & yellow cells show travel path on walkable cells
3. C++ central processing behind the QML view is implemented in PathModel (extended from Qt’s QAbstractTableModel). It uses abstract interface DataProvider for hiding behind the scene data loading, parsing & related details when it comes to acquiring the data. Once data is populated - uses strategy pattern abstraction interface for using the algorithm to process on the data. Once data processing is done, it triggers event for QML view to update the view so new data will be shown immediately.
4. DataProvider abstraction currently has single implementation for JsonDataProvider - that too is tightly coupled to Riskylab json format, this can be updated to support many formats. If new type of data source is required, even that can be safely added behind the DataProvider. Reducing the complexity in managing the dependencies when extending the functionality.
4. SearchStrategy uses strategy pattern for using graph data algorithm. This can be extended as required without too much dependency on touching other aspects of the application.
5. All in all - this is a scalable implementation in step towards putting it in a product. PS: Not finished in any ways, but each side (View, Data acquisition, processing) can be extended in each direction as required.

Content:
- TestFiles folder contents simple test cases to be used for testing. Also includes 2 files from Riskylab.
- screenshots - single screenshot showing multiple instances of app running different test cases
