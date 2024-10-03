//Latest project

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cstring>
#include<iomanip>
#include<cctype>
#include<regex>
#include<set>
#include<ctime>
#include<chrono>     // for chrono::seconds
#include<thread>     // for this_thread::sleep_for
//#include<hpdf.h>
#include<windows.h> // For Windows API functions
#include<algorithm>
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

using namespace std;
using namespace chrono;


struct date
{
	string startDate;
	string endDate;
};

struct car
{
	string plateNumber;
	string brand;
	int year;
	string model;
	int pricePerday;
	string color;
	date d;

	bool matches(const car& other) const
	{
		return plateNumber == other.plateNumber && brand == other.brand && year == other.year &&
			model == other.model && pricePerday == other.pricePerday && color == other.color;
	}
};

struct client
{
	int userID;
	string firstName;
	string lastName;
	string password;
	string phone;
	string email;
	int nbReservation;
	car* c;
};

struct RentalCar 
{
	string userID;
	string plateNumber;
	string startDate;
	string endDate;
	int pricePerDay;
};

struct RentalInfo
{
	int userID;
	string plateNumber;
	string startDate;
	string endDate;
	int price;
};

/*void generatePDF(const string& filename)
{
	HPDF_Doc pdf = HPDF_New(NULL, NULL);

	if (!pdf)
	{
		cerr << "Error: Unable to create PDF object\n";
		return;
	}

	if (HPDF_OK != HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL))
	{
		cerr << "Error: Unable to set compression mode\n";
		HPDF_Free(pdf);
		return;
	}

	if (HPDF_OK != HPDF_SetPageLayout(pdf, HPDF_PAGE_LAYOUT_SINGLE))
	{
		cerr << "Error: Unable to set page layout\n";
		HPDF_Free(pdf);
		return;
	}

	HPDF_Page page = HPDF_AddPage(pdf);
	if (!page)
	{
		cerr << "Error: Unable to add page\n";
		HPDF_Free(pdf);
		return;
	}

	HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

	HPDF_Page_BeginText(page);
	HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
	HPDF_Page_SetFontAndSize(page, font, 12);

	HPDF_Page_MoveTextPos(page, 50, HPDF_Page_GetHeight(page) - 50);

	// Write rental information
	HPDF_Page_ShowText(page, "Rental Information:\n");
	HPDF_Page_MoveTextPos(page, 0, -12); // Move to the next line
	HPDF_Page_ShowText(page, "Plate Number      UserID      Start Date   End Date   Rental Price\n");
	HPDF_Page_MoveTextPos(page, 0, -12); // Move to the next line

	ifstream rentalFile("sorted_rental_cars.txt");
	string line;
	while (getline(rentalFile, line))
	{
		istringstream iss(line);
		RentalInfo rental;
		if (!(iss >> rental.plateNumber >> rental.userID >> rental.startDate >> rental.endDate >> rental.price))
		{
			cerr << "Error: Invalid format in rental file\n";
			continue;
		}
		ostringstream rentalInfo;
		rentalInfo << rental.plateNumber << "      " << rental.userID << "      " << "       " << rental.startDate << "       " << rental.endDate << "       " << rental.price << "\n";
		HPDF_Page_ShowText(page, rentalInfo.str().c_str());
		HPDF_Page_MoveTextPos(page, 0, -12); // Move to the next line
	}

	rentalFile.close();

	HPDF_Page_MoveTextPos(page, 0, -12); // Move to the next line

	// Write car information
	HPDF_Page_ShowText(page, "Car Information:\n");
	HPDF_Page_MoveTextPos(page, 0, -12); // Move to the next line
	HPDF_Page_ShowText(page, "Plate Number   Brand   Year   Model   Price per Day   Color\n");
	HPDF_Page_MoveTextPos(page, 0, -12); // Move to the next line

	ifstream carFile("Car.txt");
	while (getline(carFile, line))
	{
		istringstream iss(line);
		car car;
		if (!(iss >> car.plateNumber >> car.brand >> car.year >> car.model >> car.pricePerday >> car.color))
		{
			cerr << "Error: Invalid format in car file\n";
			continue;
		}
		ostringstream carInfo;
		carInfo << car.plateNumber << "       " << car.brand << "     " << car.year << "      " << car.model << "       " << car.pricePerday << "       " << car.color << "\n";
		HPDF_Page_ShowText(page, carInfo.str().c_str());
		HPDF_Page_MoveTextPos(page, 0, -12); // Move to the next line
	}

	carFile.close();

	HPDF_Page_EndText(page);

	if (HPDF_OK != HPDF_SaveToFile(pdf, filename.c_str()))
	{
		cerr << "Error: Unable to save PDF to file\n";
	}

	HPDF_Free(pdf);
}*/

int loggedInUserID; // Global variable to store the logged-in userID

// Function to read rental cars from rental_cars.txt
void readRentalCars(const string& filename, RentalCar* rentalCars, int size) 
{
	ifstream file(filename);
	string line;
	int i = 0;
	while (getline(file, line) && i < size) {
		istringstream iss(line);
		iss >> rentalCars[i].userID >> rentalCars[i].plateNumber >> rentalCars[i].startDate >> rentalCars[i].endDate;
		i++;
	}
	file.close();
}

// Function to read car details from Car.txt
void readCarDetails(const string& filename, car* carDetails, int size) 
{
	ifstream file(filename);
	string line;
	int i = 0;
	while (getline(file, line) && i < size) {
		istringstream iss(line);
		iss >> carDetails[i].plateNumber >> carDetails[i].brand >> carDetails[i].year >> carDetails[i].model >> carDetails[i].pricePerday >> carDetails[i].color;
		i++;
	}
	file.close();
}

bool compareByPrice(const RentalCar& a, const RentalCar& b) 
{
	return a.pricePerDay < b.pricePerDay;
}

