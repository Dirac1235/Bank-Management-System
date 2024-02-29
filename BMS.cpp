#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include<windows.h>

using namespace std;

// BankAccount class
class BankAccount {
public:
    string accountNumber;
    string accountHolderName;
    double balance;

    BankAccount(string number, string name, double initialBalance) {
        accountNumber = number;
        accountHolderName = name;
        balance = initialBalance;
    }
};

// Node class
class Node {
public:
    BankAccount* account;
    Node* left;
    Node* right;

    Node(BankAccount* acc) {
        account = acc;
        left = NULL;
        right = NULL;
    }
};

// Binary Tree class
class BinaryTree {
public:
    Node* root;

    BinaryTree() {
        root = NULL;
    }

    void insertNode(BankAccount* account) {
        Node* newNode = new Node(account);
        if (root == NULL) {
            root = newNode;
        }
        else {
            Node* current = root;
            Node* parent;
            while (true) {
                parent = current;
                if (account->accountNumber < current->account->accountNumber) {
                    current = current->left;
                    if (current == NULL) {
                        parent->left = newNode;
                        return;
                    }
                }
                else {
                    current = current->right;
                    if (current == NULL) {
                        parent->right = newNode;
                        return;
                    }
                }
            }
        }
    }
 	void inorderTraversal(Node* node, ofstream& file) {
        if (node == NULL) {
            return;
        }

        inorderTraversal(node->left, file);
        file << node->account->accountNumber << ",";
        file << node->account->accountHolderName << ",";
        file << node->account->balance << endl;
        inorderTraversal(node->right, file);
    }
    void inorderTraversal(ofstream& file) {
        inorderTraversal(root, file);
    }
    Node* searchNode(string accountNumber) {
        Node* current = root;
        while (current != NULL) {
            if (current->account->accountNumber == accountNumber) {
                return current;
            }
            if (accountNumber < current->account->accountNumber) {
                current = current->left;
            }
            else {
                current = current->right;
			}
		}
		return NULL;
	}
	void removeNode(string accountNumber) {
    root = removeNodeHelper(root, accountNumber);
}

Node* removeNodeHelper(Node* node, string accountNumber) {
    if (node == NULL) {
        return node;
    }

    if (accountNumber < node->account->accountNumber) {
        node->left = removeNodeHelper(node->left, accountNumber);
    }
    else if (accountNumber > node->account->accountNumber) {
        node->right = removeNodeHelper(node->right, accountNumber);
    }
    else {
        if (node->left == NULL) {
            Node* temp = node->right;
            delete node;
            return temp;
        }
        else if (node->right == NULL) {
            Node* temp = node->left;
            delete node;
            return temp;
        }
		
        Node* temp = findMinNode(node->right);
        if (temp=NULL){
        	temp= findMaxNode(node->right);
		}
        node->account = temp->account;
    }
    return node;
}

Node* findMinNode(Node* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}
Node* findMaxNode(Node* node){
	while(node->right!= NULL){
		node=node->right;
	}
	return node;
}

void displayBalance(string accountNumber) {
    Node* node = searchNode(accountNumber);
    if (node != NULL) {
        cout << "Account Number: " << node->account->accountNumber << endl;
        cout << "Account Holder: " << node->account->accountHolderName << endl;
        cout << "Balance: " << node->account->balance << endl;
    }
    else {
        cout << "Account not found." << endl;
    }
}
};
class BankAccountManagementSystem {
private:
    BinaryTree binaryTree;
public:
    BankAccountManagementSystem() {
        binaryTree = BinaryTree();
        loadAccountsFromFile();
    }

    void loadAccountsFromFile() {
        ifstream file("accounts.txt");
       
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                string accountNumber, accountHolderName;
                double balance;
                getline(ss, accountNumber, ',');
                getline(ss, accountHolderName, ',');
                ss >> balance;
                BankAccount* account = new BankAccount(accountNumber, accountHolderName, balance);
                binaryTree.insertNode(account);
            }
            file.close();
            cout << "Accounts loaded from file successfully." << endl;
        } else {
            cout << "Error: Unable to open file." << endl;
        }
    }

    void saveAccountsToFile() {
        ofstream file("accounts.txt");
            if (file.is_open()){
			
            binaryTree.inorderTraversal(file);
            file.close();
        }
         else {
            cout << "Error: Unable to open file." << endl;
        }
    }

    void createAccount() {
        string accountNumber, accountHolderName;
        double initialBalance;
        system("cls");
        cout << "Enter Account Number: ";
        cin >> accountNumber;
        cout << "Enter Account Holder Name: ";
        cin.ignore();
        getline(cin, accountHolderName);
        cout << "Enter Initial Balance: ";
        cin >> initialBalance;

        BankAccount* account = new BankAccount(accountNumber, accountHolderName, initialBalance);
        binaryTree.insertNode(account);

        cout << "Account created successfully." << endl;
    }

    void deposit() {
    	system("cls");
        string accountNumber;
        double amount;
        cout << "Enter Account Number: ";
        cin >> accountNumber;
        cout << "Enter Amount to Deposit: ";
        cin >> amount;
         Node* node= binaryTree.searchNode(accountNumber);
        if (node != NULL) {
            node->account->balance += amount;
            cout << "Deposit successful. New balance: " << node->account->balance << endl;
        } else {
            cout << "Account not found." << endl;
        }
    }

    void withdraw() {
        string accountNumber;
        double amount;
        system("cls");
        cout << "Enter Account Number: ";
        cin >> accountNumber;
        cout << "Enter Amount to Withdraw: ";
        cin >> amount;
        Node* node=binaryTree.searchNode(accountNumber);
        if (node != NULL) {
            if (node->account->balance >= amount) {
                node->account->balance -= amount;
                cout << "Withdrawal successful. New balance: " << node->account->balance << endl;
            } else {
                cout << "Insufficient balance." << endl;
            }
        } else {
            cout << "Account not found." << endl;
        }
    }

    void displayBalance() {
        string accountNumber;
        system("cls");
        cout << "Enter Account Number: ";
        cin >> accountNumber;
         Node* node=binaryTree.searchNode(accountNumber);
        if (node != NULL) {
            cout << "Account Number: " << node->account->accountNumber << endl;
			cout << "Account Holder: " << node->account->accountHolderName << endl;
			cout << "Balance: " << node->account->balance << endl;
		} else {
			cout << "Account not found." << endl;
	}
}
	void removeAccount() {
		system("cls");
    	string accountNumber;
    	cout << "Enter Account Number: ";
    	cin >> accountNumber;

    	binaryTree.removeNode(accountNumber);

    	cout << "Account removed successfully." << endl;
}

void displayMenu() {
    int choice;
    do {
        cout << "\nBank Account Management System" << endl;
        cout << "-----------------------------" << endl;
        cout << "1. Create Account" << endl;
        cout << "2. Deposit" << endl;
        cout << "3. Withdraw" << endl;
        cout << "4. Display Balance" << endl;
        cout << "5. Remove Account" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice (1-6): ";
        cin >> choice;

        switch (choice) {
            case 1:
            	createAccount();
                saveAccountsToFile();
                break;
            case 2:
                deposit();
                break;
            case 3:
            	withdraw();
                break;
            case 4:
                displayBalance();
                break;
            case 5:
            	removeAccount();
            	saveAccountsToFile();
                break;
            case 6:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 6);
}
};
int main() {
    BankAccountManagementSystem system;
    system.displayMenu();

    return 0;
}
