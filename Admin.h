#ifndef ADMIN_H
#define ADMIN_H
#include "Person.h"
#include "Assistant.h"
#include "Student.h"
#include "Professor.h"
#include "Course.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

/*
    Admin class is inherited from Person class.
    An admin have also a username and password.
*/

class Admin: public Person
{
    public:
    Admin();
    ~Admin();
    Admin(const Admin &obj);
    Admin(string, string);
};

Admin::Admin():
Person()
{

}

Admin::~Admin()
{

}

Admin::Admin(string password, string username): 
Person(password, username)
{
    
};

Admin::Admin(const Admin &obj)
{
    
}

//An admin has to be in the database, otherwise the program doesn't work
void writeAdmin(const string &fileName, string username, string password)
{
    /*We won't use ios::app, because admindb.bin fills with 
    admin, adminpw whenever the program runs if we use it.*/

    ofstream file(fileName, ios::binary);

    if(file.is_open())
    {
        size_t size = username.size();
        file.write((char*)&size, sizeof(size_t));
        file.write(username.data(), size);

        size = password.size();
        file.write((char*)&size, sizeof(size_t));
        file.write(password.data(), size);
    }

    else
        cout << "Couldn't open the file!" << endl;
}

/*
    1. Size of username (size_t)
    2. Username (string)
    3. Size of password (size_t)
    4. Password (string)
*/

bool readFileAdmin(const string &fileName, string username, string password)
{
    ifstream file(fileName, ios::binary | ios::ate);
    if(file.is_open())
    {
        streampos fileSize = file.tellg();
        file.seekg(0, ios::beg);
        char* mBlock = new char[fileSize];
        file.read(mBlock, fileSize);
        char *p = mBlock;

        size_t sizeUsername = *((size_t*)p);
        p+=sizeof(size_t);

        char* bufferUsername = new char[sizeUsername + 1];
        
        for (size_t i = 0; i < sizeUsername; ++i) 
        {
            bufferUsername[i] = *p++;
        }

        bufferUsername[sizeUsername] = '\0';
        string usernameString = bufferUsername;
        delete[] bufferUsername;

        size_t sizePassword = *((size_t*)p);
        p += sizeof(size_t);

        char* bufferPassword = new char[sizePassword + 1];

        for (size_t i = 0; i < sizePassword; ++i) 
        {
            bufferPassword[i] = *p++;
        }
        bufferPassword[sizePassword] = '\0';
        string passwordString = bufferPassword;
        delete[] bufferPassword;

        bool isExist = false;
        if (usernameString == username && passwordString == password)
        {
            isExist = true;
        }
        return isExist;
        delete[] mBlock;
    }
    else
    {
        cout << "File Error!";
        return false;
    }
    
    return false;
}

//For explanation, look at the adminPanel function
void createFile(const string &filename)
{
    ofstream file(filename, ios::binary | ios::app);
}

