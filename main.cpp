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
void deleteItem(string& item_id);

int main(){
    welcomeScreen();
    while(true){
       string command;
       string disease;
        cout << "RCA Inventory>";

        getline(cin, command);
        if(toLowercase(command) == "help"){
            helpMenu();
        }
        else if(toLowercase(command) == "exit"){
            cout << "Exiting the system..." << endl;
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
        else if(toLowercase(command.substr(0, 10)) == "itemdelete"){
            string payload = command.substr(11);
            cout<<"payload: "<<payload<<endl;
            //extract item_id from payload
            stringstream ss(payload);
            string item_id;
            ss >> item_id;
            //delete item from file
            deleteItem(item_id);
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
    cout << "                                        *    Welcome to RCA Inventory system         *" << endl;
    cout << "                                       ##########################################################" << endl;
    cout << "                                       *         End of year3 DSA practical national exam 2023       *"<<endl;
    cout << "                                       *         Names: Mudahemuka MANZI                             *"<<endl;
    cout << "                                       *    Type 'help' to see the list of commands *" << endl;
  
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
    cout <<"           exit"<< endl;
    cout <<"           help"<< endl;

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
    string fileName = "rca_store.csv";
    //open file
    ofstream file(fileName, ios::app);
     if(!file){
        cout << "Error in opening file!" << endl;
        return;
     }
     //check if item_id is valid digit
    for(int i = 0; i < item_id.length(); i++){
        if(!isdigit(item_id[i])){
            cout << "Invalid item id!" << endl;
            return;
        }
    
    }
    //check if quantity is valid number
    if(quantity < 0){
        cout << "Invalid quantity!" << endl;
        return;
    }
    //check if item_name is valid string
    for(int i = 0; i < item_name.length(); i++){
        if(!isalpha(item_name[i]) && item_name[i] != ' '){
            cout << "Invalid item name!" << endl;
            return;
        }
    }

    //check if item_registration_date is valid date
    for(int i = 0; i < item_registration_date.length(); i++){
        if(!isdigit(item_registration_date[i]) && item_registration_date[i] != '-'){
            cout << "Invalid date!" << endl;
            return;
        }
    }

    //check if item_registration date is not in the future
    time_t now = time(0);
    tm* ltm = localtime(&now);
    int currentYear = 1900 + ltm->tm_year;
    int currentMonth = 1 + ltm->tm_mon;
    int currentDay = ltm->tm_mday;
    stringstream ss(item_registration_date);
    string year;
    string month;
    string day;
    getline(ss, year, '-');
    getline(ss, month, '-');
    getline(ss, day, '-');
    if(stoi(year) > currentYear){
        cout << "Invalid date!" << endl;
        return;
    }
    if(stoi(year) == currentYear && stoi(month) > currentMonth){
        cout << "Invalid date!" << endl;
        return;
    }
    if(stoi(year) == currentYear && stoi(month) == currentMonth && stoi(day) > currentDay){
        cout << "Invalid date!" << endl;
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
    ifstream file("rca_store.csv", ios::app);
    if(!file){
        cout << "Error in opening file!" << endl;
        return;
    }
    string line;
    cout<<endl;
    cout << "    ****************************************************************************************" << endl;
    cout << "    *            Available Items in RCA Inventory (Total: " << nberOfItemsIn_inventory() << ")  *" << endl;
    cout << "    ****************************************************************************************" << endl;
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
    ifstream file("rca_store.csv", ios::app);
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


//delete item given its id
void deleteItem(string& item_id){
    //delete item from csv file given item id
    string fileName = "rca_store.csv";
    //open file
    ofstream file(fileName, ios::app);
     if(!file){
        cout << "Error in opening file!" << endl;
        return;
     }
        //check if item_id is valid digit
    for(int i = 0; i < item_id.length(); i++){  
        if(!isdigit(item_id[i])){
            cout << "Invalid item id!" << endl;
            return;
        }
    }

    //check if item_id exists in items.csv file
    if(!isItemExists(fileName, item_id,"")){
        cout<<"Item not found!"<<endl;
        file.close();
        return;
    };
    
    //if found delete whole line that contains location in csv file
    ifstream file1(fileName);
    ofstream temp;
    temp.open("temp.csv");
    string line;
    while(getline(file1, line, '\n')){
        //extract all fields from line
        stringstream ss(line);
        string id;
        string name;
        string quantity;
        string item_registration_date;
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, quantity, ',');
        getline(ss, item_registration_date, ',');
        if(id != item_id){
            temp << line << endl;
        }

    }
    temp.close();
    file1.close();
    remove("rca_store.csv");
    rename("temp.csv", "rca_store.csv");
    remove("temp.csv");
    cout << "Item deleted successfully!" << endl;
}