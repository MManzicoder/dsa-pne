#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cctype>
#include <iomanip>

using namespace std;

void welcomeScreen();
void helpMenu();
void returnHelpMenuContent();
void addItem(string& item_id, string& item_name, int quantity, string& item_registration_date);
bool isItemExists(const string& filename, const string& item_id, const string& item_name);
void listItems();
int nberOfItemsIn_inventory();
string toLowercase(const std::string& str);

int main(){
    welcomeScreen();
    while(true){
       string command;
       string disease;
        cout << ">";

        getline(cin, command);
        if(toLowercase(command) == "help"){
            helpMenu();
        }
        else if(toLowercase(command) == "exit"){
            cout << "Exiting the program..." << endl;
            break;
        }
        else if(toLowercase(command.substr(0, 7)) == "itemadd"){
            string payload = command.substr(8);

            //extract item_id, item_name, quantity, item_registration_date from payload

            stringstream ss(payload);
            string item_id;
            string item_name;
            int quantity;
            string item_registration_date;

            ss >> item_id >> item_name >> quantity >> item_registration_date;

            //add item to file 

            addItem(item_id, item_name, quantity, item_registration_date);
        }
        else if(toLowercase(command) =="itemslist"){
           listItems();
        }
        else{
            cout << "Invalid command!" << endl;
        }

    }
    return 0;
}

//WELCOME SCREEN
void welcomeScreen(){
    cout << "                                       ##########################################################" << endl;
    cout << "                                        *    Welcome to Rwanda Coding Academy Inventory system         *" << endl;
  
}

//HELP MENU

void helpMenu(){
    cout << "------------------------------------------------------" << endl;
    cout << " *           Commands syntaxes                          *" << endl;
    cout << "--------------------------------------------------------" << endl;
  returnHelpMenuContent();
}

void returnHelpMenuContent(){
    cout <<"           itemadd <item_id> <item_name> <quantity> <registration_date>"<< endl;
    cout <<"           itemslist"<< endl;

}

bool isItemExists(const string& filename, const string& item_id, const string& item_name)
{
    //check if the item exists in the file with the same id and item name and return true if it exists
    ifstream file(filename, ios::app);
    if(!file){
        cout << "Error in opening file!" << endl;
        return false;
    }
    string line;
    while(getline(file, line, '\n')){
        stringstream ss(line);
        string id;
        string name;
        string quantity;
        string item_registration_date;
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, quantity, ',');
        getline(ss, item_registration_date, ',');
        if(id == item_id){
            return true;
        }
        if(id == item_id && toLowercase(name) == toLowercase(item_name)){
            return true;
        }
    }
    file.close();
    return false;
}
 
//add item
void addItem(string& item_id, string& item_name, int quantity, string& item_registration_date){
    string fileName = "rca_inventory.csv";
    //open file
    ofstream file(fileName, ios::app);
     if(!file){
        cout << "Error in opening file!" << endl;
        return;
     }

    //check if item_id exists in items.csv file

    if(isItemExists(fileName, item_id, item_name)){
        cout<<"Item already exists!"<<endl;
        return;
    };
    //if not, add item in csv file
    file << item_id << "," << item_name << "," << quantity << "," << item_registration_date << endl;
    cout << "Item "<<item_name<<" is successfully added!" << endl;
    file.close();
}



//list items
void listItems(){
    ifstream file("rca_inventory.csv", ios::app);
    if(!file){
        cout << "Error in opening file!" << endl;
        return;
    }
    string line;
    cout<<endl;
   
  //list items in alphabetical order by item name and display all  fields in the file;
    vector<string> items;
    while(getline(file, line, '\n')){
        stringstream ss(line);
        string item_id;
        string item_name;
        string quantity;
        string item_registration_date;
        getline(ss, item_id, ',');
        getline(ss, item_name, ',');
        getline(ss, quantity, ',');
        getline(ss, item_registration_date, ',');
        items.push_back(item_name);
    }
    sort(items.begin(), items.end());
    for(int i = 0; i < items.size(); i++){
        file.clear();
        file.seekg(0, ios::beg);
        while(getline(file, line, '\n')){
            stringstream ss(line);
            string item_id;
            string item_name;
            string quantity;
            string item_registration_date;
            getline(ss, item_id, ',');
            getline(ss, item_name, ',');
            getline(ss, quantity, ',');
            getline(ss, item_registration_date, ',');
            if(items[i] == item_name){
                cout << left
                     << "    Item ID:" << std::setw(2) << item_id
                     << "      Item Name:" << std::setw(15) << item_name
                     << "      Quantity :" << std::setw(4) << quantity
                     << "      Reg Date :" << item_registration_date << std::endl;
                cout<<endl;
            }
        }
    }

    file.close();
}


//check how many items are in the inventory
int nberOfItemsIn_inventory(){
    ifstream file("rca_inventory.csv", ios::app);
    if(!file){
        cout << "Error in opening file!" << endl;
        return 0;
    }
    string line;
    int nberOfItems = 0;
    while(getline(file, line, '\n')){
        stringstream ss(line);
        string item_id;
        string item_name;
        string quantity;
        string item_registration_date;
        getline(ss, item_id, ',');
        getline(ss, item_name, ',');
        getline(ss, quantity, ',');
        getline(ss, item_registration_date, ',');
        nberOfItems++;
    }
    file.close();
    return nberOfItems;
}



string toLowercase(const std::string& str) {
    std::string lowercaseStr = str;
    std::transform(lowercaseStr.begin(), lowercaseStr.end(), lowercaseStr.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return lowercaseStr;
}
