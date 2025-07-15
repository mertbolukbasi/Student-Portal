#ifndef PROFESSOR_H
#define PROFESSOR_H
#include "Person.h"
#include "Course.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

/*
    The Professor Class is inherited from Person class.
    That's why, Professor have also an username and password.
*/  

class Professor: public Person
{
    //Members
    public:
    Course* courseProf;

    private:
    unsigned int sizeCourseProf;

    public:
    //Constructor and Destructors
    Professor();
    Professor(string, string);
    Professor(const Professor &obj);
    ~Professor();


    //Setter and Getters
    void setSizeCourseProf(unsigned int sizeCourseProf);
    unsigned int getSizeCourseProf();
};

Professor::Professor()
:Person()
{
    this->courseProf = nullptr;
    this->sizeCourseProf = 0;
}

Professor::Professor(string password, string username)
:Person(password, username)
{
    this->sizeCourseProf = 0;
    this->courseProf = nullptr;
}

Professor::Professor(const Professor &obj)
:Person(obj)
{
    this->sizeCourseProf = obj.sizeCourseProf;
    this->courseProf = new Course[this->sizeCourseProf];

    for(unsigned short i = 0; i < this->sizeCourseProf; i++)
        this->courseProf[i] = obj.courseProf[i];
}

Professor::~Professor()
{
    this->sizeCourseProf = 0;
    delete[] courseProf;
    courseProf = nullptr;
}

void Professor::setSizeCourseProf(unsigned int sizeCourseProf)
{
    this->sizeCourseProf = sizeCourseProf;
}

unsigned int Professor::getSizeCourseProf()
{
    return this->sizeCourseProf;
}

/*
1. Size of username (size_t)
2. Username (string)
3. Size of password (size_t)
4. Size of Courses (unsigned int)
5. Course ID of all courses (unsigned int)
*/
void writeProfessorFile(const string &fileName, Professor prof)
{
    ofstream file(fileName, ios::binary | ios::app);

    if(file.is_open())
    {
        size_t size = prof.getUsername().size();
        file.write((char*)&size, sizeof(size_t));
        file.write(prof.getUsername().data(), size);

        size = prof.getPassword().size();
        file.write((char*)&size, sizeof(size_t));
        file.write(prof.getPassword().data(), size);

        unsigned int sizeCourseProf = prof.getSizeCourseProf();
        file.write((char*)&sizeCourseProf, sizeof(unsigned int));

        unsigned int courseID = 0;
        for(unsigned int i = 0; i < sizeCourseProf; i++)
        {
            courseID = prof.courseProf[i].getId();
            file.write((char*)&courseID, sizeof(unsigned int));
        }
        file.close();
    }

    else
        cout << "Couldn't open the file!" << endl;
}

bool readProfessorFile(const string &fileName, string username, string password, Professor &prof, bool isExistCheck)
{
    ifstream file(fileName, ios::binary | ios::ate);
    if(file.is_open())
    {
        streampos fileSize = file.tellg();
        file.seekg(0, ios::beg);
        char* mBlock = new char[fileSize];
        file.read(mBlock, fileSize);
        file.close();
        char* p = mBlock;
        char* end = mBlock + fileSize;
        bool isExist = false;

        while(p < end)
        {
            size_t sizeUsername = *((size_t*)p);
            p+=sizeof(size_t);

            char* bufferUsername = new char[sizeUsername + 1];
        
            for (size_t i = 0; i < sizeUsername; ++i) 
            {
                bufferUsername[i] = *(p++);
            }

            bufferUsername[sizeUsername] = '\0';
            string usernameString = bufferUsername;
            delete[] bufferUsername;

            if(isExistCheck)
            {
                if(username == usernameString)
                    return isExistCheck;
            }

            size_t sizePassword = *((size_t*)p);
            p += sizeof(size_t);

            char* bufferPassword = new char[sizePassword + 1];

            for (size_t i = 0; i < sizePassword; ++i) 
            {
                bufferPassword[i] = *(p++);
            }

            bufferPassword[sizePassword] = '\0';
            string passwordString = bufferPassword;
            delete[] bufferPassword;

            
            if (usernameString == username && passwordString == password)
            {
                isExist = true;

                unsigned int sizeOfCourses = *((unsigned int*)p);
                p += sizeof(unsigned int);

                prof.setUsername(usernameString);
                prof.setPassword(passwordString);
                prof.setSizeCourseProf(sizeOfCourses);
                prof.courseProf = new Course[sizeOfCourses];
                for(unsigned int i = 0; i < sizeOfCourses; i++)
                {
                    unsigned int id = *((unsigned int*)p);
                    p += sizeof(unsigned int);
                    prof.courseProf[i].setId(id);
                }
                break;
            }
            
            unsigned int sizeOfCourses = *((unsigned int*)p);
            p += sizeof(unsigned int);

            p += sizeOfCourses * sizeof(unsigned int);
        }
        delete[] mBlock;
        return isExist;
                
    }
    else
        cout << "Couldn't open the file!" << endl;

    return false;
}

