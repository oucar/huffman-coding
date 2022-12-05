#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
// swap()
#include <algorithm>

// global vectors to store characters and codes
std::vector<std::string> characters;
std::vector<std::string> codes;

// binary tree
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

    // node is leaf node
    if ((root->left == NULL) && (root->right == NULL))
    {
        characters.push_back(root->character);
        codes.push_back(str);
        return;
    }

    // check for left-leaves recursively
    if (root->left != NULL)
    {
        str.append("0");
        printLeafNodes(root->left, str);
    }

    // remove the last element before starting to right-side branch
    str.pop_back();

    // check for right-leaves recursively
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

    int pos = 0;
    while (getline(inputFile, tempStr))
    {
        pos = tempStr.find(space);
        charvec.push_back(tempStr.substr(0, pos));
        freqvec.push_back(stoi(tempStr.substr(pos + 1)));
    }
    inputFile.close();

    // print given input letters
    std::cout << "Input from 'freq.txt': " << std::endl;
    for (int i = 0; i < charvec.size(); i++) std::cout << std::setw(2) << charvec[i] << " ";

    std::cout << std::endl;

    // print given input letter frequencies
    for (int i = 0; i < freqvec.size(); i++) std::cout << std::setw(2) << freqvec[i] << " ";

    std::cout << std::endl;
    std::cout << std::endl;

    // sorts the frequency and character vectors
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

    // prints out the input from the file in sorted order
    std::cout << "Sorted Input: " << std::endl;
    for (int i = 0; i < charvec.size(); i++) std::cout << std::setw(2) << charvec[i] << " ";
    
    std::cout << std::endl;

    for (int i = 0; i < freqvec.size(); i++) std::cout << std::setw(2) << freqvec[i] << " ";
    
    std::cout << std::endl;
    std::cout << std::endl;

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

        for (int k = 0; k < Nodes.size(); k++) std::cout << std::setw(2) << Nodes[k]->data << " ";
        
        std::cout << std::endl;

        // temprorary node
        Node *temp = new Node(Nodes[0]->data + Nodes[1]->data);
        temp->left = Nodes[0];
        temp->right = Nodes[1];
        // remove 0 and 1 
        Nodes.erase(Nodes.begin());
        Nodes.erase(Nodes.begin());

        // list is empty
        if (Nodes.size() == 0)
        {
            Nodes.push_back(temp);
            break;
        }

        // insert temproray node into the correct position in Nodes list
        for (int j = 0; j < Nodes.size(); j++)
        {
            if (Nodes[j]->data > temp->data)
            {
                Nodes.insert((Nodes.begin() + j), temp);
                break;
            }

            if (j == Nodes.size() - 1)
            {
                Nodes.push_back(temp);
                break;
            }
        }
    }

    for (int k = 0; k < Nodes.size(); k++) std::cout << std::setw(2) << Nodes[k]->data << " ";
    
    std::cout << std::endl;

    printLeafNodes(Nodes[0], "");
    std::cout << std::endl;


    // bubble sort for the characters and their codes
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

    // outputting 
    std::ofstream outputFile("codetable.txt");
    if (!outputFile)
    {
        std::cerr << "Error: file could not be opened" << std::endl;
        exit(1);
    }

    // final print of the characters & codes
    // (same as codetable.txt)
    std::cout << "Character   |   Code" << std::endl;
    for (int i = 0; i < characters.size(); i++)
    {
        std::cout << std::setw(2) << "    " << characters[i] << "       |   " << codes[i] << std::endl;
        outputFile << characters[i] << " " << codes[i] << std::endl;
    }
    outputFile.close();
}