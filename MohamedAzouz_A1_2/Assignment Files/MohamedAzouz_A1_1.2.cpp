// C++ Program for Huffman Coding using Priority Queue
#include <iostream>
#include <queue>
#include <stack>
#include <map>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

#define MAX_SIZE 100 // Maximum Height of Huffman Tree.
class HuffmanTreeNode {
public:

	char data; // Stores character
	int freq; // Stores frequency of the character
	HuffmanTreeNode* left; // Left child of the current node
	HuffmanTreeNode* right; // Right child of the current node

	HuffmanTreeNode(char character, int frequency) // Initializing the current node
	{
		data = character;
		freq = frequency;
		left = right = NULL;
	}
};

// Custom compare class
class Compare {
public:
	bool operator()(HuffmanTreeNode* a, HuffmanTreeNode* b)
	{   // Defining priority on the basis of frequency
		return a->freq > b->freq;
	}
};

// Function to generate Huffman Encoding Tree
HuffmanTreeNode* generateTree(priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, Compare> pq)
{
	// We keep on looping till only one node remains in the Priority Queue
	while (pq.size() != 1) {

		// Remove nodes ni, nj with lowest frequencies pi, pj from the Priority Queue
		HuffmanTreeNode* ni = pq.top();
		pq.pop();
		HuffmanTreeNode* nj = pq.top();
		pq.pop();


		// Create a new symbol (could be a anything, it won't be used) 
		// with the new frequency = pi+pj (we are only concerned with the frequency)
		char tempdata = ';';
		int tempfreq = ni->freq + nj->freq;

		// A new node is formed with the new symbol and frequency 
		HuffmanTreeNode* temp = new HuffmanTreeNode(tempdata, tempfreq);

		// set the left and right children of the newely formed node to be ni and nj 
		temp->left = ni; temp->right = nj;

		// Push back node created to the Priority Queue
		pq.push(temp);
	}
	// The Priority Queue should have one element: the entire Tree
	return pq.top();
}

// Function to print the huffman code for each character.
// It uses arr to store the codes
// Function to print the Huffman code for each character (non-recursive)
void printCodes(HuffmanTreeNode* root) {
	if (!root)
		return;

	// Create a map to store Huffman codes
	map<HuffmanTreeNode*, string> huffmanCodes;

	// Initialize the stack for iterative traversal
	stack<HuffmanTreeNode*> stk;
	stk.push(root);
	huffmanCodes[root] = ""; // Initialize root's code as empty

	while (!stk.empty()) {
		HuffmanTreeNode* curr = stk.top();
		stk.pop();

		// If it's a leaf node, print the character and its code
		if (!curr->left && !curr->right) {
			cout << curr->data << ": " << huffmanCodes[curr] << endl;
		}

		// Push left child (if exists) with updated code
		if (curr->left) {
			huffmanCodes[curr->left] = huffmanCodes[curr] + "0";
			stk.push(curr->left);
		}
		// Push right child (if exists) with updated code
		if (curr->right) {
			huffmanCodes[curr->right] = huffmanCodes[curr] + "1";
			stk.push(curr->right);
		}
	}
}

string encode(char c, HuffmanTreeNode* root)
{
	if (root == NULL) // returns an empty string if the character is not found
	{
		return "";
	}
	if (root->data == c) // returns a string with a single space to confirm the character is present
	{
		return " ";
	}
	string code = ""; // initialises code of the character as an empty string

	if (root->left != NULL) // checks if there is a left subtree
	{
		string left = ""; // initialises an empty string which stores the encoding of the character in the left subtree
		left = encode(c, root->left);
		if (left == " ") // if the character is found but there are no current characters in the string it is initialised with a zero since this is a left traversal
		{
			code = "0";
		}
		else if (left != "") // if there is already characters in the code then 0 is appended onto the beginning of the string
		{
			string t = "0";
			code = t.append(left);
		}
	}
	if (root->right != NULL)
	{
		string right = "";
		right = encode(c, root->right);
		if (right == " ")
		{
			code = "1"; // exact same as the left subtree code however uses 1 in stead of 0 since this is a right traversal
		}
		else if (right != "")
		{
			string t = "1";
			code = t.append(right);
		}
	}
	return code; // final encoding is returned
}

