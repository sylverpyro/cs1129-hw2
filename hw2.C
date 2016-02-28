//---------------------------------------------------------------
// File		:hw2.C
// Author	:Michael Benson
// Course	:CS1129
// Date		:Febuary 20, 2006
// 
// Homework 2 will take a birthday input in the format dd/mm/yyyy, 
// output the birthdate in regular text, then generate various 
// horoscope items based on that particular date
//	
// Extra Credit
// 
// 1. The program will, when entering the birthdate, only accept
// 	Febuary 29th if it falls in a leap year
// 2. The users current age is calculated instead of calculating
// 	based on December 31, 2006
//----------------------------------------------------------------


#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Start up functions
void getBirthdate(int& userMonth, int& userDay, int& userYear);
int getAge(int userDay, int userMonth, int userYear);
void personalInfo(int userMonth, int userDay, int userYear, int age);

// Menu functions
int mainMenu();
void menuControl(int userMonth, int userDay);
void exitCheck(int& select);

// Services funcions
void zodiacProfile(int userMonth, int userDay);
void luckyNumbers(int randFactor);
void horoscope(int userMonth, int userDay, int randFactor);

// Horoscope prediction functions
void openingLines(int select);
void moneyPredict(int select);
void lovePredict(int select);
void healthPredict(int select);

// Utility Functions
bool monthCheck (int userMonth, int userDay, int userYear);
bool yearCheck(int userYear);
bool leapYearCheck(int userYear);
void datePrint(int userMonth, int userDay, int userYear);
void signPrint(int userMonth, int userDay);
int signReturn(int userMonth, int userDay);

// Global variables for Months, Month Lengths, and Signs
enum Months { JAN = 1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, 
	OCT, NOV, DEC };
enum MonthLengths { JAN_L = 31, FEB_L = 28, LEAP_FEB_L = 29, 
	MAR_L = 31, APR_L = 30, MAY_L = 31, JUN_L = 30, JUL_L = 31, 
	AUG_L = 31, SEP_L = 30,	OCT_L = 31, NOV_L = 30, DEC_L = 31 };
enum Signs { CAP, AQU, PIS, ARI, TAU, GEM, CAN, LEO, VIR, LIB, SCO, 
	SAG };

//-----------------------------------------------------------
// main()
//-----------------------------------------------------------

int main()
{
	// Variable Declarations
	int userMonth;
	int userDay;
	int userYear;
	int age;
	
	// Run the startup functions (get user information)
	getBirthdate(userMonth, userDay, userYear);
	age = getAge(userMonth, userDay, userYear);
	personalInfo(userMonth, userDay, userYear, age);	

	// Pass control over to the menu function
	menuControl(userMonth, userDay);
	
	return 0;

}

//--------------------------------------------------------------
// getBirthdate (int&, int&, int&)
// Pre-conditions	:Needs userMonth, userDay, and userYear 
// Post-conditions 	:Modifies userMonth, Day, and Year in 
// 			calling function to user input
//--------------------------------------------------------------

void getBirthdate(int& userMonth,int& userDay, int& userYear)
{
	// Variable Declarations
	char sl;	
	bool acceptYear = false;
	bool acceptDay = false;
	
	// Get user input
	cout << "Enter your birthdate (mm/dd/yyyy)" << endl;
	cin >> userMonth >> sl >> userDay >> sl >> userYear;

	// Check to make sure its a valid day and year
	acceptYear = yearCheck(userYear);
	acceptDay = monthCheck(userMonth, userDay, userYear);
	
	// If either check fails, ask for the input again
	while ( acceptYear == false || acceptDay == false )
	{
		// Get user input
		cout << "Please enter a valid date between January 1";
		cout << ", 1900 and December 31, 2006" << endl;
		cin >> userMonth >> sl >> userDay >> sl >> userYear;
		
		// Check the day and year are valid
		acceptDay = yearCheck(userYear);
		acceptYear = monthCheck(userMonth, userDay, userYear);
	}
}


//-------------------------------------------------------------
// getAge(int, int, int)
// Pre-conditions	:Needs userDay, userMonth, and userYear
// Post-conditions	:Returns the users age
//-------------------------------------------------------------

