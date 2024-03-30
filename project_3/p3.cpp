#include <iostream>
#include <string>

#include "p3.h"

using namespace std;

Info::Info() {
    this->name = "No Name Set";
    this->value = "No Value Set";
    this->next = nullptr;
}

Info::Info(std::string name, std::string value, Info *next) {
    this->name = name;
    this->value = value;
    this->next = next;
}

Contact::Contact() {
    this->first = "No First Set";
    this->last = "No Last Set";
    this->next = nullptr;
    this->headInfoList = nullptr;
}

Contact::Contact(std::string first, std::string last, Contact *next) {
    this->first = first;
    this->last = last;
    this->next = next;
    this->headInfoList = nullptr;
}

ContactList::ContactList() {
    this->headContactList = nullptr;
    this->count = 0;
}

int ContactList::getCount() {
    return this->count;
}

// print the specified contact and its information
// 1. return false and print nothing if the contact is not in the list
// 2. otherwise return true and print the contact
bool ContactList::printContact(std::ostream &os, std::string first, std::string last) {
   Contact *temp = this->headContactList;
    while (temp != nullptr) {
        if (temp->first == first && temp->last == last) {
            // Contact found, print the contact
           std::cout << "Contact Name: " << temp->first << " " << temp->last << std::endl;
            
            // Print the contact's information
            Info *infoTemp = temp->headInfoList;
            while (infoTemp != nullptr) {
               std::cout << "\t" << infoTemp->name << " | " << infoTemp->value << std::endl;
                infoTemp = infoTemp->next;
            }
            return true;
        }
        temp = temp->next;
    }

    // Contact not found
    return false;
}

// print all contacts and their information
// print nothing if the list is empty
void ContactList::print(std::ostream &os) {
     Contact *temp = this->headContactList;
    while (temp != nullptr) {
        // Print the contact name
        std::cout << "Contact Name: " << temp->first << " " << temp->last << std::endl;

        // Print the contact's info list
        Info *infoTemp = temp->headInfoList;
        while (infoTemp != nullptr) {
            std::cout << "\t" << infoTemp->name << " | " << infoTemp->value << std::endl;
            infoTemp = infoTemp->next;
        }

        temp = temp->next;
    }
}

// add a contact to the back of the list
// 1. return false and do nothing if the contact is already in the list
// 2. otherwise return true and add the contact to the back of the list
// - do not forget to update count
bool ContactList::addContact(std::string first, std::string last) {
       // Check if the contact already exists in the list
    Contact *temp = this->headContactList;
    while (temp != nullptr) {
        if (temp->first == first && temp->last == last) {
            return false; // Contact already exists
        }
        temp = temp->next;
    }

    // If the contact does not exist, add it to the end of the list
    Contact *newContact = new Contact(first, last, nullptr);
    if (this->headContactList == nullptr) {
        // If the list is empty, make the new contact the head of the list
        this->headContactList = newContact;
    } else {
        // If the list is not empty, traverse to the end and add the new contact
        temp = this->headContactList;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newContact;
    }

    // Increase the count of contacts
    this->count++;

    return true;
}

// add info to the back of a contact's info list
// 1. return false and do nothing if the contact is not in the list
// 2. if the infoName is already in the info list, update the infoValue and return true
// 3. otherwise add the info to the back of the contact's list and return true
bool ContactList::addInfo(std::string first, std::string last, std::string infoName, std::string infoVal) {
// Check if the contact exists in the list
    Contact *temp = this->headContactList;
    while (temp != nullptr) {
        if (temp->first == first && temp->last == last) {
            // Contact found, check if the infoName already exists in the info list
            Info *infoTemp = temp->headInfoList;
            while (infoTemp != nullptr) {
                if (infoTemp->name == infoName) {
                    // InfoName found, update the infoValue and return true
                    infoTemp->value = infoVal;
                    return true;
                }
                infoTemp = infoTemp->next;
            }

            // If the infoName does not exist, add it to the end of the info list
            Info *newInfo = new Info(infoName, infoVal, nullptr);
            if (temp->headInfoList == nullptr) {
                // If the info list is empty, make the new info the head of the list
                temp->headInfoList = newInfo;
            } else {
                // If the info list is not empty, traverse to the end and add the new info
                infoTemp = temp->headInfoList;
                while (infoTemp->next != nullptr) {
                    infoTemp = infoTemp->next;
                }
                infoTemp->next = newInfo;
            }

            return true;
        }
        temp = temp->next;
    }

    // Contact not found
    return false;
}

