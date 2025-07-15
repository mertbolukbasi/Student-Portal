#ifndef ASSISTANT_H
#define ASSISTANT_H
#include "Professor.h"
#include "Student.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/*
    The Assistant class is inherited from Student and Professor.
    This is called multiple inheritance.
    An assistant include these informations: username, password,
    student id, gpa, size of student course, student courses,
    size of professor course and professor courses.
*/

class Assistant: public Student, public Professor
{   
    public:
    Assistant();
    ~Assistant();
    Assistant(const Assistant &obj);
    Assistant(string, string, unsigned int, float);
};

Assistant::Assistant()
:Student(), Professor()
{
}

Assistant::~Assistant()
{
    if(courseStudent != nullptr)
    {
        delete[] courseStudent;
        courseStudent = nullptr;
    }
    
    if(courseProf != nullptr)
    {
        delete[] courseProf;
        courseProf = nullptr;
    }
}

Assistant::Assistant(const Assistant &obj)
:Student(obj), Professor(obj)
{

}

Assistant::Assistant(string password, string username, unsigned int id, float gpa)
:Student(password, username, id, gpa), Professor(password, username) 
{

};

/*
    1. Size of username (size_t)
    2. Username (string)
    3. Size of password (size_t)
    4. Password (string)
    5. Student ID (unsigned int)
    6. GPA (float)
    7. Size of Professor Courses (unsigned int)
    8. Professor Courses (unsigned int)
    9. Size of Student Courses (unsigned int)
    10. Student Courses (unsigned int)
*/

void writeAssistantFile(const string &fileName, Assistant assistant)
{
    ofstream file(fileName, ios::binary | ios::app);

    if(file.is_open())
    {
        /*
            Assistant is created from multiple inheritance.
            Therefore, assistant have actually two username as a professor and a student.
            If we write assistant.getUsername(), the compiler cannot decide which getUsername().
            That's why, the scope resolution operator is used.
        */
        size_t size = assistant.Student::getUsername().size();
        file.write((char*)&size, sizeof(size_t));
        file.write(assistant.Student::getUsername().data(), size);

        size = assistant.Student::getPassword().size();
        file.write((char*)&size, sizeof(size_t));
        file.write(assistant.Student::getPassword().data(), size);

        unsigned int id = assistant.getId();
        file.write((char*)&id, sizeof(unsigned int));

        float gpa = assistant.getGpa();
        file.write((char*)&gpa, sizeof(float));

        unsigned int sizeCourseProf = assistant.getSizeCourseProf();
        file.write((char*)&sizeCourseProf, sizeof(unsigned int));

        int courseIDProf = 0;
        for(unsigned int i = 0; i < sizeCourseProf; i++)
        {
            courseIDProf = assistant.courseProf[i].getId();
            file.write((char*)&courseIDProf, sizeof(unsigned int));
        }

        unsigned int sizeCourseStudent = assistant.getSizeCourseStudent();
        file.write((char*)&sizeCourseStudent, sizeof(unsigned int));

        int courseIDStudent = 0;
        for(unsigned int i = 0; i < sizeCourseStudent; i++)
        {
            courseIDStudent = assistant.courseStudent[i].getId();
            file.write((char*)&courseIDStudent, sizeof(unsigned int));
        }


        file.close();
        
    }

    else
        cout << "Couldn't open the file!" << endl;
}

