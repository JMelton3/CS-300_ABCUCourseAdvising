//============================================================================
// Name        : Project 2 ABCU Course Sorting
// Author      : John Melton
// Date        : 4/16/2023
//============================================================================

//#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;


struct Course {
    string courseId; // unique identifier
    string title;
    vector<string> preReqs;
};

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};




class BinarySearchTree {

private:
    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    void postOrder(Node* node);
    void preOrder(Node* node);

public:
    Node* root;
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    void Print(Node* node);
    void Print(Course);
    Course Search(string courseId);
    
};

// Constructor
BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr;
}

/**
/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {}

void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root
    inOrder(root);
}


Course BinarySearchTree::Search(string courseId) {
    // set current node equal to root
    Node* currNode = root;

    // keep looping downwards until bottom reached or matching courseId found
        // We do not need to check if the courseId is found in the while loop since if course is found it will return and exit the loop. -JM  
    while (currNode != NULL) {
        // if match found, return current course
        if (currNode->course.courseId == courseId) { return currNode->course; }

        // if course is smaller than current node then traverse left
        if (courseId < currNode->course.courseId) {
            currNode = currNode->left;
        }
        else {
            // else larger so traverse right
            currNode = currNode->right;
        }
    }

    // If course isn't found return empty course.
    Course course;
    return course;
}

void BinarySearchTree::Insert(Course course) {
    // if root equal to null ptr
    if (root == nullptr) {
        // root is equal to new node course
        root = new Node(course);
    }
    else {
        // add Node root and course
        addNode(root, course);
    }
}

void BinarySearchTree::addNode(Node* node, Course course) {
    // if node is larger then add to left
    if (course.courseId < node->course.courseId) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        }
        else {
            // else recurse down the left node
            addNode(node->left, course);
        }
    }
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right        
            node->right = new Node(course);
        }
        else {
            // recurse down the left node
            addNode(node->right, course);
        }
    }
}

void BinarySearchTree::inOrder(Node* node) {
    // if node isn't nullptr 
    if (node != nullptr) {
        inOrder(node->left);
        Print(node);
        inOrder(node->right);
    }

}
void BinarySearchTree::postOrder(Node* node) {
    // if node isn't nullptr 
    if (node != nullptr) {
        postOrder(node->left);
        postOrder(node->right);
        Print(node);
    }
}

void BinarySearchTree::preOrder(Node* node) {  
    // if node isn't nullptr 
    if (node != nullptr) {
        Print(node);
        postOrder(node->left);
        postOrder(node->right);
    }
}

// used for printing data to screen
void BinarySearchTree::Print(Node* node) {
    if (node != nullptr) {
        cout << node->course.courseId << ", " << node->course.title << endl;
    }

}

// Used to print detailed information about a course (Including prereqs).
void BinarySearchTree::Print(Course course)
{
    cout << course.courseId << ", " << course.title << endl;
    
    // If course contians Prereqs we will build a string and then print it to the screen.
    if (course.preReqs.size() >= 1) {
        string returnString = "";
        //Building string
        for (int i = 0; i < course.preReqs.size(); i++)
        {
            // concat next prereq onto string
            returnString += course.preReqs[i];
            
            // If there are additional prereqs add a space and comma for readabillity.
            if (course.preReqs.size() > i+1) {
                returnString += ", ";
            }
        }
        cout << "Prerequisites: " << returnString << endl;
    }
}

void loadData(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    ifstream file(csvPath);
    string line;
    std::vector<Course> data;

    // loop to read rows of a CSV file
    while (getline(file,line)) {

        std::stringstream lineStream(line);
        std::string cell;
        std::vector<std::string> row;

        while (getline(lineStream, cell, ',')) {
            // get data into vector collection to work with 
            row.push_back(cell);
        }
        Course course;
        course.courseId = row[0];
        course.title = row[1];

        //cout << course.title << endl;

        // Check if there are more than 2 items in the second column IE
        if (row.size() >= 3) {
            // Read additional items from the second column into a vector
            vector<string> preReqs;
            
            for (unsigned int i = 2; i < row.size(); i++) {
                preReqs.push_back(row[i]);
            }
            // Assign the vector to the course object
            course.preReqs = preReqs;
        }
        // push this course to the end 
        bst->Insert(course);
    }
}

bool diplayMainMenu() {

    // !Important Used to input csv filepath on load courses
    string csvFP = "./classSampleInput.csv";

    // Define tree to hold all courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    //Place holder for checking user inputs
    int choice = 0;
    while (choice != 9) {
        string userInput = "";
        cout << "Menu:" << endl;
        cout << "  1. Load Data Structure. " << endl;
        cout << "  2. Print Course List. " << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> userInput;

        // type Checking and guard clause
        if (!std::isdigit(userInput[0]) || userInput.length() > 1) {
            cout << "Error: invalid input\n   Please enter a valid input to continue\n";
            continue;
        }
        // converting input to int for case 
        choice = stoi(userInput.substr(0,1));

        // used in search logic if needed
        string searchInput = "";
        Course searchCourse;

        switch (choice) {

        case 1:
            loadData(csvFP, bst);
            break;

        case 2:
            bst->InOrder();

            bst;
            break;

        case 3:
            cout << "What course do you want to know about ?" << endl;
            
            //getting input from user
            cin >> searchInput;

            // Call search logic
            searchCourse = bst->Search(searchInput);

            if (searchCourse.courseId == searchInput){
                bst->Print(searchCourse);
            }
            else {
                cout << "Unable to find a Course with a matching ID.\n" << "Please try again.\n";
            }

            break;

        case 9:
            return true;
            break;
        
        default:
            cout << "Error: invalid input\n   Please enter a valid input to continue\n";
            break;
        }
    }
    return false;
}


int main()
{
    cout << "Welcome to the course planner.\n";

    // Used for main loop.
    bool done = false;

    // Main loop function will return exit program bool when user selects end program
    while (done == false) { done = diplayMainMenu();}

    cout << "Thank you for using the course planner!\n";

    return 0;
}