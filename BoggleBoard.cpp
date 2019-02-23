/* prog5Boggle.cpp

Program: #5, Boggle
Author: Ruchit Patel
TA: Moumita Samanta, Tues 9-9:50
Nov 20, 2017

Welcome to the game of Boggle, where you play against the clock
to see how many words you can find using adjacent letters on the
board.  Each letter can be used only once for a given word.


The dictionary total number of words is: 263533
Number of words of the right length is:  259709

*/


#define _CRT_SECURE_NO_WARNINGS
#include <iostream>   // For Input and Output
#include<cstdlib>
#include <fstream>    // For file input and output
#include <cassert>    // For the assert statement
#include <cctype>     // Allows using the tolower() function
#include <ctime>      // Timer functions
#include <cstring>    // For strlen
using namespace std;

// Global constants
const char DictionaryFileName[] = "dictionary.txt";
const int MaxNumberOfWords = 263533; // Number of dictionary words
const int MinWordLength = 3;         // Minimum dictionary word size to be stored
const int MaxWordLength = 16;	     // Max word size.  Add 1 for null
const int MaxUserInputLength = 81;   // Max user input length
const int NumberOfLetters = 26;      // Letters in the alphabet
const int TotalSecondsToPlay = 60;   // Total number of seconds to play per board
int scoreCounter = 0;                // Score counter




 //--------------------------------------------------------------------------------
 // Display name and program information
void displayIdentifyingInformation()
{
	printf("\n");
	printf("Author: Ruchit Patel          \n");
	printf("Program: #5, Boggle        \n");
	printf("TA: Moumita Samanta, Tues 9-9:50  \n");
	printf("Nov 20, 2017                \n");
	printf("\n");
}//end displayIdentifyingInformation()


 //--------------------------------------------------------------------------------
 // Display instructions
void displayInstructions()
{
	printf("Welcome to the game of Boggle, where you play against the clock   \n");
	printf("to see how many words you can find using adjacent letters on the  \n");
	printf("board.  Each letter can be used only once for a given word.       \n");
	printf("  \n");
	printf("When prompted to provide input you may also:                      \n");
	printf("     Enter 'r' to reset the board to user-defined values.         \n");
	printf("     Enter 's' to solve the board and display all possible words. \n");
	printf("     Enter 't' to toggle the timer on/off.                        \n");
	printf("     Enter 'x' to exit the program.                               \n");
	printf("  \n");
}//end displayInstructions()






 //---------------------------------------------------------------------------
 // Read in dictionary
 //    First dynamically allocate space for the dictionary.  Then read in words
 // from file.  Note that the '&' is needed so that the new array address is
 // passed back as a reference parameter.
void readInDictionary(
	char ** &dictionary,                      // dictionary words
	long int &numberOfWords)                  // number of words stored
{
	// Allocate space for large array of C-style strings
	dictionary = new char*[MaxNumberOfWords];

	// For each array entry, allocate space for the word (C-string) to be stored there
	for (int i = 0; i < MaxNumberOfWords; i++) {
		dictionary[i] = new char[MaxWordLength + 1];
		// just to be safe, initialize C-strings to all null characters
		for (int j = 0; j < MaxWordLength; j++) {
			dictionary[i][j] = '\0';
		}//end for (int j=0...
	}//end for (int i...

	 // Now read in the words from the file
	ifstream inStream;                 // declare an input stream for my use
	numberOfWords = 0;                   // Row for the current word
	inStream.open(DictionaryFileName);
	assert(!inStream.fail());       // make sure file open was OK

									// Keep repeating while input from the file yields a word
	char theWord[81];    // declare input space to be clearly larger than largest word
	while (inStream >> theWord) {
		int wordLength = (int)strlen(theWord);
		if (wordLength >= MinWordLength && wordLength <= MaxWordLength) {
			strcpy(dictionary[numberOfWords], theWord);
			// increment number of words
			numberOfWords++;
		}
	}//end while( inStream...

	cout << "The dictionary total number of words is: " << MaxNumberOfWords << endl;
	cout << "Number of words of the right length is:  " << numberOfWords << endl;

	// close the file
	inStream.close();
}//end readInDictionary()


 //--------------------------------------------------------------------------------------
 // Use binary search to look up the search word in the dictionary array, returning index
 // if found, -1 otherwise