void setTextColor(int color)
{
	// Uses Windows API to change the text color
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

bool isIdAvailable(const string& id, const string& filename) 
{

	ifstream file(filename);

	string line;
	while (getline(file, line))
	{
		istringstream iss(line);
		client user1;
		if (iss >> user1.userID)
		{
			if (user1.userID == stoi(id))
			{
				file.close();
				return true; // ID found
			}
		}
	}

	file.close();
	return false; // ID not found
}

pair<bool, string> isPassAvailable1(const string& id, const string& pass, const string& filename)
{
	ifstream file(filename);

	string line;
	while (getline(file, line))
	{
		istringstream iss(line);
		client user1;
		if (iss >> user1.userID >> user1.firstName >> user1.lastName >> user1.password)
		{

			if (user1.userID == stoi(id) && user1.password == pass)
			{
				file.close();
				return make_pair(true, user1.firstName); // Password found
			}
		}
	}

	file.close();
	return make_pair(false, ""); // Password not found
}

bool isAdminPassAvailable(const string& id, const string& pass, const string& filename)
{
	ifstream file(filename);

	string line;
	while (getline(file, line))
	{
		istringstream iss(line);
		client user1;
		if (iss >> user1.userID >> user1.firstName >> user1.lastName >> user1.password)
		{

			if (user1.userID == stoi(id) && user1.password == pass)
			{
				file.close();
				return true; // Password found
			}
		}
	}

	file.close();
	return false; // Password not found
}

bool Checkmail(string& x)
{
	const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
	return regex_match(x, pattern);
}

bool checkPhone(string x) {
	if (x.length() == 9 && x.at(2) == '-')
		return true;
	return false;
}

bool validuserID(string& id) {
	// Check if the ID has exactly 4 digits
	return (id.length() == 4 && all_of(id.begin(), id.end(), ::isdigit));
}

bool validadminID(string& id) {
	// Check if the ID has exactly 4 digits
	return (id.length() == 6 && all_of(id.begin(), id.end(), ::isdigit));
}

int strhashing(string a) 
{
	hash<string>hash_obj;
	return hash_obj(a);
}

bool Checkname(string x)
{
	for (int i = 0; i < x.length(); i++) 
	{
		if (x.at(i) == '1' || x.at(i) == '2' || x.at(i) == '3' || x.at(i) == '4' || x.at(i) == '5' || x.at(i) == '6' || x.at(i) == '7' || x.at(i) == '8' || x.at(i) == '9' || x.at(i) == '0' || x.at(i) == ' ' || x.at(i) == '!' || x.at(i) == '@' || x.at(i) == '#' || x.at(i) == '$' || x.at(i) == '%' || x.at(i) == '^' || x.at(i) == '&' || x.at(i) == '*' || x.at(i) == '(' || x.at(i) == ')' || x.at(i) == '.' || x.at(i) == '!' || x.at(i) == '?')
			return false;
	}
	return true;
}

bool CheckPassword(string& x)
{
	int countupp = 0;
	int countdown = 0;
	int countspecialchar = 0;
	int countdigit = 0;
	if (x.length() < 8)
		return false;
	for (int i = 0; i < x.length(); i++)
	{

		if (x.at(i) >= 97 && x.at(i) <= 122)
			countdown++;
		if (x.at(i) >= 65 && x.at(i) <= 90)
			countupp++;
		if (isdigit(x.at(i)))
			countdigit++;
		if ((x.at(i) >= '!' && x.at(i) <= '/') ||
			(x.at(i) >= ':' && x.at(i) <= '@') ||
			(x.at(i) >= '[' && x.at(i) <= '`') ||
			(x.at(i) >= '{' && x.at(i) <= '~'))
			countspecialchar++;

	}
	if (countdown > 0 && countupp > 0 && countdigit > 0 && countspecialchar > 0)
		return true;
	return false;
}

string generateRandomID()
{
	string ID = "", d;
	srand(time(0));
	for (int i = 0; i < 4; i++)
	{
		d = to_string(rand() % 10);

		while (ID.find(d) != string::npos)
		{
			d = to_string(rand() % 10);
		}
		ID += d;
	}
	return ID;
}

// Add New Car

bool isValidDate(const string& date) 
{
	// Regular expression to match the date format
	regex pattern("^[0-9]{2}-[0-9]{2}-[0-9]{4}$");
	if (!regex_match(date, pattern)) {
		return false; // Date format is not correct
	}

	// Extract year, month, day from the input string
	int year, month, day;
	sscanf(date.c_str(), "%d-%d-%d", &day, &month, &year);

	// Use chrono to build a date
	system_clock::time_point now = system_clock::now();
	time_t tt = system_clock::to_time_t(now);
	tm local_tm = *localtime(&tt);

	// Current date components
	int currentYear = local_tm.tm_year + 1900;
	int currentMonth = local_tm.tm_mon + 1;
	int currentDay = local_tm.tm_mday;

	// Compare with current date
	if (year < currentYear || 
		(year == currentYear && month < currentMonth) || 
		(year == currentYear && month == currentMonth && day < currentDay)) 
	{
		return false; // Date is in the past
	}

	return true; // Date is valid and not in the past
}

tm parseDate(const string& date) 
{
	tm tm{};
	stringstream ss(date);
	ss >> get_time(&tm, "%d-%m-%Y");
	return tm;
}

// Helper function to compare two tm structures
bool isBefore(const tm& startDate, const tm& endDate) 
{
	if (startDate.tm_year < endDate.tm_year) {
		return true;
	}
	else if (startDate.tm_year == endDate.tm_year) {
		if (startDate.tm_mon < endDate.tm_mon) {
			return true;
		}
		else if (startDate.tm_mon == endDate.tm_mon) {
			if (startDate.tm_mday < endDate.tm_mday) {
				return true;
			}
		}
	}
	return false;
}

bool validatePlateNumber(const string& plateNumber)
{
	// Regular expression pattern to match 6 digits followed by a capital letter
	regex pattern("^\\d{6}[A-Z]$");
	return regex_match(plateNumber, pattern);
}

bool checkCarBrand(string x)
{
	for (int i = 0; i < x.length(); i++) {
		if (x.at(i) == '1' || x.at(i) == '2' || x.at(i) == '3' || x.at(i) == '4' || x.at(i) == '5' || x.at(i) == '6' || x.at(i) == '7' || x.at(i) == '8' || x.at(i) == '9' || x.at(i) == '0' || x.at(i) == ' ' || x.at(i) == '!' || x.at(i) == '@' || x.at(i) == '#' || x.at(i) == '$' || x.at(i) == '%' || x.at(i) == '^' || x.at(i) == '&' || x.at(i) == '*' || x.at(i) == '(' || x.at(i) == ')' || x.at(i) == '.' || x.at(i) == '!' || x.at(i) == '?')
			return false;
	}
	return true;
}

bool validateCarYear(int year)
{
	int currentYear = 2024;
	return (year >= 1900 && year <= currentYear);
}

bool validateCarModel(const string& model)
{
	const set<string> validModels = { "Corolla", "Camry", "RAV4", "Highlander", "Tacoma", "Tundra", "Land Cruiser", "Civic", "Accord", "CR-V", "Pilot", "Mustang", "Expedition", "EcoSport", "Tahoe", "Suburban", "Camaro", "Golf", "Tiguan", "Golf GTI", "X3", "X5", "X1", "X7", "X6", "C-Class", "E-Class", "GLC", "GLE", "A-Class", "S-Class", "GLA", "CLA", "GLB", "GLS", "A4", "A6", "Q5", "Q7", "A3", "A5", "Q3", "A8", "Q8", "RS 3", "Altima", "Rogue",  "Pathfinder", "Maxima", "Elantra", "Accent", "Veloster" };

	return validModels.find(model) != validModels.end();
}

bool validatePricePerDay(int price)
{
	string priceStr = to_string(price);
	regex pattern("^[1-9][0-9]*$");
	return regex_match(priceStr, pattern);
}

bool validateCarColor(const string& color)
{
	// Convert the color string to lowercase for case-insensitive comparison
	string lowercaseColor = color;
	transform(lowercaseColor.begin(), lowercaseColor.end(), lowercaseColor.begin(),
		[](unsigned char c) { return tolower(c); });

	string colors[] = { "black", "white", "red", "blue", "green", "silver", "gray", "yellow", "orange", "brown" };

	// Check if the input color matches any of the common color names
	return find(begin(colors), end(colors), lowercaseColor) != end(colors);
}

pair<bool, int> isCarAvailable(const string& plateNumber, const string& brand, int year, const string& model, int pricePerday, const string& color, const string& filename)
{
	ifstream file(filename);
	int index = 0;
	string line;
	while (getline(file, line))
	{

		istringstream iss(line);
		car cr;
		if (iss >> cr.plateNumber >> cr.brand >> cr.year >> cr.model >> cr.pricePerday >> cr.color)
		{
			index++;
			if ((cr.plateNumber == plateNumber) && (cr.brand == brand) && (cr.year == year) && (cr.model == model) && (cr.pricePerday == pricePerday) && (cr.color == color))
			{
				file.close();
				return { true, index }; // Car found
			}
		}

	}

	file.close();
	return { false, -1 }; // Car not found
}

bool isCarAvailableforAdding(const string& plateNumber, const string& filename)
{
	ifstream file(filename);
	int index = 0;
	string line;
	while (getline(file, line))
	{
		istringstream iss(line);
		car cr;
		if (iss >> cr.plateNumber)
		{

			if (cr.plateNumber == plateNumber)
			{
				file.close();
				return true; // Car found
			}
		}
		index++;
	}

	file.close();
	return false; // Car not found
}

pair<bool, int> isCarAvailableforremoval(const string& plateNumber, const string& brand, int year, const string& model, int pricePerday, const string& color, const string& filename)
{
	ifstream file(filename);
	int index = 0;
	string line;
	while (getline(file, line))
	{

		istringstream iss(line);
		car cr;
		if (iss >> cr.plateNumber >> cr.brand >> cr.year >> cr.model >> cr.pricePerday >> cr.color)
		{
			index++;
			if ((cr.plateNumber == plateNumber) && (cr.brand == brand) && (cr.year == year) && (cr.model == model) && (cr.pricePerday == pricePerday) && (cr.color == color))
			{
				file.close();
				return { true, index }; // Car found
			}
		}

	}

	file.close();
	return { false, -1 }; // Car not found
}

pair<bool, int> isCarAvailableforremoval1(const string& plateNumber, const string& filename)
{
	ifstream file(filename);
	int index = 0;
	string line;
	while (getline(file, line))
	{

		istringstream iss(line);
		car cr;
		if (iss >> cr.plateNumber)
		{
			index++;
			if (cr.plateNumber == plateNumber)
			{
				file.close();
				return { true, index }; // Car found
			}
		}

	}

	file.close();
	return { false, -1 }; // Car not found
}

bool isCarRented(const string& id, const string& plateNumber, const string& startDate, const string& endDate, const string& filename)
{
	ifstream file(filename);

	string line;
	while (getline(file, line))
	{
		istringstream iss(line);
		client user1;
		car car1;
		date d1;
		if (iss >> user1.userID >> car1.plateNumber >> d1.startDate >> d1.endDate)
		{

			if (user1.userID == stoi(id) && car1.plateNumber == plateNumber && d1.startDate == startDate && d1.endDate == endDate)
			{
				file.close();
				return true; // Rent found
			}
		}
	}

	file.close();
	return false; // Rent not found
}

bool isCarRented1(const string& id, const string& plateNumber, const string& startDate, const string& endDate, const string& filename)
{
	ifstream file(filename);

	string line;
	while (getline(file, line))
	{
		istringstream iss(line);
		client user1;
		car car1;
		date d1;
		if (iss >> user1.userID >> car1.plateNumber )
		{

			if (car1.plateNumber == plateNumber )
			{
				file.close();
				return true; // Rent found
			}
		}
	}

	file.close();
	return false; // Rent not found
}

void displayRentsOfClient()
{
	int lineNumber = 1;
	string filename = "rental cars.txt";
	ifstream file(filename);

	string line;
	while (getline(file, line))
	{
		istringstream iss(line);
		date d;
		client user1;
		car cr;
		if (iss >> user1.userID >> cr.plateNumber >> d.startDate >> d.endDate)
		{
			if (user1.userID == loggedInUserID) {
				setTextColor(FOREGROUND_INTENSITY); // Set text intensity
				cout << endl << endl << lineNumber++ << ". ";

				setTextColor(FOREGROUND_GREEN); // Set text color to green
				cout << "UserID: ";
				setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset text color
				cout << user1.userID << "\t";

				setTextColor(FOREGROUND_GREEN); // Set text color to green
				cout << "Plate Number: ";
				setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset text color
				cout << cr.plateNumber << "\t";

				setTextColor(FOREGROUND_GREEN); // Set text color to green
				cout << "Start date: ";
				setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset text color
				cout << d.startDate << "\t";

				setTextColor(FOREGROUND_GREEN); // Set text color to green
				cout << "End date: ";
				setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset text color
				cout << d.endDate << "\t";
			}
			
		}
	}
	cout << endl << endl;
	file.close();
}

void displayCars()
{
	int lineNumber = 1;
	string filename = "Cars.txt";
	ifstream file(filename);

	string line;
	while (getline(file, line))
	{
		istringstream iss(line);
		car cr;
		if (iss >> cr.plateNumber >> cr.brand >> cr.year >> cr.model >> cr.pricePerday >> cr.color)
		{

			setTextColor(FOREGROUND_INTENSITY); // Set text intensity
			cout << endl << endl << lineNumber++ << ". ";

			setTextColor(FOREGROUND_GREEN); // Set text color to green
			cout << "Plate Number: ";
			setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset text color
			cout << cr.plateNumber << "\t";

			setTextColor(FOREGROUND_GREEN); // Set text color to green
			cout << "Brand: ";
			setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset text color
			cout << cr.brand << "\t";

			setTextColor(FOREGROUND_GREEN); // Set text color to green
			cout << "Year: ";
			setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset text color
			cout << cr.year << "\t";

			setTextColor(FOREGROUND_GREEN); // Set text color to green
			cout << "Model: ";
			setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset text color
			cout << cr.model << "\t";

			setTextColor(FOREGROUND_GREEN); // Set text color to green
			cout << "Price per day ($): ";
			setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset text color
			cout << cr.pricePerday << "\t";

			setTextColor(FOREGROUND_GREEN); // Set text color to green
			cout << "Color: ";
			setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset text color
			cout << cr.color << endl;

		}
	}

	file.close();
}

void getCarDatatoremove(car* cr)
{
	ofstream outfile;
	outfile.open("Car.txt", ios::out | ios::app);
	ifstream infile;
	infile.open("Car.txt", ios::in);

	car userInput;

	int i = 1;

	do {
		cout << '\t' << '\t' << '\t' << "Enter the car's plate number: ";
		cin >> userInput.plateNumber;
	} while (!validatePlateNumber(userInput.plateNumber));

	string filenamee = "Car.txt";
	auto result = isCarAvailableforremoval1(userInput.plateNumber, filenamee);

	if (result.first)
	{
		ifstream inFile("Car.txt"); // Input file
		ofstream outFile("temp.txt"); // Temporary output file

		string line;
		bool plateFound = false;

		while (getline(inFile, line))
		{
			// Check if the line contains the plate number
			if (line.find(userInput.plateNumber) == string::npos) {
				outFile << line << endl; // Write line to output file if plate number is not found
			}
			else
			{
				plateFound = true;
			}
		}

		inFile.close();
		outFile.close();

		cout << "\t\t\tCar removed successfully." << endl;
	}
	else
	{
		cout << "\t\t\tCar not found!" << endl;
	}
}

void getCarDataToModify(car* cr) 
{

	ofstream outfile;
	outfile.open("Car.txt", ios::out | ios::app);
	ifstream infile;
	infile.open("Car.txt", ios::in);

	car userInput;

	cout << '\t' << '\t' << '\t' << "Enter the car's plate number: ";
	cin >> userInput.plateNumber;

	if (!validatePlateNumber(userInput.plateNumber)) {
		cout << "Invalid plate number." << endl;
		return;
	}

	string filename = "Car.txt";
	auto result = isCarAvailableforremoval1(userInput.plateNumber, filename);


	if (result.first) 
	{
		ifstream inFile("Car.txt"); // Input file
		ofstream outFile("temp.txt"); // Temporary output file

		string line;
		bool plateFound = false;

		while (getline(inFile, line))
		{
			// Check if the line contains the plate number
			if (line.find(userInput.plateNumber) == string::npos) {
				outFile << line << endl; // Write line to output file if plate number is not found
			}
			else
			{
				plateFound = true;
			}
		}

		cout << '\t' << '\t' << '\t' << "Enter the new car details: " << endl;

		cout << '\t' << '\t' << '\t' << "Enter the car's brand: ";
		cin >> userInput.brand;

		cout << '\t' << '\t' << '\t' << "Enter the car's year: ";
		cin >> userInput.year;

		cout << '\t' << '\t' << '\t' << "Enter the car's model: ";
		cin.ignore(INT_MAX, '\n');
		getline(cin, userInput.model);

		cout << '\t' << '\t' << '\t' << "Enter the car's price per day: ";
		cin >> userInput.pricePerday;

		cout << '\t' << '\t' << '\t' << "Enter the car's color: ";
		cin >> userInput.color;

		outFile << userInput.plateNumber << '\t'
			<< userInput.brand << '\t'
			<< userInput.year << '\t'
			<< userInput.model << '\t'
			<< userInput.pricePerday << '\t'
			<< userInput.color << '\n';

		cout << "Data updated successfully." << endl;
		
	}
	else {
		cout << '\t' << '\t' << '\t' << "Car not found!" << endl;
	}
	
	outfile.close();
}

void getCarDatatorent(car* cr)
{
	ofstream outfile;
	outfile.open("Car.txt", ios::out | ios::app);
	ifstream infile;
	infile.open("Car.txt", ios::in);

	string* userID = new string;

	infile.is_open();
	string filename1 = "Clients.txt"; 

	date datetorent;
	car userInput;

	int i = 1;

	do {
		cout << '\t' << '\t' << '\t' << "Enter your userID: ";
		cin >> *userID;
	} while (!validuserID(*userID) || !isIdAvailable(*userID, filename1));

	do {
		cout << '\t' << '\t' << '\t' << "Enter the car's plate number: ";
		cin >> userInput.plateNumber;
	} while (!validatePlateNumber(userInput.plateNumber));


	string filenameofrented = "rental cars.txt";
	string filename = "Car.txt";
	auto result = isCarAvailableforremoval1(userInput.plateNumber, filename);
	if (result.first)
	{
		cout << '\t' << '\t' << '\t' << "Enter the start date of the rental (DD-MM-YYYY): ";
		cin >> datetorent.startDate;
		while (!isValidDate(datetorent.startDate))
		{
			cout << '\t' << '\t' << '\t' << "Invalid or past date. Please enter a valid future date in DD-MM-YYYY format: ";
			cin >> datetorent.startDate;
		}

		cout << '\t' << '\t' << '\t' << "Enter the end date of the rental (DD-MM-YYYY): ";
		cin >> datetorent.endDate;
		while (!isValidDate(datetorent.endDate))
		{
			cout << '\t' << '\t' << '\t' << "Invalid or past date. Please enter a valid future date in DD-MM-YYYY format: ";
			cin >> datetorent.endDate;
		}

		tm startDate1 = parseDate(datetorent.startDate);
		tm endDate1 = parseDate(datetorent.endDate);

		//Car found, you can rent
		// Check if end date is not before start date
		if (!isBefore(startDate1, endDate1))
		{
			cout << '\t' << '\t' << '\t' << "Error - The end date must be after the start date!" << endl;
			exit(0); // Exit with error
		}
		else
		{
			if (!isCarRented1(*userID, userInput.plateNumber, datetorent.startDate, datetorent.endDate, filenameofrented))
			{
				ofstream outfile;
				outfile.open("rental cars.txt", ios::out | ios::app);
				ifstream infile;
				infile.open("rental cars.txt", ios::in);

				outfile << *userID << '\t'
					<< userInput.plateNumber << '\t'
					<< datetorent.startDate << '\t'
					<< datetorent.endDate << '\n';

				outfile.close();
				cout << endl << endl << '\t' << '\t' << '\t' << "Car rented successfully!" << endl << endl;
				cout << '\t' << '\t' << '\t' << "Enjoy your ride and remember to drive safe and use the seat belt!" << endl << endl;
			}
			else
			{
				cout << endl << endl << '\t' << '\t' << '\t' << "Car already rented!" << endl << endl;
			}
		}
	}
	else
	{
		//Car not available
		cout << '\t' << '\t' << '\t' << "Sorry, car not available!" << endl << endl;
	}


}

void getCarDatatorenttomodify(car* cr)
{
	ofstream outfile1;
	outfile1.open("Car.txt", ios::out | ios::app);
	ifstream infile1;
	infile1.open("Car.txt", ios::in);

	ofstream outfile;
	outfile.open("rental cars.txt", ios::out | ios::app);
	ifstream infile;
	infile.open("rental cars.txt", ios::in);

	car userInput;

	int i = 1;

	do {
		cout << '\t' << '\t' << '\t' << "Enter the car's plate number: ";
		cin >> userInput.plateNumber;
	} while (!validatePlateNumber(userInput.plateNumber));




	ifstream inFile("rental cars.txt"); // Input file
	ofstream outFile("temp1.txt"); // Temporary output file

	string line;
	bool plateFound = false;

	while (getline(inFile, line))
	{
		// Check if the line contains the plate number
		if (line.find(userInput.plateNumber) == string::npos)
		{
			outFile << line << endl; // Write line to output file if plate number is not found
		}
		else
		{
			plateFound = true;
		}
	}

	inFile.close();
	outFile.close();

	cout << "\t\t\tRent removed successfully." << endl;


	string* userID = new string;

	infile1.is_open();
	string filename1 = "Clients.txt";

	date datetorent;


	do {
		cout << '\t' << '\t' << '\t' << "Enter your userID: ";
		cin >> *userID;
	} while (!validuserID(*userID) || !isIdAvailable(*userID, filename1));


	string filenameofrented = "rental cars.txt";
	string filename = "Car.txt";
	auto result = isCarAvailableforremoval1(userInput.plateNumber, filename);
	if (result.first)
	{
		cout << '\t' << '\t' << '\t' << "Enter the start date of the rental (DD-MM-YYYY): ";
		cin >> datetorent.startDate;
		while (!isValidDate(datetorent.startDate))
		{
			cout << '\t' << '\t' << '\t' << "Invalid or past date. Please enter a valid future date in DD-MM-YYYY format: ";
			cin >> datetorent.startDate;
		}

		cout << '\t' << '\t' << '\t' << "Enter the end date of the rental (DD-MM-YYYY): ";
		cin >> datetorent.endDate;
		while (!isValidDate(datetorent.endDate))
		{
			cout << '\t' << '\t' << '\t' << "Invalid or past date. Please enter a valid future date in DD-MM-YYYY format: ";
			cin >> datetorent.endDate;
		}

		tm startDate1 = parseDate(datetorent.startDate);
		tm endDate1 = parseDate(datetorent.endDate);

		//Car found, you can rent
		// Check if end date is not before start date
		if (!isBefore(startDate1, endDate1))
		{
			cout << '\t' << '\t' << '\t' << "Error - The end date must be after the start date!" << endl;
			exit(0); // Exit with error
		}
		else
		{
			if (!isCarRented(*userID, userInput.plateNumber, datetorent.startDate, datetorent.endDate, filenameofrented))
			{
				ofstream outfile2;
				outfile2.open("temp1.txt", ios::out | ios::app);
				ifstream infile2;
				infile2.open("temp1.txt", ios::in);

				outfile2 << *userID << '\t'
					<< userInput.plateNumber << '\t'
					<< datetorent.startDate << '\t'
					<< datetorent.endDate << '\n';

				outfile2.close();
				cout << endl << endl << '\t' << '\t' << '\t' << "Car rented successfully!" << endl << endl;
				cout << '\t' << '\t' << '\t' << "Enjoy your ride and remember to drive safe and use the seat belt!" << endl << endl;
			}
			else
			{
				cout << endl << endl << '\t' << '\t' << '\t' << "Car already rented!" << endl << endl;
			}
		}
	}
	else
	{
		//Car not available
		cout << '\t' << '\t' << '\t' << "Sorry, car not available!" << endl << endl;
	}


}

void getCarDatatocancelrent(car* cr)
{
	ofstream outfile;
	outfile.open("rental cars.txt", ios::out | ios::app);
	ifstream infile;
	infile.open("rental cars.txt", ios::in);

	car userInput;

	int i = 1;

	do {
		cout << '\t' << '\t' << '\t' << "Enter the car's plate number: ";
		cin >> userInput.plateNumber;
	} while (!validatePlateNumber(userInput.plateNumber));

		ifstream inFile("rental cars.txt"); // Input file
		ofstream outFile("temp1.txt"); // Temporary output file

		string line;
		bool plateFound = false;

		while (getline(inFile, line))
		{
			// Check if the line contains the plate number
			if (line.find(userInput.plateNumber) == string::npos) 
			{
				outFile << line << endl; // Write line to output file if plate number is not found
			}
			else
			{
				plateFound = true;
			}
		}

		inFile.close();
		outFile.close();

		cout << "\t\t\tRent removed successfully." << endl;
}

// Add New car


void Create_Profile(client* ST, int nbP)
{
	ofstream outfile;
	outfile.open("Clients.txt", ios::out | ios::app);
	ifstream infile;
	infile.open("Clients.txt", ios::in);

	int i = 1;
	do {
		cout << '\t' << '\t' << '\t' << "Enter your first name: ";
		cin >> ST[i].firstName;

	} while (!Checkname(ST[i].firstName));
	do {
		cout << '\t' << '\t' << '\t' << "Enter your last name: ";
		cin >> ST[i].lastName;

	} while (!Checkname(ST[i].lastName));
	do {
		cout << '\t' << '\t' << '\t' << "Enter your password: ";
		cin >> ST[i].password;
	} while (!CheckPassword(ST[i].password));

	do {
		cout << '\t' << '\t' << '\t' << "Enter your email: ";
		cin.ignore(INT_MAX, '\n');
		getline(cin, ST[i].email);
	} while (!Checkmail(ST[i].email));

	do {
		cout << '\t' << '\t' << '\t' << "Enter your phone number [00-000000]: ";
		cin >> ST[i].phone;
	} while (!checkPhone(ST[i].phone));

	ST[i].userID = stoi(generateRandomID());
	cout << endl << endl << endl << '\t' << '\t' << '\t' << "Your unique ID is: " << ST[i].userID << ", Please keep it safe!" << endl << endl << endl;



	outfile << ST[i].userID << '\t'
		<< ST[i].firstName << '\t'
		<< ST[i].lastName << '\t'
		<< strhashing(ST[i].password) << '\t'
		<< ST[i].email << '\t'
		<< ST[i].phone << '\n';

	outfile.close();
	cout << '\t' << '\t' << '\t' << "Sign up successfull!" << endl << endl;
	cout << '\t' << '\t' << '\t' << "Log in is required: " << endl << endl;
}

bool loginClient()
{
	string* userID = new string;
	string* l = new string;
	string* lP = new string;
	string* Pass = new string;

	do
	{
		cout << '\t' << '\t' << '\t' << "Enter your userID: ";
		cin >> *userID;
	} while (!validuserID(*userID));


	bool efound = false;
	ifstream infile("Clients.txt", ios::in);

	if (infile.is_open())
	{
		string filename = "Clients.txt"; // Change this to your file's name

		if (isIdAvailable(*userID, filename))
		{
			cout << '\t' << '\t' << '\t' << "The user is Valid." << endl << endl;
			loggedInUserID = stoi(*userID);

			do
			{
				cout << '\t' << '\t' << '\t' << "Enter your password: ";
				cin >> *Pass;
			} while (!CheckPassword(*Pass));

			string hashed_pass = to_string(strhashing(*Pass));
			ifstream infile("Clients.txt", ios::in);
			string filename = "Clients.txt"; // Change this to your file's name
			pair<bool, string> result = isPassAvailable1(*userID, hashed_pass, filename);

			if (result.first)
			{
				cout << '\t' << '\t' << '\t' << "The password is Valid." << endl << endl;
				cout << '\t' << '\t' << '\t' << "Welcome client "<< result.second <<", choose one of the following options: " << endl;
				return true;
			}
			else
			{
				this_thread::sleep_for(chrono::seconds(3)); //add some time to prevent brute force attack
				cout << endl << endl << '\t' << '\t' << '\t' << "! ! ! Wrong password ! ! !" << endl;
				cout << endl << '\t' << '\t' << '\t' << "! ! ! System aborted ! ! !";
				return false;
			}
		}

		else
		{
			this_thread::sleep_for(chrono::seconds(3)); //add some time to prevent brute force attack
			cout << endl << endl << '\t' << '\t' << '\t' << "! ! ! User Not Found ! ! !" << endl;
			cout << endl << '\t' << '\t' << '\t' << "! ! ! System aborted ! ! !";
			return false;
		}
	}

}

bool loginAdmin()
{
	string* adminID = new string;
	string* l = new string;
	string* lP = new string;
	string* Pass = new string;

	do
	{
		cout << '\t' << '\t' << '\t' << "Enter your adminID: ";
		cin >> *adminID;
	} while (!validadminID(*adminID));

	bool efound = false;
	ifstream infile("Admin.txt", ios::in);

	if (infile.is_open())
	{
		string filename = "Admin.txt";

		if (isIdAvailable(*adminID, filename))
		{
			cout << '\t' << '\t' << '\t' << "The admin is Valid." << endl << endl;

			do {
				cout << '\t' << '\t' << '\t' << "Enter your password: ";
				cin >> *Pass;
			} while (!CheckPassword(*Pass));

			string hashed_pass = to_string(strhashing(*Pass));
			ifstream infile("Admin.txt", ios::in);
			string filename = "Admin.txt";


			if (isAdminPassAvailable(*adminID, hashed_pass, filename))
			{
				cout << endl << '\t' << '\t' << '\t' << "The password is Valid." << endl << endl;
				cout << '\t' << '\t' << '\t' << "Welcome admin, choose one of the following options: " << endl;
				return true;
			}
			else
			{
				this_thread::sleep_for(chrono::seconds(3)); //add some time to prevent brute force attack
				cout << endl << endl << '\t' << '\t' << '\t' << "! ! ! Wrong password ! ! !" << endl;
				cout << endl << '\t' << '\t' << '\t' << "! ! ! System aborted ! ! !";
				return false;
			}
		}

		else
		{
			this_thread::sleep_for(chrono::seconds(3)); //add some time to prevent brute force attack
			cout << endl << endl << '\t' << '\t' << '\t' << "! ! ! Admin Not Found ! ! !" << endl;
			cout << endl << '\t' << '\t' << '\t' << "! ! ! System  aborted ! ! !";
			return false;
		}
	}

}

void addNewCar(car* cr, int nbp)
{
	ofstream outfile;
	outfile.open("Car.txt", ios::out | ios::app);
	ifstream infile;
	infile.open("Car.txt", ios::in);

	int i = 1;

	do {
		cout << '\t' << '\t' << '\t' << "Enter the car's plate number: ";
		cin >> cr[i].plateNumber;
	} while (!validatePlateNumber(cr[i].plateNumber));



	string filename = "Car.txt";

	if (!isCarAvailableforAdding(cr[i].plateNumber, filename))
	{

		do {
			cout << '\t' << '\t' << '\t' << "Enter the car's brand: ";
			cin >> cr[i].brand;
		} while (!checkCarBrand(cr[i].brand));

		do {
			cout << '\t' << '\t' << '\t' << "Enter the car's year: ";
			cin >> cr[i].year;
		} while (!validateCarYear(cr[i].year));


		do {
			cout << '\t' << '\t' << '\t' << "Enter the car's model: ";
			cin.ignore(INT_MAX, '\n');
			getline(cin, cr[i].model);
		} while (!validateCarModel(cr[i].model));

		do {
			cout << '\t' << '\t' << '\t' << "Enter the car's price per day: ";
			cin >> cr[i].pricePerday;
		} while (!validatePricePerDay(cr[i].pricePerday));

		do {
			cout << '\t' << '\t' << '\t' << "Enter the car's color: ";
			cin >> cr[i].color;
		} while (!validateCarColor(cr[i].color));

		outfile << cr[i].plateNumber << '\t'
			<< cr[i].brand << '\t'
			<< cr[i].year << '\t'
			<< cr[i].model << '\t'
			<< cr[i].pricePerday << '\t'
			<< cr[i].color << '\n';

	}
	else
	{
		cout << '\t' << '\t' << '\t' << "ERROR - Car already exists." << endl;
	}
	outfile.close();
}

void removeCar()
{
	int nbp = 2;
	car* cr = new car[nbp];
	displayCars();

	cout << endl << '\t' << '\t' << '\t' << "Enter the data about the car you wish to remove: " << endl << endl;
	getCarDatatoremove(cr);
	remove("Car.txt");
	rename("temp.txt", "Car.txt"); // Rename the temporary file to the original name
}

void modifyCar()
{
	int nbp = 2;
	car* cr = new car[nbp];
	displayCars();

	cout << endl << '\t' << '\t' << '\t' << "Enter the data about the car you wish to modify: " << endl << endl;
	getCarDataToModify(cr);
	remove("Car.txt");
	rename("temp.txt", "Car.txt"); // Rename the temporary file to the original name

}


void adminOptions()
{
	int option;
	do
	{
		cout << '\t' << '\t' << '\t' << "1- Add a new car." << endl;
		cout << '\t' << '\t' << '\t' << "2- Remove an existing car. " << endl;
		cout << '\t' << '\t' << '\t' << "3- Modify an already existing car." << endl;
		cin >> option;
	} while (option != 1 && option != 2 && option != 3);

	if (option == 1)
	{
		int nbp = 2;
		car* cr = new car[nbp];
		addNewCar(cr, nbp);
	}

	if (option == 2)
	{
		removeCar();
	}

	if (option == 3)
	{
		modifyCar();
	}

}

void clientOptions()
{
	int option;
	do {
		cout << '\t' << '\t' << '\t' << "1- Rent a car." << endl;
		cout << '\t' << '\t' << '\t' << "2- Cancel an existing car rental reservation. " << endl;
		cout << '\t' << '\t' << '\t' << "3- Modify a rented car date." << endl;
		cin >> option;
	} while (option != 1 && option != 2 && option != 3);

	if (option == 1)
	{
		int nbp = 2;
		car* cr = new car[nbp];
		displayCars();

		cout << endl << '\t' << '\t' << '\t' << "Provide us with the necessary information to complete the rental procedure: " << endl << endl << endl;
		getCarDatatorent(cr);
	}

	if (option == 2)
	{
		int nbp = 2;
		car* cr = new car[nbp];
		displayRentsOfClient();

		cout << endl << '\t' << '\t' << '\t' << "Provide us with the necessary information to cancel the desired rent: " << endl << endl << endl;
		getCarDatatocancelrent(cr);
		remove("rental cars.txt");
		rename("temp1.txt", "rental cars.txt"); 
	}

	if (option == 3)
	{
		int nbp = 2;
		car* cr = new car[nbp];
		displayRentsOfClient();

		cout << endl << '\t' << '\t' << '\t' << "Provide us with the necessary information to cancel the desired rent: " << endl << endl << endl;
		getCarDatatorenttomodify(cr);
		remove("rental cars.txt");
		rename("temp1.txt", "rental cars.txt");
	}

}


int main()
{

	const int MAX_CARS = 100; // Maximum number of cars
	RentalCar rentalCars[MAX_CARS];
	car carDetails[MAX_CARS];

	readRentalCars("rental cars.txt", rentalCars, MAX_CARS);
	readCarDetails("Car.txt", carDetails, MAX_CARS);

	int rentalCarsCount = 0;
	int carDetailsCount = 0;


	int customertype, clientoradmin, s;
	cout << '\t' << '\t' << '\t' << "Welcome to Kiwan's Car Rental Company!" << endl;
	cout << '\t' << '\t' << '\t' << "I hope you will find you order." << endl;
	cout << '\t' << '\t' << '\t' << "--------------------------------------" << endl;

	do
	{
		cout << "\t\t\tAre you a new customer?" << endl;
		cout << "\t\t\t1- YES" << endl;
		cout << "\t\t\t2- NO" << endl;
		cin >> customertype;
	} while (customertype != 1 && customertype != 2);

	if (customertype == 1)
	{
		cout << '\t' << '\t' << '\t' << "Sign up procedure: " << endl;
		int nbP = 2;

		client* ST = new client[nbP];
		Create_Profile(ST, nbP);
		delete[]ST;

		do
		{
			cout << '\t' << '\t' << '\t' << "Are you a client or an administrator?" << endl;
			cout << '\t' << '\t' << '\t' << "1- Client" << endl;
			cout << '\t' << '\t' << '\t' << "2- Administrator" << endl;
			cin >> clientoradmin;
		} while (clientoradmin != 1 && clientoradmin != 2);

		if (clientoradmin == 1)
		{
			loginClient();
		}
		if (clientoradmin == 2)
		{
			loginAdmin();
		}
	}

	if (customertype == 2)
	{
		do
		{
			cout << '\t' << '\t' << '\t' << "Are you a client or an administrator?" << endl;
			cout << '\t' << '\t' << '\t' << "1- Client" << endl;
			cout << '\t' << '\t' << '\t' << "2- Administrator" << endl;
			cin >> clientoradmin;
		} while (clientoradmin != 1 && clientoradmin != 2);

		if (clientoradmin == 1)
		{
			if (loginClient())
			{
				clientOptions();

			}
		}
		if (clientoradmin == 2)
		{
			if (loginAdmin())
			{
				adminOptions();
			}

		}

		do {
			cout << "\t\t\t Would you like to get a sorted list of all rented cars?" << endl;
			cout << "\t\t\t1- YES" << endl;
			cout << "\t\t\t2- NO " << endl;
			cin >> s;
		} while (s != 1 && s != 2);

		if (s == 1)
		{
			for (int i = 0; i < MAX_CARS && rentalCars[i].plateNumber != ""; ++i) {
				for (int j = 0; j < MAX_CARS && carDetails[j].plateNumber != ""; ++j) {
					if (rentalCars[i].plateNumber == carDetails[j].plateNumber) {
						rentalCars[i].pricePerDay = carDetails[j].pricePerday;
						rentalCarsCount++;
						carDetailsCount++;
						break;
					}
					carDetailsCount++;
				}
			}

			// Sort rental cars by price per day in ascending order
			sort(rentalCars, rentalCars + rentalCarsCount, compareByPrice);

			// Write sorted rental cars to a new file
			ofstream outFile("sorted_rental_cars.txt");
			if (outFile.is_open()) 
			{
				for (int i = 0; i < rentalCarsCount; ++i) 
				{
					outFile << rentalCars[i].plateNumber << '\t' << rentalCars[i].userID << '\t' << rentalCars[i].startDate << '\t' << rentalCars[i].endDate << '\t' << rentalCars[i].pricePerDay << endl;
				}
				outFile.close();
				//generatePDF("Receipt.pdf");
				cout << "Receipt generated successfully\n";
				remove("sorted_rental_cars.txt");
			}
			else 
			{
				cout << "Unable to open file for writing!" << endl;
			}
			return 0;
		}
		if (s == 2)
		{
			return 0;
		}
	}
	return 0;
}