HuffmanTreeNode* HuffmanCodes(vector<char> data, vector<int> freq, int size)
{
	// Declaring priority queue using custom comparator
	priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, Compare> pq;

	// Populating the priority queue
	for (int i = 0; i < size; i++) {
		HuffmanTreeNode* newNode = new HuffmanTreeNode(data.at(i), freq.at(i));
		pq.push(newNode);
	}

	// Generate Huffman Encoding Tree and get the root node
	HuffmanTreeNode* root = generateTree(pq);

	// Print Huffman Codes
	int arr[MAX_SIZE], top = 0;
	printCodes(root);//, arr, top);
	return root;
}

void decode(string x, HuffmanTreeNode* root, HuffmanTreeNode* current)
{
	if (x == "") // if the string is empty or contains an invalid character then the function is terminated
	{
		cout << current->data;
		return;
	}
	else if (x.at(0) == '0') // checks the current character in the encoded message
	{
		if (current->left == NULL) // checks if this is a leaf node, if it is the current node's data is printed
		{
			cout << current->data;
			decode(x, root, root);
		}
		else // if it is not a leaf node then the first character is erased and the decode function is recursively called with the left subtree since this is the 0 digit
		{
			decode(x.substr(1), root, current->left);
		}
	}
	else if (x.at(0) == '1') // identical to the code for 0 but using the right subtree since this is a 1
	{
		if (current->right == NULL)
		{
			cout << current->data;
			decode(x, root, root);
		}
		else
		{
			decode(x.substr(1), root, current->right);
		}
	}
}

void createvectors(vector<char>& data, vector<int>& freq, char c) // function which takes data and frequency vectors and adds the character approrpiately
{
	bool flag = false; // flag to check whether the character is already in the vector
	if (!data.empty() || !freq.empty())
	{
		vector<char>::iterator iter = data.begin();
		vector<int>::iterator iter2 = freq.begin();
		for (iter; iter < data.end(); iter++) // loops over the data vector to find the element
		{
			if (*iter == c) // if the character is found, the frequency is updated
			{
				(*iter2)++;
				flag = true;
				break;
			}
			iter2++;
		}
	}
	if (!flag) // if the character is not found, a new character is added to the vector and the frequency is initialised as 1
	{
		data.push_back(c);
		freq.push_back(1);
	}
}

void read(string filename) // this function takes a file name and reads it
{
	vector<char> data;
	vector<int> freq;
	HuffmanTreeNode* root;
	ifstream inputFile(filename);
	char c;
	while (inputFile >> c) // data and frequency vectors are build with the text in the file (each char individually)
	{
		createvectors(data, freq, c);
	}
	inputFile.close();
	root = HuffmanCodes(data, freq, data.size()); // calls the huffman codes function to build the tree and encode the text
	ifstream ifile2(filename);
	string x;
	while (ifile2 >> c)
	{
		x.append(encode(c, root)); // finds the encoding of each individual character and stores it in string x
	}
	cout << x << endl; // prints out the encoding
	decode(x, root, root); // decodes and prints the encoded text
	ifile2.close();
}



// Driver Code
int main()
{
	HuffmanTreeNode* root;
	cout << "Enter 1 for text from file, 2 for user input (3 for test file)" << endl;
	int x;
	cin >> x;

	if (x == 1)
	{
		cout << "Enter file path: ";
		string file;
		cin >> file;
		read(file);
	}
	else if (x == 2)
	{
		cout << "Enter text in 1 line: ";
		string text;
		cin >> text;
		vector<char> data;
		vector<int> freq;
		for (int i = 0; i < text.size(); i++)
		{
			createvectors(data, freq, text.at(i)); // creates the data and frequency vectors using each character
		}
		root = HuffmanCodes(data, freq, data.size()); // calls the huffman codes function to build the tree and stores the root
		string x = "";
		for (int i = 0; i < text.size(); i++)
		{
			x.append(encode(text.at(i), root)); // stores the encoding of all the characters
		}
		cout << x << endl; // the encoded text is printed before being decoded 
		decode(x, root, root);
	}
	else if (x == 3)
	{
		read("C:\\Users\\moham\\source\\repos\\Analysis Lab 3\\Asn1.2Test.txt");
	}
	else
	{
		cout << "Invalid input";
	}
	return 0;
}