int binarySearch(const char searchWord[MaxWordLength + 1], // word to be looked up
	char **dictionary)               // the dictionary of words
{
	int low, mid, high;     // array indices for binary search
	int searchResult = -1;  // Stores index of word if search succeeded, else -1

							// Binary search for word
	low = 0;
	high = MaxNumberOfWords - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		// searchResult negative value means word is to the left, positive value means
		// word is to the right, value of 0 means word was found
		searchResult = strcmp(searchWord, dictionary[mid]);
		if (searchResult == 0) {
			// Word IS in dictionary, so return the index where the word was found
			return mid;
		}
		else if (searchResult < 0) {
			high = mid - 1; // word should be located prior to mid location
		}
		else {
			low = mid + 1; // word should be located after mid location
		}
	}

	// Word was not found
	return -1;
}//end binarySearch()


 //---------------------------------------------------------------------------
 // Get random character
 //    Find random character using a table of letter frequency counts.
 // Iterate through the array and find the first position where the random number is
 // less than the value stored.  The resulting index position corresponds to the
 // letter to be generated (0='a', 1='b', etc.)
char getRandomCharacter()
{
	// The following table of values came from the frequency distribution of letters in the dictionary
	float letterPercentTotals[NumberOfLetters] = {
		0.07680,  //  a
		0.09485,  //  b
		0.13527,  //  c
		0.16824,  //  d
		0.28129,  //  e
		0.29299,  //  f
		0.32033,  //  g
		0.34499,  //  h
		0.43625,  //  i
		0.43783,  //  j
		0.44627,  //  k
		0.49865,  //  l
		0.52743,  //  m
		0.59567,  //  n
		0.66222,  //  o
		0.69246,  //  p
		0.69246,  //  q
		0.76380,  //  r
		0.86042,  //  s
		0.92666,  //  t
		0.95963,  //  u
		0.96892,  //  v
		0.97616,  //  w
		0.97892,  //  x
		0.99510,  //  y
		1.00000 }; //  z

				   // generate a random number between 0..1
				   // Multiply by 1.0 otherwise integer division truncates remainders
	float randomNumber = 1.0 * rand() / RAND_MAX;

	// Find the first position where our random number is less than the
	// value stored.  The index corresponds to the letter to be returned,
	// where 'a' is 0, 'b' is 1, and so on.
	for (int i = 0; i<NumberOfLetters; i++) {
		if (randomNumber < letterPercentTotals[i]) {
			// we found the spot.  Return the corresponding letter
			return (char) 'a' + i;
		}
	}

	// Sanity check
	cout << "No alphabetic character generated.  This should not have happened. Exiting program.\n";
	exit(-1);
	return ' ';   // should never get this
}//end getRandomCharacter



// class contain functions
class boggleboard
{
public:

	char board[36];                    // declare array for board

	// initialize array values
	boggleboard()
	{
		for (int i = 0; i < 36; i++)
		{
			board[i] = '*';                // set all values to special character
		}
		for (int j = 1; j < 5; j++)
		{
			for (int k = 1; k < 5; k++)
			{
				board[j * 6 + k] = getRandomCharacter();           // call function for random character
			}
		}
	}

	// Display board only random character
	void boarddisplay()
	{
		for (int j = 1; j < 5; j++)
		{
			for (int k = 1; k < 5; k++)
			{
				
				cout << board[j * 6 + k] << " ";   // Only print random character

			}
			cout << endl;

		}
	}

	// Function that find word in board
	bool findword(const char word[MaxWordLength + 1]) const
	{
		bool lookedChar[36];
		for (int l = 0; l < 36; l++)
		{
			if (board[l] == word[0])
			{
				for (int b = 0; b < 36; b++)
				{
					lookedChar[b] = false;            // return false if word not found

				}

				// Call function of recurson
				if (checkSecond(word, l, 1, lookedChar))
				{
					return true;                     // return if value is true
				}
				return false;
			}

		}
		return false;
	}
	
	
	// Function of recurson that check every character on board       // board index   index2 index of word   checking array
	bool checkSecond(const char word[MaxWordLength + 1], int index, int index2, bool lookedChar[]) const
	{

		static int moves[8] = { -7,-6,-5,-1,1,5,6,7 };          //moves to check of adjacent on each character   
		if (index2 == strlen(word))
		{
			return true;
		}
			lookedChar[index] = true;
			for (int l = 0; l < 8; l++)                           // Look to each side of character for next character
			{
				if (board[index + moves[l]] == word[index2])
				{
					if (!lookedChar[index + moves[l]])
					{
						if (checkSecond(word, index + moves[l], index2 + 1, lookedChar))
						{
							return true;                        // return true when all character found on board
						}
					}
				}
			}
		lookedChar[index] = false;
		return false;
	}


	// Function of reset board
	void resetBoard();

	// Function of display all possible words
	void displayAllwords(int minWords, int maxWords, char **dictionary);
	
private:

};


// Function of when user enter 's' print values between minimum to maximum
void boggleboard::displayAllwords(int minWords, int maxWords, char **dictionary)
{
	// Loop find words between min words and max words
	for (int i = minWords; i <= maxWords; i++)
	{
		for (int j = 0; j < MaxNumberOfWords; j++)
		{
			if (strlen(dictionary[j]) == i && findword(dictionary[j]))
			{
				cout << dictionary[j] << " ";                  // print words between given limits
			}
		}
	}

}// end displayAllwords