int getAge(int userDay, int userMonth, int userYear)
{
	// Variable Declarations
	int curDay;
	int curMonth;
	int curYear;
	int age;
	bool acceptDay = false;
	bool acceptYear = false;
	char sl;
	
	// Loop untill the user inputs a correct date
	while (acceptDay == false || acceptYear == false)
	{
		cout << "Please enter todays date (mm/dd/yyyy)\n";
		cin >> curDay >> sl >> curMonth >> sl >> curYear;
		acceptDay = monthCheck(curDay, curMonth, curYear);
		acceptYear = yearCheck(curYear);
		if (curYear <= userYear)
			acceptYear = false;
	}
	
	// Calculate the users current age based on there input
	//  (within a year)
	if (userMonth <= curMonth && userDay <= curDay)
		age = curYear - userYear;
	else
		age = curYear - userYear - 1;
	
	return age;
}

//----------------------------------------------------------
// personalInfo(int, int, int, int)
// Pre-conditions	:Needs userMonth, Day, Year, and age
// Post-conditions	:Prints formatted user information
//----------------------------------------------------------

void personalInfo(int userMonth, int userDay, int userYear, int age)
{

	// Print out header
	cout << "--------------Personal Info---------------" << endl;
	
	// Print out the birthdate with datePrint(int, int, int)
	cout << "\t  Birthdate:\t";
	datePrint(userMonth, userDay, userYear);
	cout << endl;
	
	// Print out the age
	cout << "\t\tAge:\t" << age << endl;

	// Print out the users sign with signPrint(int, int)
	cout << "  Astrological Sign:\t";
	signPrint(userMonth, userDay);
	cout << endl;

	cout << "------------------------------------------" << endl;
	

}

//--------------------------------------------------------
// mainMenu()
// Pre-conditions	:None
// Post-conditions	:Returns the users selection
//---------------------------------------------------------

int mainMenu()
{
	// Variable Declations
	int select;
	
	// Prompt the user for there input
	cout << "Please chose a service" << endl;
	cout << " 1. Zodiac Profile" << endl;
	cout << " 2. Daily Horoscope" << endl;
	cout << " 3. Lucky Numbers" << endl;
	cout << " 4. Quit" << endl;
	cout << "Your Selection?" << endl;

	cin >> select;

	return select;
}

//-----------------------------------------------------------------
// menuControl(int, int)
// Pre-conditions	:Needs userMonth, and userDay
// Post-conditions	:Executes functions based on user input
//-----------------------------------------------------------------

void menuControl(int userMonth, int userDay)
{
	// Variable Declations
	int select;
	int randN;
	
	// Get a random number and store it so it is not lost
	// 	while useing other functions that rely on srand()
	// 	This variable is used latter to ensure that output
	// 	during a sigle run of the program is the same
	srand(time(0));
	randN = rand();
	
	while (select != 4)
	{
		// Find out what the user wants to do
		select = mainMenu();

		// Run the program the user asked for
		switch(select)
		{
			case 1:
				zodiacProfile(userMonth, userDay);
				break;
			case 2:
				horoscope(userMonth, userDay, randN);
				break;
			case 3:
				luckyNumbers(randN);
				break;
			case 4:
				exitCheck(select);
				break;
			default:
				select = 0;
		}
	}
}

//--------------------------------------------------------------
// exitCheck(int&)
// Pre-conditions	:Needs a value for select
// Post-condiditions	:Modifies select to be 0 if user decides 
// 			to not exit
//--------------------------------------------------------------

void exitCheck(int& select)
{
	// Variable Declarations
	char answer;
	
	// Prompt the user
	cout << "Are you sure you want to quit? [y/n]";
	cin >> answer;
	
	// Make sure the user inputed either y or n
	while (answer != 'y' && answer != 'n')
	{
		cout << "Invalid selection" << endl;
		cout << "Are you sure you want to quit? [y/n]";
		cin >> answer;
	}
	
	// If the user does not want to exit, set select to 0
	if (answer == 'n')
		select = 0;
}	

