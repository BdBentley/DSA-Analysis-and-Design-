//============================================================================
// Name        : Source.cpp
// Author      : Brian Bentley 
// Version     : 1.0
// Copyright   : SNHU @ 2022 COCE
// Description : Code for Project 2, SNHU.
//============================================================================

#include <iostream>

#include <string>

#include <algorithm>

#include <vector>

#include <sstream>

#include <fstream>


using namespace std;


// This struct creates the Course, with course ID and course name as well as prereq.
struct Course {
    string courseId;
    string courseName;
    vector<string> preRequisite;
};

// Structure for the tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node() { // Default constructor
        left = nullptr;
        right = nullptr;
    }
    // Initialize with the course
    Node(Course aCourse) : Node() {
        this->course = aCourse;
    }

};

// Define a class containing data members and
// methods to implement a binary search tree
class BinarySearchTree {

private:

    void Destruct(Node* node);

public:
    Node* root;
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void Insert(BinarySearchTree* tree, Node* node);
    void Search(string courseId);
    void PrintCourse(Node* node);
};

// The default constructor
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
};

// The destructor
BinarySearchTree::~BinarySearchTree() {
    Destruct(root);
};

// WIll destructs left and right pointer nodes and deletes them
void BinarySearchTree::Destruct(Node* node) {
    if (node != nullptr) {
        Destruct(node->left);
        node->left = nullptr;
        Destruct(node->right);
        node->right = nullptr;
        delete node;
    }
};


// Search for a course
void BinarySearchTree::Search(string courseId) {
    Node* currentNode = root;

    while (currentNode != nullptr) {
        if (currentNode->course.courseId == courseId) {
            // prints course id, name
            cout << currentNode->course.courseId << ", ";
            cout << currentNode->course.courseName;
            cout << endl;
            cout << "Prerequisites: ";
            // prints any prerequisites, followed by , if there are more than 1
            for (string preRequisite : currentNode->course.preRequisite) {
                if (preRequisite == currentNode->course.preRequisite.back()) {

                    cout << preRequisite << endl;
                }
                else {
                    cout << preRequisite << ", ";
                }
            }

            return;
        }
        // Searches left pointer if not found
        else if (courseId < currentNode->course.courseId) {

            if (currentNode->left != nullptr) {
                currentNode = currentNode->left;
            }
        }
        // searches right pointer if not found
        else {

            currentNode = currentNode->right;
        }
    }
    // Course is not in the list
    cout << "Course " << courseId << "not found. " << endl;
    return;
}

// Inserts course into a course list
void BinarySearchTree::Insert(BinarySearchTree* tree, Node* node) {

    if (tree->root == nullptr) {
        tree->root = node;
    }
    else {
        Node* curr = tree->root;
        while (curr != nullptr) {

            if (node->course.courseId < curr->course.courseId) {
                if (curr->left == nullptr) {
                    curr->left = node;
                    curr = nullptr;
                }
                else {
                    curr = curr->left;
                }
            }
            else {

                if (curr->right == nullptr) {
                    curr->right = node;
                    curr = nullptr;
                }
                else {
                    curr = curr->right;
                }
            }

        }

    }
}
// Prints course list
void BinarySearchTree::PrintCourse(Node* node) {

    // Traverse and print entire tree
    if (node == nullptr) {
        return;
    }

    // Looks at left node, prints course id, course name, then look at next node
    PrintCourse(node->left);
    cout << node->course.courseId << ", ";
    cout << node->course.courseName << endl;
    PrintCourse(node->right);
};

// Loads file and creates course list
void loadCourse(string filename, BinarySearchTree* bst) {
    ifstream file(filename);
    if (file.is_open()) {
        cout << "File loaded." << endl;

        int num;
        string line;
        string word;

        while (getline(file, line)) {

            num = 0;
            Node* node = new Node();
            stringstream str(line);

            while (num < 2) {
                getline(str, word, ',');
                if (num == 0) {
                    node->course.courseId = word;
                }
                else {
                    node->course.courseName = word;
                }
                num++;
            }
            while (getline(str, word, ',')) {
                node->course.preRequisite.push_back(word);
            }

            // Inserts node into bst
            bst->Insert(bst, node);
        }
    }
    // File error if file not found
    else {
        cout << "File error, please try again. " << endl;
        return;
    }

}

// This function displays the programs menu
void DisplayMenu() {
    cout << "1. Load Data Structure" << endl;
    cout << "2. Print Course List" << endl;
    cout << "3. Print Course" << endl;
    cout << "9. Exit" << endl << endl;
    cout << "What would you like to do? ";
}

void main() {

    BinarySearchTree* bst = new BinarySearchTree();

    string fin;
    string courseChoice;

    int userInput = 0;

    cout << "Welcome to the course planner." << endl << endl;
    // Main while loop to keep program open while choice is not 9
    while (userInput != 9) {
        DisplayMenu();
        cin >> userInput;

        switch (userInput) {
            // Loads the ABCUCourses.csv file
        case 1:
            cout << endl;
            fin = "ABCUCourses.csv";
            loadCourse(fin, bst);
            cout << endl;
            break;

            // Prints courses in alphanumeric order
        case 2:
            cout << endl;
            cout << "Here is a sample schedule: " << endl << endl;
            // Prints course starting at root   	
            bst->PrintCourse(bst->root);
            cout << endl;
            break;

            //prints course that user chooses and any prerequisites
        case 3:
            cout << endl;
            cout << "What course do you want to know about? ";
            cin >> courseChoice;
            cout << endl;

            std::transform(courseChoice.begin(), courseChoice.end(), courseChoice.begin(), ::toupper);
            bst->Search(courseChoice);

            cout << endl;
            break;

            // Exits the program
        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

            // Default catch statement to let user know option is not valid.
        default:
            cout << userInput << " is not a valid option." << endl << endl;
            break;
        }
    }
}