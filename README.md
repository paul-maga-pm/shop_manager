# shop_manager

# Description
A small C console application for managing and querring the stock of a generic shop. It can perform CRUD operations as: adding, deleting, updating a product, sorting and
querring the current stock.

One product of the stock has: name, description, manufacturer, type, price and quantity.

The application works only with in-memory data, so the stock will appear as empty after closing and then openning again the application.

# UI Commands
![Screenshot 2022-03-30 162248](https://user-images.githubusercontent.com/93886764/160844680-6cdb1e46-ce34-457e-8e6e-3f2d699a9f85.png)

print menu - displays the application main menu;

see stock - displays all products from current stock;

add item    - adds a new item to the stock; if the item already exists in stock (an item with same type, model, manufacturer and type already exists) the quantity will be
increased by the given quantity;
            - before adding, the item's properties are validated (non-empty model, manufacturerm type) and positive price and quantity;
            - if the user enters an item that doesn't respect the above validation rules, for each invalid field will be displayed a proper message;
         
update item - updates the price and quantity, which must be positive values
            - the user will be informed if the values are invalid or the item which he wants to update doesn't exist in stock

remove item - removes item from the stock
            - the user will be informed if the item doesn't exist
            
filter      - querries the current stock based on the current filter
            - for price filter: displays all items with price smaller than a given value
            - for quantity filter: displays all items with quantity smaller than a given value
            - for manufacturer filter: displays items of a manufacturer
            - filter command will be run based on the displayed filter criteria in the menu; the criteria can be changed using command set filter

sort        - displays all items of stock sorted by the set field (price or quantity) in the menu and the set order (ascending or descending)


set sort/set filter/set order  - commands for changing the criteria for sorting items, filtering items and the order of sorting the items


# Implementation Details
The application uses dynamic allocation. The memory allocations and deallocations are managed in a OOP manner using constructors, destructors and copy constructors.
All types that use dynamic allocation are encapsulated using a typedef definition in the header file and providing actual implementation of the struct in the source 
file, so the client will have access to the atributes of the object only using the provided interface in the header file.

# Architecture
The application is divided in: Presentation layer (console interface), Business layer (shop service) and Domain layer (the product struct)

# Used data structures:
The application uses a list DS with an interface simillar to the one in the C++ standard library. The list is implemented as double-linked list. Also, the 
implementation gives the client the option to add copy constructor and destructor for the contained objects, so deallocation and copy will be realised with
those functions if are provided

For managing the available commands in the console interface, a map DS is used with linked-list implementation. The size of the map is static and the keys are
strings.

# Usage
For easier compilation of the project I recommend using Visual studio 2019 or 2022. The solution and project files are uploaded in the repository.

  