//--------------------------------------------------------------
// zodiacProfile(int, int)
// Preconditions	:Needs userMonth and userDay
// Postconditions	:Prints out the profile of the Zodiac 
// 			for the given day and month
//-------------------------------------------------------------

void zodiacProfile(int userMonth, int userDay)
{
	// Variable Declarations
	int sign;

	// Print profile Header
	cout << endl;
	cout << endl;
	cout << "-------------Zodiac Profile--------------";
	cout << endl;
	
	// Get the users sign
	sign = signReturn(userMonth, userDay);

	// Print out he sign's profile
	switch (sign)
	{
		case CAP:
			cout << "Capricorn - The Goat\n";
			cout << "Practical and prudent\n";
			cout << "Ambitious and disciplined\n";
			cout << "Patient and carefull\n";
			cout << "Humorous and reserved\n";
			break;
		case AQU:
			cout << "Aquarius - The Water Carrier\n";
			cout << "Friendly and humanitarian\n";
			cout << "Honest and loyal\n";
			cout << "Original and inventive\n";
			cout << "Independent and intellectual\n";
			break;
		case PIS:
			cout << "Pisces - The Fishes\n";
			cout << "Imaginative and sensitive\n";
			cout << "Compassionate and kind\n";
			cout << "Selfless and unworldly\n";
			cout << "Intuitive and sympathetic\n";
			break;
		case ARI:
			cout << "Aries - The Ram\n";
			cout << "Adventurous and energetic\n";
			cout << "Pioneering and courageous\n";
			cout << "Enthusiastic and confident\n";
			cout << "Dynamic and quick-witted\n";
			break;
		case TAU:
			cout << "Taurus - The Bull\n";
			cout << "Patient and reliable\n";
			cout << "Warmhearted and loving\n";
			cout << "Presistent and determined\n";
			cout << "Placid and security loving\n";
			break;
		case GEM:
			cout << "Gemini - The Twins\n";
			cout <<	"Adaptable and versatile\n";
			cout <<	"Communicative and witty\n";
			cout << "Intellectual and eloquent\n";
			cout << "Youthful and lively\n";
			break;
		case CAN:
			cout << "Cancer - The Crab\n";
			cout << "Emotional and loving\n";
			cout << "Intuitive and imaginative\n";
			cout << "Shrewd and cautious\n";
			cout << "Protective and sympathetic\n";
			break;
		case LEO:
			cout << "Leo - The Lion";
			cout << "Generous and warmhearted\n";
			cout << "Creative and enthusiastic\n";
			cout << "Broad-minded and expansive\n";
			cout << "Failthful and loving\n";
			break;
		case VIR:
			cout << "Virgo - the Virgin\n";
			cout << "Modest and shy\n";
			cout << "Meticulous and reliable\n";
			cout << "Practical and diligent\n";
			cout << "Intelligent and analytical\n";
			break;
		case LIB:
			cout << "Libra - The Scales\n";
			cout << "Diplomatic and urband\n";
			cout << "Romantic and charming\n";
			cout << "Easygoing and sociable\n";
			cout << "Idealistic and peaceable\n";
			break;
		case SCO:
			cout << "Scorpio - The Scorpion\n";
			cout << "Determined and forceful\n";
			cout << "Emotional and intuitive\n";
			cout << "Powerful and passionate\n";
			cout << "Exciting and magnetic\n";
			break;
		case SAG:
			cout << "Sagittarius - The Archer\n";
			cout << "Optimistic and freedom-loving\n";
			cout << "Jovial and good-humored\n";
			cout << "Honest and straightforward\n";
			cout << "Intellectual and philosophical\n";
			break;
	}
	cout << "------------------------------------------";
	cout << endl;
	cout << endl;
	cout << endl;
}

//------------------------------------------------------------------
// horoscope(int, int, int)
// Preconditions	:Needs userMonth, userDay, and a randFactor
// Postconditions	:Outputs the sign name followed by a set
// 			of predictions
// -----------------------------------------------------------------