bool readAssistantFile(const string &fileName, string username, string password, Assistant &assistant, bool isExistCheck)
{
    ifstream file(fileName, ios::binary | ios::ate);

    if(file.is_open())
    {
        streampos fileSize = file.tellg();
        file.seekg(0, ios::beg);
        char* mBlock = new char[fileSize];
        file.read(mBlock, fileSize);
        char *p = mBlock;
        char* endOfFile = mBlock + fileSize;

        bool isExist = false;
        while(p < endOfFile)
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
                if(usernameString == username)
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

                unsigned int id = *((unsigned int*)p);
                p += sizeof(unsigned int);

                float gpa = *((float*)p);
                p += sizeof(float);

                unsigned int sizeCoursesProf = *((unsigned int*)p);
                p += sizeof(unsigned int);

                assistant.Student::setUsername(usernameString);
                assistant.Student::setPassword(password);
                assistant.setId(id);
                assistant.setGpa(gpa);
                assistant.setSizeCourseProf(sizeCoursesProf);
                assistant.courseProf = new Course [sizeCoursesProf];

                for(unsigned int i = 0; i < sizeCoursesProf; i++)
                {
                    unsigned int courseID = *((unsigned int*)p);
                    p += sizeof(unsigned int);

                    assistant.courseProf[i].setId(courseID);
                }

                unsigned int sizeCoursesStudent = *((unsigned int*)p);
                p += sizeof(unsigned int);

                assistant.setSizeCourseStudent(sizeCoursesStudent);
                assistant.courseStudent = new Course [sizeCoursesStudent];

                for(unsigned int i = 0; i < sizeCoursesStudent; i++)
                {
                    unsigned int courseID = *((unsigned int*)p);
                    p += sizeof(unsigned int);

                    assistant.courseStudent[i].setId(courseID);
                }

                break;
            }

            p += sizeof(unsigned int);
            p += sizeof(float);

            unsigned int sizeCoursesProf = *((unsigned int*)p);
            p += sizeof(unsigned int);
            p += sizeCoursesProf * sizeof(unsigned int);

            unsigned int sizeCoursesStudent = *((unsigned int*)p);
            p += sizeof(unsigned int);
            p += sizeCoursesStudent * sizeof(unsigned int);

        }

        delete[] mBlock;

        return isExist;
    }

    else
    {
        cout << "Couldn't open the file" << endl;
        return false;
    }
}

Assistant* readAllAssistants(const string &fileName, int &numberOfAssistant)
{
    Assistant* tab = nullptr;
    numberOfAssistant = 0;

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

            p += sizeof(unsigned int);
            p += sizeof(float);

            unsigned int sizeOfCoursesProf = *((unsigned int*)p);
            p+=sizeof(unsigned int); //I appreciate it to my friend from here. He forgot it and I struugled with it for 2 hours.
            p+=sizeOfCoursesProf * sizeof(int);

            unsigned int sizeOfCoursesStudent = *((unsigned int*)p);
            p+=sizeof(unsigned int);
            p+=sizeOfCoursesStudent * sizeof(int);

            numberOfAssistant++;
        }

        p = mBlock;
        end = fileSize + mBlock;
        tab = new Assistant[numberOfAssistant];

        for(int i = 0; i < numberOfAssistant; i++)
        {
           
            size_t sizeUsername = *((size_t*)p);
            p+=sizeof(size_t);

            char* bufferUsername = new char[sizeUsername + 1];
        
            for (size_t j = 0; j < sizeUsername; ++j) {
                bufferUsername[j] = *(p++);
            }

            bufferUsername[sizeUsername] = '\0';
            string usernameString = bufferUsername;
            delete[] bufferUsername;

            size_t sizePassword = *((size_t*)p);
            p+=sizeof(size_t);

            char* bufferPassword = new char[sizePassword + 1];

            for (size_t j = 0; j < sizePassword; ++j) {
                bufferPassword[j] = *p++;
            }

            bufferPassword[sizePassword] = '\0';
            string passwordString = bufferPassword;
            delete[] bufferPassword;

            unsigned int id = *((unsigned int*)p);
            p += sizeof(unsigned int);

            float gpa = *((float*)p);
            p += sizeof(float);

            unsigned int sizeOfCoursesProf = *((unsigned int*)p);
            p+=sizeof(unsigned int);

            tab[i].courseProf = new Course[sizeOfCoursesProf];
            for(unsigned int j = 0; j < sizeOfCoursesProf; j++)
            {
                unsigned int courseID = *((unsigned int*)p);
                p += sizeof(unsigned int);
                tab[i].courseProf[j].setId(courseID);
            }

            unsigned int sizeOfCoursesStudent = *((unsigned int*)p);
            p+=sizeof(unsigned int);

            tab[i].courseStudent = new Course[sizeOfCoursesStudent];
            for(unsigned int j = 0; j < sizeOfCoursesStudent; j++)
            {
                unsigned int courseID = *((unsigned int*)p);
                p+=sizeof(unsigned int);
                tab[i].courseStudent[j].setId(courseID);
            }

            tab[i].Student::setUsername(usernameString);
            tab[i].Student::setPassword(passwordString);
            tab[i].setId(id);
            tab[i].setGpa(gpa);
            tab[i].setSizeCourseProf(sizeOfCoursesProf);
            tab[i].setSizeCourseStudent(sizeOfCoursesStudent);
        }
        delete[] mBlock;
    }
    else
        cout << "Couldn't open the file!" << endl;

    return tab;
}