//Reset board function user can enter their value
void boggleboard::resetBoard()
{
		for (int i = 0; i < 36; i++)
		{
			board[i] = '*';

		}
		for (int j = 1; j < 5; j++)
		{
			for (int k = 1; k < 5; k++)
			{
				cin >> board[j * 6 + k];             // Enter values of when reset board

			}

		}

}// end resetBoard




//---------------------------------------------------------------------------
int main()
{
	// declare variables
	char **dictionary;                // 2d array of dictionary words, dynamically allocated
	long int numberOfWords;           // how many words actually found in dictionary
	char userInput[MaxUserInputLength];
	int scoreCounter = 0;            // Score counter
	int minWords, maxWords;          // variables to word lengths
	
	bool keepWord[MaxNumberOfWords] = { false };  // bool function to hold words
	bool toggle = false;              // Toggle

	// Declare a variable to hold a time, and get the current time
	time_t startTime = time(NULL);


	displayIdentifyingInformation();  // Call function of Id information
	displayInstructions();            // Call function of Instruction

	
	srand(time(0));                   //in built function of change latters everytime

	// Declare object with class
	boggleboard display;
	
	// read in dictionary.  numberOfWords returns the actual number of words found
	readInDictionary(dictionary, numberOfWords);

	
	cout << endl << endl;
	
	
	int elapsedSeconds = 0;   // Declare variable for time

	// while loop for play game
	while (toggle || elapsedSeconds < TotalSecondsToPlay)
	{
		if (!toggle)
		{
			elapsedSeconds = difftime(time(NULL), startTime);
			cout << "There are " << TotalSecondsToPlay - elapsedSeconds << " seconds left." << endl;

		}
		cout << endl;
		
		display.boarddisplay();                         // call function to displayboard
		cout << "Score: " << scoreCounter << endl;      // print score

		// Prompt for and get user input
		cout << endl;
		cout << "Enter a word: ";
		cin >> userInput;                             // Userinput word

		
		// If statement when user press 'x'
		if ((userInput[0] == 'x') && (strlen(userInput) == 1))
		{
			cout << " Existing the game" << endl;
			exit (-1);
		}

		//// If statement when user press 'r'
		else if ((userInput[0] == 'r') && (strlen(userInput) == 1))
		{
			cout << "Enter 16 characters to be used to set the board: ";
			display.resetBoard();
			
		}

		// If statement when user press 't'
		else if ((userInput[0] == 't') && (strlen(userInput) == 1))
		{
			// If toggle ON
			if (!toggle)
			{
				toggle = true;
				cout << "toggle is OFF" << endl;
			}

			// toggle OFF
			else {
				toggle = false;
				cout << "toggle is ON"  << endl;
			}
		}

		// If statement when user press 's'
		else if ((userInput[0] == 's') && (strlen(userInput) == 1))
		{
			cout << "Enter min and max word lengths to display : " << endl;

			cin >> minWords;                // Get minimum length
			cin >> maxWords;                // Get maximum length

			// Call function from class to display all word
			display.displayAllwords(minWords, maxWords, dictionary);
			
			return 0;
		}

		
		// If function to check word in dictionary
		else if (binarySearch(userInput, dictionary) != -1)                     // Userinput check in dictionary
		{
			
			// If function to check word on board
			if (display.findword(userInput))                              // Function of user check in board
			{ 
				// Bool function to hold words found on board
				if (keepWord[binarySearch(userInput, dictionary)])
				{
					cout << "Sorry, that word was already previously found." << endl;
				}
				else
				{
					keepWord[binarySearch(userInput, dictionary)] = true;
					cout << "Words found so far are: ";
					for (int i = 0; i < MaxNumberOfWords; i++)
					{

						if (keepWord[i] == true)
						{
							cout << dictionary[i] << " ";        //print found word on board
						}
					}

					// Giving score regarding word length
					if (strlen(userInput) == 3)                   // when word length 3
					{
						scoreCounter = 1 + scoreCounter;
					}
					else if (strlen(userInput) == 4)             // when word length 4
					{
						scoreCounter = 2 + scoreCounter;

					}
					else if (strlen(userInput) == 5)            // when word length 5
					{
						scoreCounter = 4 + scoreCounter;
					}
					else if (strlen(userInput) > 5)             // when word length > 5
					{
						scoreCounter = strlen(userInput) + scoreCounter;
					}

				}
			} // end if word on board

			// Print error when word not on board
			else
			{
				cout << "cannot be formed on this board." << endl;
			}
		} // end if binary search

		  // Print error when word not in dictionary
		else {
			cout << userInput << " is NOT in the dictionary." << endl;
		}
		
		
		// Calculate how many seconds have elapsed since we started the timer.
		
	}
	cout << "I let you finish your last move. Time is up!" << endl;


	return 0;
}//end main()