// add a contact to the list in ascending order by last name
//     if last names are equal, then order by first name ascending
// 1. return false and do nothing if the contact is already in the list
// 2. otherwise return true and add the contact to the list
// - do not forget to update count
// - compare strings with the built-in comparison operators: <, >, ==, etc.
// - a compare method/function is recommended
bool ContactList::addContactOrdered(std::string first, std::string last) {
   // Check if the contact already exists in the list
    Contact *temp = this->headContactList;
    while (temp != nullptr) {
        if (temp->first == first && temp->last == last) {
            return false; // Contact already exists
        }
        temp = temp->next;
    }

    // If the contact does not exist, create a new contact
    Contact *newContact = new Contact(first, last, nullptr);

    // If the list is empty, make the new contact the head of the list
    if (this->headContactList == nullptr) {
        this->headContactList = newContact;
    } else {
        // If the list is not empty, find the correct position to insert the new contact
        temp = this->headContactList;
        Contact *prev = nullptr;
        while (temp != nullptr && (temp->last < last || (temp->last == last && temp->first < first))) {
            prev = temp;
            temp = temp->next;
        }

        // Insert the new contact at the correct position
        if (prev == nullptr) {
            // Insert at the head of the list
            newContact->next = this->headContactList;
            this->headContactList = newContact;
        } else {
            // Insert at the middle or end of the list
            newContact->next = prev->next;
            prev->next = newContact;
        }
    }

    // Increase the count of contacts
    this->count++;

    return true;
}

// add info to a contact's info list in ascending order by infoName
// 1. return false and do nothing if the contact is not in the list
// 2. if the infoName is already in the info list, update the infoValue and return true
// 3. otherwise add the info to the contact's list and return true
bool ContactList::addInfoOrdered(std::string first, std::string last, std::string infoName, std::string infoVal) {
 // Check if the contact exists in the list
    Contact *temp = this->headContactList;
    while (temp != nullptr) {
        if (temp->first == first && temp->last == last) {
            // Contact found, check if the infoName already exists in the info list
            Info *infoTemp = temp->headInfoList;
            Info *prev = nullptr;
            while (infoTemp != nullptr) {
                if (infoTemp->name == infoName) {
                    // InfoName found, update the infoValue and return true
                    infoTemp->value = infoVal;
                    return true;
                }
                if (infoTemp->name > infoName) {
                    break;
                }
                prev = infoTemp;
                infoTemp = infoTemp->next;
            }

            // If the infoName does not exist, create a new info and insert it in the correct position
            Info *newInfo = new Info(infoName, infoVal, infoTemp);
            if (prev == nullptr) {
                // Insert at the head of the list
                temp->headInfoList = newInfo;
            } else {
                // Insert at the middle or end of the list
                prev->next = newInfo;
            }

            return true;
        }
        temp = temp->next;
    }

    // Contact not found
    return false;
}

// remove the contact and its info from the list
// 1. return false and do nothing if the contact is not in the list
// 2. otherwise return true and remove the contact and its info
// - do not forget to update count
bool ContactList::removeContact(std::string first, std::string last) {
  // Check if the list is empty
    if (this->headContactList == nullptr) {
        return false; // List is empty, contact not found
    }

    Contact *temp = this->headContactList;
    Contact *prev = nullptr;

    // Traverse the list to find the contact
    while (temp != nullptr) {
        if (temp->first == first && temp->last == last) {
            // Contact found, remove it from the list
            if (prev == nullptr) {
                // The contact to remove is the head of the list
                this->headContactList = temp->next;
            } else {
                // The contact to remove is in the middle or end of the list
                prev->next = temp->next;
            }

            // Delete the contact's info list
            Info *infoTemp = temp->headInfoList;
            while (infoTemp != nullptr) {
                Info *nextInfo = infoTemp->next;
                delete infoTemp;
                infoTemp = nextInfo;
            }

            // Delete the contact
            delete temp;

            // Decrease the count of contacts
            this->count--;

            return true;
        }

        prev = temp;
        temp = temp->next;
    }

    // Contact not found
    return false;
}