void removeAssistant(const string &fileName, const string &username)
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
                bufferUsername[i] = *p++;
            }

            bufferUsername[sizeUsername] = '\0';
            string usernameString = bufferUsername;
            delete[] bufferUsername;

            size_t sizePassword = *((size_t*)p);
            p += sizeof(size_t);

            char* bufferPassword = new char[sizePassword + 1];

            for(size_t i = 0; i < sizePassword; ++i)
            {
                bufferPassword[i] = *p++;
            }

            bufferPassword[sizePassword] = '\0';
            string passwordString = bufferPassword;
            delete[] bufferPassword;

            unsigned int id = *((unsigned int*)p);
            p += sizeof(unsigned int);

            float gpa = *((float*)p);
            p += sizeof(float);

            unsigned int sizeOfCoursesProf = *((unsigned int*)p);
            p += sizeof(unsigned int);

            Course* courseProf = new Course [sizeOfCoursesProf];

            for(unsigned int i = 0; i < sizeOfCoursesProf; i++)
            {
                unsigned int courseID = *((unsigned int*)p);
                p += sizeof(unsigned int);
                courseProf[i].setId(courseID);
            }

            unsigned int sizeOfCourseStudent = *((unsigned int*)p);
            p += sizeof(unsigned int);


            Course* coursesStudent = new Course [sizeOfCourseStudent];

            for(unsigned int i = 0; i < sizeOfCourseStudent; i++)
            {
                unsigned int id = *((unsigned int*)p);
                p += sizeof(int);
                coursesStudent[i].setId(id);
            }

            if(username != usernameString)
            {
                temp.write((char*)&sizeUsername, sizeof(size_t));
                temp.write(usernameString.data(), sizeUsername);
                temp.write((char*)&sizePassword, sizeof(size_t));
                temp.write(passwordString.data(), sizePassword);
                temp.write((char*)&id, sizeof(unsigned int));
                temp.write((char*)&gpa, sizeof(float));
                temp.write((char*)&sizeOfCoursesProf, sizeof(unsigned int));

                for(unsigned int i = 0; i < sizeOfCoursesProf; i++)
                {
                    int courseID = courseProf[i].getId();
                    temp.write((char*)&courseID, sizeof(unsigned int));
                }

                temp.write((char*)&sizeOfCourseStudent, sizeof(unsigned int));

                for(unsigned int i = 0; i < sizeOfCourseStudent; i++)
                {
                    int courseID = coursesStudent[i].getId();
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

/*
    An assistant panel:
    1. Viewing Student and Professor Courses
    2. For username and gpa and student ID
*/
void assistantPanel(Assistant &assistant, bool &isSystemOpen)
{
    cout << "******************************************" << endl;
    cout << "Welcome to Assistant Panel!" << endl;
    cout << "******************************************" << endl;
    cout << "1. View Courses" << endl;
    cout << "2. View Personal Information" << endl;
    cout << "3. Back to main menu" << endl;
    cout << "4. Exit the system" << endl;

    unsigned short number = 0;
    do
    {
        cout << "Enter the your choice: ";
        cin >> number;
    } while (number > 4);
    
    if(number == 1)
    {
        cout << "Courses that you learn: " << endl;
        for(unsigned int i = 0; i < assistant.getSizeCourseStudent(); i++)
        {
            cout << "Course ID: " << assistant.courseStudent[i].getId() << endl;
        }

        cout << "Courses that you teach: " << endl;
        for(unsigned int i = 0; i < assistant.getSizeCourseProf(); i++)
        {
            cout << "Course ID: " << assistant.courseProf[i].getId() << endl;
        }

        return assistantPanel(assistant, isSystemOpen);
    }

    if(number == 2)
    {
        cout << "Assistant ID: " << assistant.getId() << endl;
        cout << "Assistant Username: " << assistant.Student::getUsername() << endl;
        cout << "Student GPA: " << assistant.getGpa() << endl;
        return assistantPanel(assistant, isSystemOpen);
    }

    if(number == 3)
    {

    }

    if(number == 4)
    {
        isSystemOpen = false;
    }
}

#endif