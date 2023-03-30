/*
	This system is a menu-driven program that provides the service of community locker for residents 
	living in Boulevard Service Apartment to enhance the convenient lifestyle of community living.
	It allows residents to do resident registration and parcel collection.
	It allows courier to do parcel registration.

	Precondition:
	- A resident should have an registered account under an apartment unit number
	  to obtain parcel registration and collection services.
	- The courier should notify the parcel's owner after the parcel is delivered.
	- The users should enter the choice in the main menu by using integer data type
	  to avoid error.
	- The system only allow a maximum of 500 account to be registered.
	- The system only allow an account to be registered under an apartment unit.
*/

#include <iostream>
#include <iomanip>
#include <cctype>
#include <fstream>
#include <cstring>
#include <ios>	   //used to get stream size
#include <limits>  //used to get numeric limits
using namespace std;

#define SIZE_OF_ARRAY 500 // allow maximum 500 account to be created
#define TIMES_OF_ATTEMPT 3 // allow maximum of 3 times attempting to enter the OTP code

struct APT_UNIT
{
	char apt_num[4];		// maximun 3 char
	char phone[12];			// maximum 11 char
	char password[16];		// maximum 16 char
	char OTP[5];			// maximum 4 char
	char name[31];			// maximum 30 char
};

//system modules
void resident_registration();
void parcel_registration();
void parcel_collection();

//read the information of registered account of residents
void read_data(APT_UNIT data[], int& index);

//Resident Registration functions
APT_UNIT register_account();
bool is_registered(APT_UNIT register_acc, APT_UNIT data[], int size, int& index);
void register_new_acc(APT_UNIT data[], APT_UNIT register_acc, int size);

//Parcel Registration functions
int line_count();
APT_UNIT parcel_register();
string generate_OTP();
void update_OTP(APT_UNIT data[], APT_UNIT parcel_data, string new_otp, int size);

//Parcel Collection functions
APT_UNIT get_data();
bool verification(APT_UNIT login_acc, APT_UNIT data[], int index, string* OTP);
void parcel_retrieval(APT_UNIT login_acc, string OTP);
void OTP_confirm(string OTP, int attempt_num);

int main()
{
	int choice;
	bool cont = true;  // initialization

	while (cont)
	{
		system("CLS"); //clear the information display on the screen
		cout << "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
			<< "\t                  Community Parcel Locker - Main Menu                  \n"
			<< "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
		cout << "\tWelcome to Boulevard Service Apartment community parcel lockers.\n\n";
		cout << "\t1. Resident Registration\n"
			<< "\t2. Parcel Registration\n"
			<< "\t3. Parcel Collection\n"
			<< "\t4. Exit.\n\n";

		//repeat while the choice is invalid
		do
		{
			cout << "\tEnter your choice <1-4>: ";
			cin >> choice;
			cout << endl;
			if (choice != 1 && choice != 2 && choice != 3 && choice != 4)
				cout << "\tThat is an invalid option.\n";
		} while (choice != 1 && choice != 2 && choice != 3 && choice != 4);

		//run the system modules according to the choice entered
		if (choice == 1)
			resident_registration();
		else if (choice == 2)
			parcel_registration();
		else if (choice == 3)
			parcel_collection();
		else if (choice == 4)
		{
			cout << "\n\tThanks for using Boulevard Service Apartment community parcel lockers.\n";
			cout << "\tExiting...\n";
			exit(1);  //stop the program
		}
	}
	return 0;
}

//resident registration module that provide registration functionalitiesto residents and store the information of registered account  
void resident_registration()
{
	system("CLS");
	APT_UNIT register_acc;
	APT_UNIT data[SIZE_OF_ARRAY];
	int index = 0;
	bool register_status = false;
	bool register_cont = true;
	read_data(data, index); //get the information of registered account

	cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear buffer before taking new line
	register_acc = register_account(); //get the information of the account to eb registered from residents
	register_status = is_registered(register_acc, data, SIZE_OF_ARRAY, index); //get the registration status
	if (register_status)
		register_new_acc(data, register_acc, SIZE_OF_ARRAY); //store registered account

	while (register_cont)
	{
		char choice;

		//repeat while the choice is invalid
		do
		{
			cout << "\tDo you want to register account again? \n"
				<< "\tEnter <Y/y> if YES.\n"
				<< "\tEnter <N/n> if NO.\n";
			cout << "\tPlease enter your option: ";
			cin >> choice;
			cout << endl;

			if (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n')
				cout << "\tThat is an invalid option. Please enter your option again.\n"; //display error message if choice is invalid
		} while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n');

		if (choice == 'Y' || choice == 'y')
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear buffer before taking new line
			register_acc = register_account();
			register_status = is_registered(register_acc, data, SIZE_OF_ARRAY, index);
			if (register_status)
				register_new_acc(data, register_acc, SIZE_OF_ARRAY);
		}
		else
		{
			register_cont = false;
			cout << "\tReturning to main menu...\n";
		}
	}
}