// remove the info from a contact's info list
// 1. return false and do nothing if the contact is not in the list
// 2. return false and do nothing if the info is not in the contact's info list
// 3. otherwise return true and remove the info from the contact's list
bool ContactList::removeInfo(std::string first, std::string last, std::string infoName) {
  // Check if the contact exists in the list
    Contact *temp = this->headContactList;
    while (temp != nullptr) {
        if (temp->first == first && temp->last == last) {
            // Contact found, check if the infoName exists in the info list
            Info *infoTemp = temp->headInfoList;
            Info *prev = nullptr;
            while (infoTemp != nullptr) {
                if (infoTemp->name == infoName) {
                    // InfoName found, remove it from the list
                    if (prev == nullptr) {
                        // The info to remove is the head of the list
                        temp->headInfoList = infoTemp->next;
                    } else {
                        // The info to remove is in the middle or end of the list
                        prev->next = infoTemp->next;
                    }

                    // Delete the info
                    delete infoTemp;

                    return true;
                }

                prev = infoTemp;
                infoTemp = infoTemp->next;
            }

            // InfoName not found
            return false;
        }

        temp = temp->next;
    }

    // Contact not found
    return false;
}

// destroy the list by removing all contacts and their infos
ContactList::~ContactList() {
    Contact *cur = headContactList, *nextContact;
    while (cur != nullptr) {
        nextContact = cur->next;
        Info *curInfo = cur->headInfoList, *nextInfo;
        while (curInfo != nullptr) {
            nextInfo = curInfo->next;
            delete curInfo; // Free each Info node
            curInfo = nextInfo;
        }
        delete cur; // Free each Contact node
        cur = nextContact;
    }
}

// deep copy the source list
// - do not forget to update count
ContactList::ContactList(const ContactList &src) {
 headContactList = nullptr;
    count = src.count;
    Contact **lastPtr = &headContactList;
    for (Contact *srcCur = src.headContactList; srcCur != nullptr; srcCur = srcCur->next) {
        *lastPtr = new Contact(srcCur->first, srcCur->last);
        Info **lastInfoPtr = &((*lastPtr)->headInfoList);
        for (Info *srcInfo = srcCur->headInfoList; srcInfo != nullptr; srcInfo = srcInfo->next) {
            *lastInfoPtr = new Info(srcInfo->name, srcInfo->value);
            lastInfoPtr = &((*lastInfoPtr)->next);
        }
        lastPtr = &((*lastPtr)->next);
    }
}

// remove all contacts and their info then deep copy the source list
// - do not forget to update count
const ContactList &ContactList::operator=(const ContactList &src) {
 if (this != &src) { // Protect against self-assignment
        // Clear current list
        this->~ContactList(); // Reuse destructor to free current memory

        // Deep copy from src
        headContactList = nullptr;
        count = src.count;
        Contact **lastPtr = &headContactList;
        for (Contact *srcCur = src.headContactList; srcCur != nullptr; srcCur = srcCur->next) {
            *lastPtr = new Contact(srcCur->first, srcCur->last);
            Info **lastInfoPtr = &((*lastPtr)->headInfoList);
            for (Info *srcInfo = srcCur->headInfoList; srcInfo != nullptr; srcInfo = srcInfo->next) {
                *lastInfoPtr = new Info(srcInfo->name, srcInfo->value);
                lastInfoPtr = &((*lastInfoPtr)->next);
            }
            lastPtr = &((*lastPtr)->next);
        }
    }
    return *this;
}