void horoscope(int userMonth, int userDay, int randFactor)
{
	// Variable Declarations
	int sign;

	// Find the users sign
	sign = signReturn(userMonth, userDay);
	
	// Display the users sign
	cout << "-------------";
	signPrint(userMonth, userDay);
	cout << "-------------\n";
	
	// Seed the number generator with the users sign and the
	// 	randFactor to ensure same output within this run,
	// 	but different output next run.
	srand(sign * randFactor);
	
	// Output the horoscope with each respective function
	openingLines(rand());
	moneyPredict(rand());
	lovePredict(rand());
	healthPredict(rand());
	cout << "--------------------------------\n";
}

//------------------------------------------------------------------
// luckyNumbers(int)
// Preconditions	:Needs a randomFactor to seed srand() with
// Postconditions	:Prints out 5 asending, differrent, random 
// 			numbers
//------------------------------------------------------------------

void luckyNumbers(int randFactor)
{
	// Variable Declarations
	int n1,n2,n3,n4,n5;
	
	// Seed the random generator
	srand(randFactor);

	// Generate 5 random numbers in acending order
	n1 = rand()%10 + 1;
	n2 = n1 + rand()%10 + 1;
	n3 = n2 + rand()%10 + 1;
	n4 = n3 + rand()%10 + 1;
	n5 = n4 + rand()%10 + 1;
	
	// Print the numbers out
	cout << "------------Lucky Numbers------------------\n";

	cout << "\t" << n1 << "\t" << n2 << "\t" << n3 << "\t" << n4;
        cout << "\t" << n5;	

	cout << "\n-------------------------------------------\n";
}	

//---------------------------------------------------------------
// openingLines(int)
// Preconditions	:Needs select (any int)
// Postconditions	:Prints out an opening line
//---------------------------------------------------------------

void openingLines(int select)
{
	// Variable Declarations
	const int numberOfCases = 4;

	// Make sure the input variable is 0-numberOfCases
	select = select % numberOfCases;
	
	// Print out an opening Line
	switch(select)
	{
		case 0:
			cout << "Your clever wit will be much";
			cout << " in evidence as the day begins.";
			break;
		case 1:
			cout << "You're quite preceptive today ";
			cout << "and you'll have good insights ";
			cout << "into what makes other tick.";
			break;
		case 2:
			cout << "You may be making some innovative ";
			cout << "changes around the home today.";
			break;
		case 3:
			cout << "You'll be serious in outlook today";
			cout << ", with an eye for getting things ";
			cout << "accomplished.";
			break;
	}
	
	cout << endl;
}

//---------------------------------------------------------------
// moneyPredict(int)
// Preconditions	:Needs select (any int)
// Postconditions	:Prints out a money prediction
//---------------------------------------------------------------
	
void moneyPredict(int select)
{
	// Variable Declarations
	const int numberOfCases = 4;

	// Make sure the input variable is 0-numberOfCases
	select = select % numberOfCases;
	
	// Print out a money prediction
	switch(select)
	{
		case 0:
			cout << "Career hopes could escalate; hard ";
			cout << "work and sacrifice will be needed.";
			break;
		case 1:
			cout << "Where money is concerned, try not ";
			cout << "to argue.  Meet others halfway.";
			break;
		case 2:
			cout << "A money delay is finally over.";
			break;
		case 3:
			cout << "Watch for an important turning ";
			cout << "point in business, perhaps a new "; 
			cout << "job or assignment";
			break;
	}
	
	cout << endl;
}

//---------------------------------------------------------------
// lovePredict(int)
// Preconditions	:Needs select (any int)
// Postconditions	:Prints out a love prediction
//---------------------------------------------------------------
	
void lovePredict(int select)
{
	// Variable Declarations
	const int numberOfCases = 4;

	// Make sure the input variable is 0-numberOfCases
	select = select % numberOfCases;
	
	// Print out a love Prediction
	switch(select)
	{
		case 0:
			cout << "You should avoid taking chances or ";
			cout << "speculating, especially in romance.";
			break;
		case 1:
			cout << "An unusual person becomes your ";
			cout << "friend.";
			break;
		case 2:
			cout << "Find a more sensible approach to ";
			cout << "your romantic life and stick with ";
			cout << "it.";
			break;
		case 3:
			cout << "Tonight, a gift or favor may be ";
			cout << "just the thing you have been "; 
			cout << "looking for.";
			break;
	}
	
	cout << endl;
}