//parcel registration module that allows courier to register parcel to its owner's registered account
void parcel_registration()
{
	system("CLS");
	APT_UNIT data[SIZE_OF_ARRAY];
	APT_UNIT parcel_data;
	int index = 0;
	bool register_cont = true;
	read_data(data, index); //get the information of registered account
	int count = line_count(); //count line number in txt file 

	parcel_data = parcel_register();
	string new_otp = generate_OTP();
	update_OTP(data, parcel_data, new_otp, count);

	while (register_cont)
	{
		char choice;
		//repeat while the choice is invalid
		do
		{
			cout << "\tDo you want to register parcel again? \n"
				<< "\tEnter <Y/y> if YES.\n"
				<< "\tEnter <N/n> if NO.\n";
			cout << "\tPlease enter your option: ";
			cin >> choice;
			cout << endl;

			if (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n')
				cout << "\tThat is an invalid option. Please enter your option again.\n";   //display error message if choice is invalid
		} while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n');

		if (choice == 'Y' || choice == 'y')
		{
			parcel_data = parcel_register();
			string new_otp = generate_OTP();
			update_OTP(data, parcel_data, new_otp, count);
		}
		else
		{
			register_cont = false;
			cout << "\tReturning to main menu...\n";
		}
	}
}

//parcel collection module that allows residents to log in and view their OTP code and retrive their parcel
void parcel_collection()
{
	system("CLS");
	APT_UNIT data[SIZE_OF_ARRAY];
	APT_UNIT login_acc;
	string OTP;
	char choice;
	int index = 0;
	bool is_authentic = false;
	bool cont = true;
	read_data(data, index);
	
	
	//repeat while the account details input is not matching with the account details registered
	while (cont)
	{
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear buffer before taking new line
		login_acc = get_data();  //get the account information from resident
		is_authentic = verification(login_acc, data, index, &OTP);  //get the log in status

		if (!is_authentic)
		{
			cout << "\tThe information provided is incorrect.\n";
			cout << "\tTo attempt to log in again\tPRESS (R)\n";
			cout << "\tTo back to main menu\tPRESS(M)\n";

			do // repeat while the option is invalid
			{
				cout << "\tPlease enter your option: ";
				cin >> choice;   // ask user if he/she want to login again
				cout << endl;
				if (choice != 'r' && choice != 'R' && choice != 'M' && choice != 'm')
					cout << "\n\tThe character you have key in is not in the option.\n";  //display error message if choice is invalid
				else if(choice == 'r' || choice == 'R')
					cont = true;
				else if (choice == 'm' || choice == 'M')
				{
					cont = false;
					cout << "\tReturning to main menu...\n\n";
				}
			} while (choice != 'r' && choice != 'R' && choice != 'M' && choice != 'm');
		}
		else
			cont = false;
	}
	if (is_authentic)
		parcel_retrieval(login_acc, OTP); //display resident's info, ask resident whether want to retrive parcel, and matching OTP code for parcel collection
}

//read all the registered account and store it in structure of array, and get the maximum index number of the array
void read_data(APT_UNIT data[], int& index)
{
	ifstream in_file("apt_unit.txt");

	if (!in_file)
		cout << "Error in opening Personal Data File.\n";  //check for error
	else
	{
		in_file >> data[index].apt_num; //initialization
		while (in_file)
		{
			in_file >> data[index].phone;
			in_file >> data[index].password;
			in_file >> data[index].OTP;
			in_file.getline(data[index].name, 31);
			strcpy_s(data[index].name, &(data[index].name)[1]); // To delete first character (a blank space)
			index++;
			in_file >> data[index].apt_num;  // after-action update
		}
		in_file.close();
	}
}

//To get the information of the resident account to log in
APT_UNIT register_account()
{
	APT_UNIT register_acc;
	bool valid_name = true;
	bool is_digit_apt_num = true;
	bool is_digit_phone = true;
	bool good_password = false;

	system("CLS");
	cout << "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
		<< "\t                 RESIDENT REGISTRATION\n"
		<< "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
	
	//repeat while the name entered is invalid
	do
	{
		cout << "\tPlease enter your name: ";
		cin.getline(&register_acc.name[0], 31);

		//checking whether all character is alpahbet
		for (int i = 0; i < strlen(register_acc.name); i++)
			if (!isalpha(register_acc.name[i]) && register_acc.name[i] != ' ')
			{
				valid_name = false;
				i = 31;
			}

		if (!valid_name)
			cout << "\tPlease enter a valid name that only contain alphabetic characters.\n"; //display error if name is invalid
	} while (!valid_name);

	//repeat while the apartment unit number entered is invalid
	do
	{
		cout << "\tPlease enter your apartment unit number: ";
		cin >> register_acc.apt_num;
		
		//checking whether all character is digit
		for (int i = 0; i < strlen(register_acc.apt_num); i++)
		{
			if (!isdigit(register_acc.apt_num[i]))
			{
				is_digit_apt_num = false;
				i = strlen(register_acc.apt_num);
			}
			else
				is_digit_apt_num = true;
		}

		//display error if apartment unit number is invalid
		if (strlen(register_acc.apt_num) != 3 || !is_digit_apt_num)
			cout << "\tThe apartment unit number should be in range of 000 to 999.\n"
			<< "\tPlease enter your apartment unit number again.\n";   

	} while (strlen(register_acc.apt_num) != 3 || !is_digit_apt_num);

	//repeat while the phone number entered is invalid
	do
	{
		cout << "\tPlease enter your phone number: ";
		cin >> register_acc.phone;

		//checking whether all character is digit
		for (int i = 0; i < strlen(register_acc.phone); i++)
		{
			if (!isdigit(register_acc.phone[i]))
			{
				is_digit_phone = false;
				i = strlen(register_acc.phone);
			}
			else
				is_digit_phone = true;
		}

		//display error if phone number is invalid
		if (strlen(register_acc.phone) > 11 || strlen(register_acc.phone) < 9 || !is_digit_phone)
			cout << "\tThe phone number entered is invalid.\n\tPlease enter your phone number again.\n";
	} while (strlen(register_acc.phone) > 11 || strlen(register_acc.phone) < 9 || !is_digit_phone);

	//repeat while the password entered is invalid
	do
	{
		cout << "\tPlease enter your password : ";
		cin >> register_acc.password;

		int i = 0;
		//invalid if password is less than 8 characters
		if (strlen(register_acc.password) < 8)
			good_password = false;
		else
		{
			//invalid if do not have at least a digit
			for (int j = 0; j < strlen(register_acc.password); j++)
			{
				if (isdigit(register_acc.password[j]))
				{
					i++;
					j = 16;
				}
			}

			//invalid if do not have at least an uppercase
			for (int j = 0; j < strlen(register_acc.password); j++)
			{
				if (isupper(register_acc.password[j]))
				{
					i++;
					j = 16;
				}
			}

			//invalid if do not have at least an lowercase
			for (int j = 0; j < strlen(register_acc.password); j++)
			{
				if (islower(register_acc.password[j]))
				{
					i++;
					j = 16;
				}
			}
		}
		//valid if all criteria is met
		if (i == 3)
			good_password = true;

		//display error if phone number is invalid
		if (!good_password)
		{
			cout << "\tThe password entered is invalid.\n";
			cout << "\tThe password should be at least has 8 characters,\n"
				<< "\tcontain at least a uppercase letter,\n"
				<< "\ta lowercase letter, "
				<< "and a numeric digit.\n";
			cout << "\tPlease enter a valid password.\n";
		}
	} while (!good_password);

	cout << endl;

	// convert all character of name to uppercase
	for (int i = 0; i < 31; i++) 	
		register_acc.name[i] = toupper(register_acc.name[i]);

	return register_acc;
}

//return registration status
bool is_registered(APT_UNIT register_acc, APT_UNIT data[], int size, int& index)
{
	bool registered = true;
	//not registered if an account is registered under the apartment unit
	for (int i = 0; i < size; i++)
	{
		if (strcmp(register_acc.apt_num, data[i].apt_num) == 0)
		{
			registered = false;
			i = size;
		}
	}
	//display registration failed
	if (!registered)
		cout << "\tThe apartment unit you register was registered.\n"
			<< "\tOnly an account can be registered under an apartment unit.\n";


	//register the account
	else
	{
		cout << "\tThis account is registered successfully under " << register_acc.apt_num << " apartment unit.\n";
		strcpy_s(data[index].name, register_acc.name);
		strcpy_s(data[index].apt_num, register_acc.apt_num);
		strcpy_s(data[index].password, register_acc.password);
		strcpy_s(data[index].phone, register_acc.phone);
		index++;
	}
	return registered;
}

//append the registered account into the file
void register_new_acc(APT_UNIT data[], APT_UNIT register_acc, int size)
{
	ofstream out_file("apt_unit.txt", ios::app);

	if (!out_file)
		cout << "\tError opening output file\n";
	else
	{
		out_file << register_acc.apt_num << " "
			<< register_acc.phone << " "
			<< register_acc.password << " "
			<< "0000" << " "  //OTP is set to '0000' after registration
			<< register_acc.name
			<< endl;
	}
	out_file.close();

	cout << "\tYour personal information is shown below\n\n";
	cout << "\t" << setw(21) << left << "Apartment unit number" << ": " << register_acc.apt_num << endl;
	cout << "\t" << setw(21) << left << "Name" << ": " << register_acc.name << endl;
	cout << "\t" << setw(21) << left << "Phone" << ": " << register_acc.phone << "\n\n";
}

//count line number in txt file 
int line_count()
{
	ifstream in_file("apt_unit.txt");
	int count = 0;

	if (!in_file)
		cout << "Error in opening Personal Data File.\n";
	else
	{
		char c;										
		while (in_file.get(c))
		{
			if (c == '\n')
				count++;
		}
	}
	return count;
}

//get the apartment unit number of the parcel's owner from courier
APT_UNIT parcel_register()
{
	APT_UNIT parcel_data;

	system("CLS");
	cout << "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
		<< "\t                 PARCEL REGISTRATION\n"
		<< "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";

	cout << "\tPlease enter the apartment unit number: ";
	cin >> parcel_data.apt_num;
	cout << endl;

	return parcel_data;
}

//generate OTP range from 0000 to 9999
string generate_OTP()
{
	srand(time(NULL));
	int random[4]{ 0 };

	for (int i = 0; i < 4; i++)				//generate random number
		random[i] = rand() % 10;

	string str;								//turn random numbers to string
	for (int i : random)
		str.push_back(i + '0');

	return str;
}

//update the OTP code of the account that a parcel is registered
void update_OTP(APT_UNIT data[], APT_UNIT parcel_data, string new_otp, int size)
{
	ofstream out_file("apt_unit.txt");
	int determine = 0;

	if (!out_file)
		cout << "Error opening output file\n";
	else
	{
		for (int i = 0; i < size; i++)					//rewrite file
		{
			if (strcmp(data[i].apt_num, parcel_data.apt_num) == 0)
			{
				out_file << data[i].apt_num << " "
					<< data[i].phone << " "
					<< data[i].password << " "
					<< new_otp << " "
					<< data[i].name
					<< endl;
				determine = 1;							//determine apt_num exist or not
			}

			else
				out_file << data[i].apt_num << " "
				<< data[i].phone << " "
				<< data[i].password << " "
				<< data[i].OTP << " "
				<< data[i].name
				<< endl;
		}
	}
	out_file.close();

	if (determine == 1)
		cout << "\tParcel registerred successfully and new OTP is updated.\n";
	else
		cout << "\tApartment unit number is incorrect, please try again." << endl;

}

//get the account information from resident to log in
APT_UNIT get_data()
{
	APT_UNIT login;
	system("CLS");
	cout << "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
		<< "\t                 PARCEL COLLECTION\n"
		<< "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
	cout << "\tPlease enter your name: ";
	cin.getline(login.name, 31);
	cout << "\tPlease enter your apartment unit number: ";
	cin >> login.apt_num;
	cout << "\tPlease enter your phone number: ";
	cin >> login.phone;
	cout << "\tPlease enter your password: ";
	cin >> login.password;
	cout << endl;
	for (int i = 0; i < 31; i++) 	// convert all characters to uppercase
		login.name[i] = toupper(login.name[i]);

	return login;
}

// To verify the account details entered by the user, return verification status, and get OTP of the account
bool verification(APT_UNIT login_acc, APT_UNIT data[], int index, string* OTP)
{
	bool login = false;
	
	//check the account info entered with the information of registered accounts
	for (int i = 0; i < index; i++)
	{
		if (strcmp(login_acc.apt_num, data[i].apt_num) == 0)
		{

			if (strcmp(login_acc.phone, data[i].phone) == 0
				&& strcmp(login_acc.name, data[i].name) == 0
				&& strcmp(login_acc.password, data[i].password) == 0)
			{
				*OTP = data[i].OTP;
				login = true;
			}
		}
	}
	return login;
}

// To display account data and OTP code,
void parcel_retrieval(APT_UNIT login_acc, string OTP)
{
	char choice;

	//display account data and OTP code and ask account holder whether want to retrive the parcel
	system("CLS");
	cout << "\n\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << "\t                     ACCOUNT DETAILS\n";
	cout << "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
	cout << "\t" << setw(21) << left << "Apartment unit number" << ": " << login_acc.apt_num << endl;
	cout << "\t" << setw(21) << left << "Name" << ": " << login_acc.name << endl;
	cout << "\t" << setw(21) << left << "Phone" << ": " << login_acc.phone << "\n\n";
	cout << "\t***************************\n";
	cout << "\t " << setw(21) << left << "Your OTP code is" << OTP << endl;
	cout << "\t***************************\n\n";

	cout << "\tWould you like to retrive your parcel?\n"
		<< "\tPlease enter <Y/y> for parcel collection.\n"
		<< "\tPlease enter <N/n> to return back to main menu.\n";

	do // repeat while the option is invalid
	{
		cout << "\tPlease enter your option: ";
		cin >> choice;   // ask user if he/she want to login again
		cout << endl;
		if (choice == 'Y' || choice == 'y')  // type Y or y for parcel collection
			OTP_confirm(OTP, TIMES_OF_ATTEMPT);  //redirect the account holder to enter OTP interface for parcel collection
		else if (choice == 'N' || choice == 'n') //type N or n to back to main menu
			cout << "\tReturning to main menu...\n\n";
		else
			cout << "\n\tThe character you have key in is not in the option.\n";  //display error when choice si invalid
	} while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n');
}

//allow residents to enter OTP code and collect parcel when the OTP is correct
void OTP_confirm(string OTP, int attempt_num)
{
	char action;
	char OTP1[5];

	system("CLS");
	cout << "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
		<< "\t                 PARCEL COLLECTION\n"
		<< "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";

	cout << "\tPlease enter the OTP code : ";
	cin >> OTP1;
	attempt_num--;

	//repeat while OTP code entered is incorrect and still have chance to attempt
	while (OTP1 != OTP && attempt_num > 0)
	{
		system("CLS");
		cout << "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
			<< "\t                 PARCEL COLLECTION\n"
			<< "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
		cout << "\tThat is an invalid OTP code.\n";

		//display if still have chance to attempt
		if (attempt_num != 0)
		{
			cout << "\tDo you want to re-enter OTP?\n"
				<< "\t-------------------------------------\n"
				<< "\tEnter <Y/y> if YES.\n"
				<< "\tYou have " << attempt_num << " more chances to re-enter.\n"
				<< "\tEnter <N/n> if NO.\n"
				<< "\tYou will return to the main menu.\n";
			cout << "\tPlease enter your option: ";
			cin >> action;
			cout << endl;
		}

		//repeat  while the action entered is invalid
		while (action != 'Y' && action != 'y' && action != 'N' && action != 'n')
		{
			//display if still have chance to attempt
			if (attempt_num != 0)
			{
				system("CLS");
				cout << "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
					<< "\t                 PARCEL COLLECTION\n"
					<< "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
				cout << "\tThat is an invalid option.\n"
					<< "\tDo you want to re-enter OTP? \n"
					<< "\t-------------------------------------\n"
					<< "\tEnter <Y/y> if YES.\n"
					<< "\tYou have " << attempt_num << " more chances to re-enter.\n"
					<< "\tEnter <N/n> if NO.\n"
					<< "\tYou will return to the main menu.\n";
				cout << "\tPlease enter your option: ";
				cin >> action;
				cout << endl;
			}
		}

		// when resident want to re-enter the OTP code and still have chance to attempt
		if (action == 'Y' || action == 'y' || attempt_num != 0)
		{
			cout << "\tPlease enter the OTP code : ";
			cin >> OTP1;
			attempt_num--;
		}

		//display no chance to attempt and will back to main menu if there is no chance and OTP code entered is incorrect
		if (attempt_num == 0 && OTP1 != OTP)
		{
			string back;
			cout << "\tThat is an invalid option.\n";
			cout << "\tYou have used up all your chances." << endl;
			cout << "\tKey in any character to back to main menu. ";
			cin >> back;
		}

		//back to main menu if the user wants to and OTP code is incorrect
		if ((action == 'N' || action == 'n' || attempt_num == 0) && OTP1 != OTP)
		{
			cout << "\tReturning to main menu......." << endl;
			attempt_num = 0;
		}
	}

	//display that the OTP code entered is correct and will back to main menu
	if (OTP1 == OTP)
	{
		string back;
		cout << "\tThis is the correct OTP.\n\tYou may retrive your parcel at your locker.\n";
		cout << "\tKey in any character to back to main menu. ";
		cin >> back;
		cout << "\tReturning to main menu......." << endl;
	}
}