/*
    An admin can do these things using the panel:
    1. Adding
    2. Removing
    3. Show all
*/
void adminPanel(bool &isSystemOpen)
{
    cout << "******************************************" << endl;
    cout << "Welcome to Admin Panel!" << endl;
    cout << "******************************************" << endl;
    cout << "1. Add Professor" << endl;
    cout << "2. Add Assistant" << endl;
    cout << "3. Add Student" << endl;
    cout << "4. Add Course" << endl;
    cout << "5. Remove Professor" << endl;
    cout << "6. Remove Student" << endl;
    cout << "7. Remove Assistant" << endl;
    cout << "8. Show all professors" << endl;
    cout << "9. Show all assistants" << endl;
    cout << "10. Show all students" << endl;
    cout << "11. Back to main menu" << endl;
    cout << "12. Exit the system" << endl;

    unsigned short number = 0;
    
    do
    {
        if(number > 12 || number < 1)
        {
            cout << "Enter the number between 1 and 12" << endl;
        }
        cout << "Enter the choice: ";
        cin >> number;
    } while (number > 12 || number < 1); //Please don't enter a invalid number, it won't work.
    
    if(number == 1)
    {
        string username = "";
        string password = "";
        unsigned int numberOfCourse = 0;
        int courseID = 0;
        bool isExistCheck = true;
        bool isThereAnyCourse = false;

        createFile("professordb.bin"); //If we don't create a file, we get "File couldn't opened"!

        cin.ignore(); //Added because terminal is affected without it
        
        cout << "Enter the professor username: ";
        getline(cin, username);
        Professor temp;

        //Checking if there is a professor with the same name
        isExistCheck = readProfessorFile("professordb.bin", username, password, temp, isExistCheck);
        if(isExistCheck)
        {
            cout << "This professor exists! You cannot add." << endl;
            adminPanel(isSystemOpen);
        }
        
        else
        {
            cout << "Enter the professor password: ";
            getline(cin, password);
            cout << "Enter the number of courses: ";
            cin >> numberOfCourse;

            Course* courses = new Course[numberOfCourse];
            for(unsigned int i = 0; i < numberOfCourse; i++)
            {
                do
                {
                    cout << "Enter the " << i+1 << ". course id: ";
                    cin >> courseID;

                    Course course; 
                    //Checking the written course is in the course database
                    isThereAnyCourse = readCourseFile("coursedb.bin", courseID, course);

                    if(isThereAnyCourse)
                    {
                        //Checking the course is assigned to a professor
                        if(doesCourseHaveProfessor("professordb.bin", courseID))
                        {
                            cout << "This course have a professor!" << endl;
                            isThereAnyCourse = false;
                        }
                    }

                    else
                    {
                        cout << "This course isn't in the database!" << endl;
                    }

                } while(!isThereAnyCourse);

                courses[i].setId(courseID);
            }
           
            Professor p1(password, username);
            p1.setSizeCourseProf(numberOfCourse);

            p1.courseProf = courses;

            writeProfessorFile("professordb.bin", p1);
            adminPanel(isSystemOpen);
        }
        
    }

    if(number == 2)
    {
        string username = "";
        string password = "";
        unsigned int numberOfCourseTeach = 0;
        int courseIDTeach = 0;
        unsigned int numberOfCourseLearn = 0;
        int courseIDLearn = 0;
        float gpa = 0.0f;
        int id = 0;
        bool isExistCheck = true;
        bool isThereAnyCourse = false;

        createFile("assistantdb.bin");

        cin.ignore();

        cout << "Enter the assistant username: ";
        getline(cin, username);
        Assistant temp;
        isExistCheck = readAssistantFile("assistantdb.bin", username, password, temp, isExistCheck);
        if(isExistCheck)
        {
            cout << "This assistant exists! You cannot add." << endl;
            adminPanel(isSystemOpen);
        }
        else
        {
            cout << "Enter the assistant password: ";
            getline(cin, password);
            cout << "Enter the assistant's student ID: ";
            cin >> id;

            do
            {
                cout << "Enter the student gpa: ";
                cin >> gpa;

                if(gpa > 4)
                    cout << "GPA cannot be bigger than four!" << endl;
            }while(gpa > 4);

            cout << "Enter the number of courses that assistant teaches: ";
            cin >> numberOfCourseTeach;

            Course* coursesTeach = new Course[numberOfCourseTeach];

            for(unsigned int i = 0; i < numberOfCourseTeach; i++)
            {
                do
                {
                    cout << "Enter " << i + 1 << ". course id:";
                    cin >> courseIDTeach;

                    Course course;
                    isThereAnyCourse = readCourseFile("coursedb.bin", courseIDTeach, course);

                    if(!isThereAnyCourse)
                    {
                        cout << "The course isn't in the database" << endl;
                    }
                    
                } while (!isThereAnyCourse);
                
                coursesTeach[i].setId(courseIDTeach);
            }

            cout << "Enter the number of courses that assistant learns: ";
            cin >> numberOfCourseLearn;

            Course* coursesLearn = new Course[numberOfCourseLearn];

            bool isConflict = false;

            for(unsigned int i = 0; i < numberOfCourseLearn; i++)
            {
                do
                {
                    cout << "Enter " << i + 1 << ". course id:";
                    cin >> courseIDLearn;

                    Course course;
                    isThereAnyCourse = readCourseFile("coursedb.bin", courseIDLearn, course);

                    if(!isThereAnyCourse)
                    {
                        cout << "The course isn't in the database" << endl;
                    }

                    else
                    {
                        isConflict = false;

                        //An assistant cannot teach and learn same course
                        for(unsigned int j = 0; j < numberOfCourseTeach; j++)
                        {
                            if(courseIDLearn == coursesTeach[j].getId())
                            {
                                isConflict = true;

                                cout << "The assistant cannot both teach and learn a course!" << endl;
                                break;
                            }
                        }
                    }
                    
                } while (!isThereAnyCourse || isConflict);

                coursesLearn[i].setId(courseIDLearn);
            }

            Assistant a1(password, username, id, gpa);

            a1.setSizeCourseProf(numberOfCourseTeach);
            a1.courseProf = coursesTeach;

            a1.setSizeCourseStudent(numberOfCourseLearn);
            a1.courseStudent= coursesLearn;

            writeAssistantFile("assistantdb.bin", a1);
            adminPanel(isSystemOpen);
        }
        
    }

    if(number == 3)
    {
        string username = "";
        string password = "";
        unsigned int numberOfCourse = 0;
        int courseID = 0;
        float gpa = 0.0;
        int id = 0;
        bool isExistCheck = true;
        bool isThereAnyCourse = false;

        createFile("studentdb.bin");

        cin.ignore();
        
        cout << "Enter the student username: ";
        getline(cin, username);
        Student temp;
        isExistCheck = readStudentFile("studentdb.bin", username, password, temp, isExistCheck);
        if(isExistCheck)
        {
            cout << "This student exists! You cannot add." << endl;
            adminPanel(isSystemOpen);
        }
        
        else
        {
            cout << "Enter the student password: ";
            getline(cin, password);
            cout << "Enter the student id: ";
            cin >> id;

            do
            {
                cout << "Enter the student gpa: ";
                cin >> gpa;

                if(gpa > 4)
                    cout << "GPA cannot be bigger than four!" << endl;
            }while(gpa > 4);


            cout << "Enter the number of courses: ";
            cin >> numberOfCourse;

            Course* courses = new Course[numberOfCourse];
            for(unsigned int i = 0; i < numberOfCourse; i++)
            {
                do
                {
                    cout << "Enter the " << i+1 << ". course id: ";
                    cin >> courseID;

                    Course course;

                    isThereAnyCourse = readCourseFile("coursedb.bin", courseID, course);

                    if(!isThereAnyCourse)
                    {
                        cout << "The course isn't in the database" << endl;
                    }


                }while(!isThereAnyCourse);

                    courses[i].setId(courseID);
            }
           
            Student s1(password, username, id, gpa);
            s1.setSizeCourseStudent(numberOfCourse);

            s1.courseStudent = courses;

            writeStudentFile("studentdb.bin", s1);
            adminPanel(isSystemOpen);
        }
    }

    if(number == 4)
    {
        string courseName;
        int id;
        bool isExistCheck = true;

        createFile("coursedb.bin");

        cin.ignore();

        cout << "Enter the course name: ";
        getline(cin, courseName);
        cout << "Enter the course id: ";
        cin >> id;

        Course c1(courseName, id);
        isExistCheck = readCourseFile("coursedb.bin", c1.getId(), c1);
        if(isExistCheck)
        {
            cout << "This course exists! You cannot add." << endl;
        }
        else
        {
            writeCourseFile("coursedb.bin", c1);
        }
        
        adminPanel(isSystemOpen);
        
        
    }

    if(number == 5)
    {
        string username = "";
        bool areYouSure;

        cin.ignore();

        cout << "Enter the professor's username: " << endl;
        getline(cin, username);

        cout << "Are you sure to delete the professor (1 [Yes] / 0 [No]): " << endl;
        cin >> areYouSure;

        if(areYouSure)
        {
            removeProfessor("professordb.bin", username);
            adminPanel(isSystemOpen);
        }

        return adminPanel(isSystemOpen);
    }

    if(number == 6)
    {
        string username = "";
        bool areYouSure;

        cin.ignore();

        cout << "Enter the student's username: " << endl;
        getline(cin, username);

        cout << "Are you sure to delete the student (1 [Yes] / 0 [No]): " << endl;
        cin >> areYouSure;

        if(areYouSure)
        {
            removeStudent("studentdb.bin", username);
            adminPanel(isSystemOpen);
        }

        return adminPanel(isSystemOpen);
    }

    if(number == 7)
    {
        string username = "";
        bool areYouSure;

        cin.ignore();

        cout << "Enter the assistant's username: " << endl;
        getline(cin, username);

        cout << "Are you sure to delete the student (1 [Yes] / 0 [No]): " << endl;
        cin >> areYouSure;

        if(areYouSure)
        {
            removeAssistant("assistantdb.bin", username);
            adminPanel(isSystemOpen);
        }

        return adminPanel(isSystemOpen);
    }

    if(number == 8)
    {
        int numberOfProfessor = 0;
        Professor* allProfs = readAllProfessor("professordb.bin", numberOfProfessor);
        for(int i = 0; i<numberOfProfessor; i++)
        {
            cout << "Professor Username: " << allProfs[i].getUsername() << endl;
            cout << "Courses: " << endl;
            for(unsigned int j = 0; j < allProfs[i].getSizeCourseProf(); j++)
            {
                cout << j+1 << ". Course ID: " << allProfs[i].courseProf[j].getId() << endl;
            }
            cout << "******************************************" << endl;
        }
        cout << endl;
        delete[] allProfs;
        return adminPanel(isSystemOpen);
    }

    if(number == 9)
    {
        int numberOfAssistant = 0;
        Assistant* allAssistants = readAllAssistants("assistantdb.bin", numberOfAssistant);
        for(int i = 0; i<numberOfAssistant; i++)
        {
            cout << "Assistant Username: " << allAssistants[i].Student::getUsername() << endl;
            cout << "Assistant Id: " << allAssistants[i].getId() << endl;
            cout << "Assistant GPA: " << allAssistants[i].getGpa() << endl;
            cout << "Learning Courses: " << endl;
            for(unsigned int j = 0; j < allAssistants[i].getSizeCourseStudent(); j++)
            {
                cout << j+1 << ". Course ID: " << allAssistants[i].courseStudent[j].getId() << endl;
            }
            cout << "Teaching Courses: " << endl;
            for(unsigned int k = 0; k < allAssistants[i].getSizeCourseProf(); k++)
            {
                cout << k+1 << ". Course ID: " << allAssistants[i].courseProf[k].getId() << endl;
            }
            cout << "******************************************" << endl;
        }
        cout << endl;
        delete[] allAssistants;
        return adminPanel(isSystemOpen);
    }

    if(number == 10)
    {
        int numberOfStudent = 0;
        Student* allStudents = readAllStudents("studentdb.bin", numberOfStudent);
        for(int i = 0; i<numberOfStudent; i++)
        {
            cout << "Student Username: " << allStudents[i].getUsername() << endl;
            cout << "Student Id: " << allStudents[i].getId() << endl;
            cout << "Student GPA: " << allStudents[i].getGpa() << endl;
            cout << "Courses: " << endl;
            for(unsigned int j = 0; j < allStudents[i].getSizeCourseStudent(); j++)
            {
                cout << j+1 << ". Course ID: " << allStudents[i].courseStudent[j].getId() << endl;
            }
            cout << "******************************************" << endl;
        }
        cout << endl;
        delete[] allStudents;
        return adminPanel(isSystemOpen);
    }

    if(number == 11)
    {
        return;
    }

    if(number == 12)
    {
        isSystemOpen = false;
    }
}

#endif