//---------------------------------------------------------------
// healthPredict(int)
// Preconditions	:Needs select (any int)
// Postconditions	:Prints out a health prediction
//---------------------------------------------------------------

void healthPredict(int select)
{
	// Variable Declarations
	const int numberOfCases = 4;

	// Make sure the input variable is 0-numberOfCases
	select = select % numberOfCases;
	
	// Print out a health prediction
	switch(select)
	{
		case 0:
			cout << "Take advantage of your good spirits";
			cout << " and escalated energy levels today";
			break;
		case 1:
			cout << "Keep plans and activities on the ";
			cout << "simple side, since energy levels ";
			cout << "deplet quickly.";
			break;
		case 2:
			cout << "Your mental energies are strong ";
			cout << "now and you'll be quick to act ";
			cout << "on your ideas";
			break;
		case 3:
			cout << "It's time to moderate your food ";
			cout << "and alcohol consumption and diet "; 
			cout << "a little";
			break;
	}
	
	cout << endl;
}

//------------------------------------------------------------------
// monthCheck(int, int, int)
// Pre-conditions	:Tkes userMonth, userDay, and userYear
// Post-conditions	:Returns boolean true if the day is within 
// 			a particular month
//------------------------------------------------------------------

bool monthCheck(int userMonth, int userDay, int userYear)
{
	// Variable Declarations
	bool correctLength = false;
	
	// Case selected based on userMonth
	switch (userMonth)
	{
		case JAN:
			if (userDay <= JAN_L)
				correctLength = true;
			break;
		case FEB:
			// Check for Leap Year	
			
			// Case for Leap Year
			if (leapYearCheck(userYear) == true)
			{
				if (userDay <= LEAP_FEB_L)
					correctLength = true;
			}
			
			// Case for NON-Leap Year
			else
			{
				if (userDay <= FEB_L)
					correctLength = true;
			}
			break;
		case MAR:
			if (userDay <= MAR_L)
				correctLength = true;
			break;
		case APR:
			if (userDay <= APR_L)
				correctLength = true;
			break;
		case MAY:
			if (userDay <= MAY_L)
				correctLength = true;
			break;
		case JUN:	
			if (userDay <= JUN_L)
				correctLength = true;
			break;
		case JUL:	
			if (userDay <= JUL_L)
				correctLength = true;
			break;
		case AUG:	
			if (userDay <= AUG_L)
				correctLength = true;
			break;
		case SEP:	
			if (userDay <= SEP_L)
				correctLength = true;
			break;
		case OCT:	
			if (userDay <= OCT_L)
				correctLength = true;
			break;
		case NOV:	
			if (userDay <= NOV_L)
				correctLength = true;
			break;
		case DEC:	
			if (userDay <= DEC_L)
				correctLength = true;
			break;
	}

	return correctLength;
}

//----------------------------------------------------------------
// yearCheck(int)
// Pre-conditions	:Needs userYear
// Post-conditions	:Returns aceptYear as true if the 
// 			input is acceptable
//----------------------------------------------------------------

bool yearCheck(int userYear)
{
	// Variable Declarations
	const int minYear = 1900;
	const int maxYear = 2006;
	bool acceptYear = false;

	// Check if the year is between minYear and maxYear
	if (userYear >= minYear && userYear <= maxYear)
		acceptYear = true;

	return acceptYear;
}

//-------------------------------------------------------------
// leapYearCheck(int)
// Pre-conditions	:Needs userYear
// Post-conditions	:Returns bool true if it is a leap year
//-------------------------------------------------------------

bool leapYearCheck(int userYear)
{
	// Variable Declarations
	bool isLeapYear = false;
	
	// If devisable by 4 and not 100 it is a leap year
	if(userYear % 4 == 0 && userYear % 100 != 0)
		isLeapYear = true;
	
	// If devisable by 400, it is a leap year
	if (userYear % 400 == 0)
		isLeapYear = true;
	
	return isLeapYear;
}

