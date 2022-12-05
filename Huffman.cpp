#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
// For swap()
#include <algorithm>

// Global vectors to store characters and codes
std::vector<std::string> characters;
std::vector<std::string> codes;

// Binary tree
// character -> character given in the freq.txt
// data -> frequency given next to the character value given in freq.txt
struct Node
{
    int data;
    std::string character;
    struct Node *left;
    struct Node *right;

    Node(int val)
    {
        data = val;
        left = NULL;
        right = NULL;
    }

    Node(std::string str)
    {
        character = str;
        left = NULL;
        right = NULL;
    }
};

// print leaves of the binary tree from left to right
void printLeafNodes(Node *root, std::string str)
{
    // node is null then we're done
    if (!root) return;

    // if node is leaf node, push it to global vectors
    if ((root->left == NULL) && (root->right == NULL))
    {
        characters.push_back(root->character);
        codes.push_back(str);
        return;
    }

    // if left child exists, check for leaf
    // recursively
    // 0 is appended for every left-move
    if (root->left != NULL)
    {
        str.append("0");
        printLeafNodes(root->left, str);
    }

    // Remove last element before going to right-side branch
    str.pop_back();

    // if right child exists, check for leaf
    // recursively
    // 1 is appended for every left-move
    if (root->right != NULL)
    {
        str.append("1");
        printLeafNodes(root->right, str);
    }
}

int main()
{

    std::fstream inputFile("freq.txt");
    std::string space = " ";
    std::string tempStr = "";
    std::vector<std::string> charvec;
    std::vector<int> freqvec;

    // Takes each line as input string, splits it into 2 disregrading a space between
    // the character and its frequency value
    // The character and frequency are both pushed into their relevant vectors
    int pos = 0;
    while (getline(inputFile, tempStr))
    {
        pos = tempStr.find(space);
        charvec.push_back(tempStr.substr(0, pos));
        freqvec.push_back(stoi(tempStr.substr(pos + 1)));
    }
    inputFile.close();

    // Print given input letters
    std::cout << "Input from 'freq.txt': " << std::endl;
    for (int i = 0; i < charvec.size(); i++) std::cout << std::setw(2) << charvec[i] << " ";

    std::cout << std::endl;

    // Print given input letter frequencies
    for (int i = 0; i < freqvec.size(); i++) std::cout << std::setw(2) << freqvec[i] << " ";

    std::cout << std::endl;
    std::cout << std::endl;

    // Sorts the frequency and character vectors at the same time
    // to preserve parallelism
    for (int i = 0; i < freqvec.size(); i++)
    {
        for (int j = 0; j < freqvec.size(); j++)
        {
            if (freqvec[i] < freqvec[j])
            {
                std::swap(freqvec[i], freqvec[j]);
                std::swap(charvec[i], charvec[j]);
            }
        }
    }

    // Prints out the input from the file in sorted order
    std::cout << "Sorted Input: " << std::endl;
    for (int i = 0; i < charvec.size(); i++)
    {
        std::cout << std::setw(2) << charvec[i] << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < freqvec.size(); i++)
    {
        std::cout << std::setw(2) << freqvec[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    // Fills up list of nodes with the sorted values of characters and frequencies
    std::vector<Node *> Nodes;
    for (int i = 0; i < freqvec.size(); i++)
    {
        Node *temp = new Node(freqvec[i]);
        temp->character = charvec[i];
        Nodes.push_back(temp);
    }

    std::cout << "Huffmann Tree Algorithm, step by step: " << std::endl;

    for (int i = 0; i < freqvec.size(); i++)
    {

        // Prints out each step of algorithm
        for (int k = 0; k < Nodes.size(); k++)
        {
            std::cout << std::setw(2) << Nodes[k]->data << " ";
        }
        std::cout << std::endl;

        // Create a node "temp" with frequency f(temp) = f(0) + f(1),
        // temp's children are 0 and 1
        Node *temp = new Node(Nodes[0]->data + Nodes[1]->data);

        temp->left = Nodes[0];
        temp->right = Nodes[1];

        // Remove nodes 0 and 1 from the list of Nodes
        Nodes.erase(Nodes.begin());
        Nodes.erase(Nodes.begin());

        // If list becomes empty, push last node into L and we're done
        if (Nodes.size() == 0)
        {
            Nodes.push_back(temp);
            break;
        }
        // Inserts temp node into correct position in Nodes list
        for (int j = 0; j < Nodes.size(); j++)
        {
            // If you find a node with a higher frequency in the list, place temp before it
            if (Nodes[j]->data > temp->data)
            {
                Nodes.insert((Nodes.begin() + j), temp);
                break;
            }
            // Insert into back of the list if greater than all elements
            if (j == Nodes.size() - 1)
            {
                Nodes.push_back(temp);
                break;
            }
        }
    }

    // Prints out last iteration of Nodes list, should only have one node left
    for (int k = 0; k < Nodes.size(); k++)
    {
        std::cout << std::setw(2) << Nodes[k]->data << " ";
    }
    std::cout << std::endl;

    // Prints out leaf nodes from left to right, keeping track of branches for encoding
    // Also adds characters and codes to global vectors
    printLeafNodes(Nodes[0], "");
    std::cout << std::endl;

    // Take global vectors of characters and their codes and order them according to their
    // ASCII value
    int min = 0;
    for (int i = 0; i < characters.size(); i++)
    {
        for (int j = i + 1; j < characters.size(); j++)
        {
            if (characters[j] < characters[i])
            {
                std::string tempChar = characters[i];
                std::string tempCode = codes[i];
                characters[i] = characters[j];
                characters[j] = tempChar;
                codes[i] = codes[j];
                codes[j] = tempCode;
            }
        }
    }

    // Open output file to write to
    std::ofstream outputFile("codetable.txt");
    if (!outputFile)
    {
        std::cerr << "Error: file could not be opened" << std::endl;
        exit(1);
    }

    // Print out ordered characters and codes
    // Also write characters and their codes to the output file
    std::cout << "Character   |   Code" << std::endl;
    for (int i = 0; i < characters.size(); i++)
    {
        std::cout << std::setw(2) << "    " << characters[i] << "       |   " << codes[i] << std::endl;
        outputFile << characters[i] << " " << codes[i] << std::endl;
    }
    outputFile.close();
}