Professor *readAllProfessor(const string &fileName, int &numberOfProfessor)
{
    Professor* tab = nullptr;

    ifstream file(fileName, ios::binary | ios::ate);
    if(file.is_open())
    {   
        streampos fileSize = file.tellg();
        file.seekg(0, ios::beg);
        char* mBlock = new char[fileSize];
        file.read(mBlock, fileSize);
        file.close();
        char* p = mBlock;
        char* end = fileSize + mBlock;

        while(p < end)
        {
            size_t sizeUsername = *((size_t*)p);
            p+=sizeof(size_t);

            p += sizeUsername;

            size_t sizePassword = *((size_t*)p);
            p+=sizeof(size_t);

            p += sizePassword;

            unsigned int sizeOfCourses = *((unsigned int*)p);
            p+=sizeof(unsigned int);

            p+=sizeOfCourses * sizeof(unsigned int) ;
            numberOfProfessor++;
        }

        p = mBlock;
        end = fileSize + mBlock;
        tab = new Professor[numberOfProfessor];

        for(int i = 0; i < numberOfProfessor; i++)
        {
           
            size_t sizeUsername = *((size_t*)p);
            p += sizeof(size_t);

            char* bufferUsername = new char[sizeUsername + 1];
        
            for (size_t i = 0; i < sizeUsername; ++i) 
            {
                bufferUsername[i] = *(p++);
            }

            bufferUsername[sizeUsername] = '\0';
            string usernameString = bufferUsername;
            delete[] bufferUsername;

            size_t sizePassword = *((size_t*)p);
            p+=sizeof(size_t);

            char* bufferPassword = new char[sizePassword + 1];

            for (size_t i = 0; i < sizePassword; ++i) 
            {
                bufferPassword[i] = *(p++);
            }

            bufferPassword[sizePassword] = '\0';
            string passwordString = bufferPassword;
            delete[] bufferPassword;

            unsigned int sizeOfCourses = *((unsigned int*)p);
            p+=sizeof(unsigned int);

            tab[i].setUsername(usernameString);
            tab[i].setPassword(passwordString);
            tab[i].setSizeCourseProf(sizeOfCourses);
            tab[i].courseProf = new Course[sizeOfCourses];
            for(unsigned int j = 0; j < sizeOfCourses; j++)
            {
                unsigned int id = *((unsigned int*)p);
                p+=sizeof(unsigned int);
                tab[i].courseProf[j].setId(id);
            }

        }
        delete[] mBlock;
    }
    else
        cout << "Couldn't open the file!" << endl;

    return tab;
}