//-------------------------------------------------------------
// datePrint(int, int, int)
// Pre-conditions	:Needs userMonth, userDay and userYear
// Post-conditions	:Outputs the date in plain text
//-------------------------------------------------------------

void datePrint(int userMonth, int userDay, int userYear)
{
	// Selects based on userMonth
	switch(userMonth)
	{
		case JAN:
			cout << "January";
			break;
		case FEB:
			cout << "Febuary";
			break;
		case MAR:
			cout << "March";
			break;
		case APR:
			cout << "April";
			break;
		case MAY:
			cout << "March";
			break;
		case JUN:
			cout << "June";
			break;
		case JUL:
			cout << "July";
			break;
		case AUG:
			cout << "August";
			break;
		case SEP:
			cout << "September";
			break;
		case OCT:
			cout << "October";
			break;
		case NOV:
			cout << "November";
			break;
		case DEC:
			cout << "December";
			break;
	}
	
	// Finish the print out of the entier date
	cout << " " << userDay << ", " << userYear;
}

//------------------------------------------------------------
// signPrint(int, int)
// Pre-conditions	:Needs userMonth and userDay
// Post-conditions	:Pints the name of the zodiac sign
//------------------------------------------------------------
void signPrint(int userMonth, int userDay)
{
	// Variable Declarations
	int sign = signReturn(userMonth, userDay);

	// Print out the sign based on the sign variable
	switch(sign)
	{
		case CAP:
			cout << "Capricorn";
			break;
		case AQU:
			cout << "Aquarius";
			break;
		case PIS:
			cout << "Pisces";
			break;
		case ARI:
			cout << "Aries";
			break;
		case TAU:
			cout << "Taurus";
			break;
		case GEM:
			cout << "Gemini";
			break;
		case CAN:
			cout << "Cancer";
			break;
		case LEO:
			cout << "Leo";
			break;
		case VIR:
			cout << "Virgo";
			break;
		case LIB:
			cout << "Libra";
			break;
		case SCO:
			cout << "Scorpio";
			break;
		case SAG:
			cout << "Sagittarius";
			break;
	}
}

//-------------------------------------------------------------------
// signReturn(int, int)
// Pre-conditions	:Sign global variables in header, and
// 			userMonth, userDay
// Post-conditions	:Returns an int with the value of the users 
// 			sign acording to the global variables
//-------------------------------------------------------------------

int signReturn(int userMonth, int userDay)
{
	// Variable Declarations
	int sign;
	
	// Find the users sign based on userMonth and userDay
	switch(userMonth)
	{
		case JAN:
			if (userDay < 20)
				sign = CAP;
			else
				sign = AQU;
			break;
		case FEB:
			if (userDay < 20)
				sign = AQU;
			else
				sign = PIS;
			break;
		case MAR:
			if (userDay < 21)
				sign = PIS;
			else
				sign = ARI;
			break;
		case APR:
			if (userDay < 21)
				sign = ARI;
			else
				sign = TAU;
			break;
		case MAY:
			if (userDay < 22)
				sign = TAU;
			else
				sign = GEM;
			break;
		case JUN:
			if (userDay < 22)
				sign = GEM;
			else
				sign = CAN;
			break;
		case JUL:
			if (userDay < 23)
				sign = CAN;
			else
				sign = LEO;
			break;
		case AUG:
			if (userDay < 22)
				sign = LEO;
			else
				sign = VIR;
			break;
		case SEP:
			if (userDay < 24)
				sign = VIR;
			else
				sign = LIB;
			break;
		case OCT:
			if (userDay < 24)
				sign = LIB;
			else
				sign = SCO;
			break;
		case NOV:
			if (userDay < 23)
				sign = SCO;
			else
				sign = SAG;
			break;
		case DEC:
			if (userDay < 23)
				sign = SAG;
			else
				sign = CAP;
			break;
	}
	return sign;
}