void removeProfessor(const string &fileName, const string &username)
{
    ifstream file(fileName, ios::binary | ios::ate);
    ofstream temp("temp.bin", ios::binary);

    if(file.is_open() && temp.is_open())
    {
        streampos fileSize = file.tellg();
        file.seekg(0, ios::beg);
        char* mBlock = new char [fileSize];
        file.read(mBlock, fileSize);
        file.close();
        char* p = mBlock;
        char* end = mBlock + fileSize;
        
        while(p < end)
        {
            size_t sizeUsername = *((size_t*)p);
            p += sizeof(size_t);

            char* bufferUsername = new char[sizeUsername + 1];
        
            for (size_t i = 0; i < sizeUsername; ++i) {
                bufferUsername[i] = *(p++);
            }

            bufferUsername[sizeUsername] = '\0';
            string usernameString = bufferUsername;
            delete[] bufferUsername;

            size_t sizePassword = *((size_t*)p);
            p += sizeof(size_t);

            char* bufferPassword = new char[sizePassword + 1];

            for(size_t i = 0; i < sizePassword; ++i)
            {
                bufferPassword[i] = *(p++);
            }

            bufferPassword[sizePassword] = '\0';
            string passwordString = bufferPassword;
            delete[] bufferPassword;

            unsigned int sizeOfCourses = *((unsigned int*)p);
            p += sizeof(unsigned int);

            Course* courses = new Course [sizeOfCourses];

            for(unsigned int i = 0; i < sizeOfCourses; i++)
            {
                unsigned int id = *((unsigned int*)p);
                p += sizeof(int);
                courses[i].setId(id);
            }

            if(username != usernameString)
            {
                temp.write((char*)&sizeUsername, sizeof(size_t));
                temp.write(usernameString.data(), sizeUsername);
                temp.write((char*)&sizePassword, sizeof(size_t));
                temp.write(passwordString.data(), sizePassword);
                temp.write((char*)&sizeOfCourses, sizeof(unsigned int));

                for(unsigned int i = 0; i < sizeOfCourses; i++)
                {
                    int courseID = courses[i].getId();
                    temp.write((char*)&courseID, sizeof(unsigned int));
                }
            }
        }

        delete[] mBlock;
        temp.close();

        remove(fileName.data());
        rename("temp.bin", fileName.data());
    }

    else
    {
        cout << "Files couldn't opened!" << endl;
    }

    
}


//According to project file one course must have one professor.
bool doesCourseHaveProfessor(const string filename, unsigned int courseID)
{
    ifstream file(filename, ios::binary | ios::ate);
 
    if(file.is_open())
    {
        streampos fileSize = file.tellg();
        file.seekg(0, ios::beg);
        char* mBlock = new char [fileSize];
        file.read(mBlock, fileSize);
        file.close();

        char* p = mBlock;
        char* end = mBlock + fileSize;

        while(p < end)
        {
            size_t sizeUsername = *((size_t*)p);
            p += sizeof(size_t);
            p += sizeUsername;

            size_t sizePassword = *((size_t*)p);
            p += sizeof(size_t);
            p += sizePassword;

            unsigned int sizeOfCourses = *((unsigned int*)p);
            p += sizeof(unsigned int);

            for(unsigned int i = 0; i < sizeOfCourses; i++)
            {
                unsigned int fileCourseID = *((unsigned int*)p);
                p += sizeof(unsigned int);

                if(fileCourseID == courseID) //Checking whether another professor has this course
                {
                    delete[] mBlock;
                    return true;
                }
            }
        }

        delete[] mBlock;
    }

    else
    {
        cout << "File couldn't opened." << endl;
    }

    return false;
}


//A professor panel for displaying courses
void professorPanel(Professor &professor, bool &isSystemOpen)
{
    cout << "******************************************" << endl;
    cout << "Welcome to Professor Panel!" << endl;
    cout << "******************************************" << endl;
    cout << "1. View Courses" << endl;
    cout << "2. Back to main menu" << endl;
    cout << "3. Exit the system" << endl;

    unsigned short number = 0;
    do
    {
        cout << "Enter the your choice: ";
        cin >> number;
    } while (number > 3 || number == 0); //You don't have to write a big number :D
    
    if(number == 1)
    {
        cout << "Courses: " << endl;
        for(unsigned int i = 0; i < professor.getSizeCourseProf(); i++)
        {
            cout << "Course ID: " << professor.courseProf[i].getId() << endl;
        }
        return professorPanel(professor, isSystemOpen);
    }

    if(number == 2)
    {
        
    }

    if(number == 3)
    {
        isSystemOpen = false;
    }
}

#endif