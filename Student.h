#ifndef STUDENT_H
#define STUDENT_H
#include "Person.h"
#include "Course.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/*
The Student class is inherited from Person class.
That's why, Student have also an username and password.
*/

class Student: public Person
{
    public:
    Course* courseStudent;

    private:
    unsigned int id;
    unsigned int sizeCourseStudent;
    float gpa;
    
    public:
    //Constructor and Destructors
    Student();
    ~Student();
    Student(const Student &obj);
    Student(string, string, unsigned int, float);

    //Setter and Getter
    void setId(unsigned int id);
    void setGpa(float gpa);
    void setSizeCourseStudent(unsigned int sizeCourseStudent);
    unsigned int getId();
    float getGpa();
    unsigned int getSizeCourseStudent();

    //Registering a course
    void enrollToCourse(Course &course);
};

Student::Student()
:Person()
{
    this->courseStudent = nullptr;
    this->sizeCourseStudent = 0;
    this->id = 0;
    this->gpa = 0;
}

Student::~Student()
{
    this->id = 0;
    this->gpa = 0;
    this->sizeCourseStudent = 0;
    delete[] courseStudent;
    this->courseStudent = nullptr;
}

Student::Student(const Student &obj)
:Person(obj)
{
    this->id = obj.id;
    this->gpa = obj.gpa;
    this->sizeCourseStudent = obj.sizeCourseStudent;

    this->courseStudent = new Course[this->sizeCourseStudent];

    for(unsigned short i = 0; i < this->sizeCourseStudent; i++)
        this->courseStudent[i] = obj.courseStudent[i];
}

Student::Student(string password, string username, unsigned int id, float gpa)
:Person(password, username)
{
    this->id = id;
    this->gpa = gpa;
    this->sizeCourseStudent = 0;
    this->courseStudent = nullptr;
};

void Student::setId(unsigned int id)
{
    this->id = id;
}

void Student::setGpa(float gpa)
{
    this->gpa = gpa;
}

void Student::setSizeCourseStudent(unsigned int sizeCourseStudent)
{
    this->sizeCourseStudent = sizeCourseStudent;
}

unsigned int Student::getId()
{
    return this->id;
}

float Student::getGpa()
{
    return this->gpa;
}

unsigned int Student::getSizeCourseStudent()
{
    return this->sizeCourseStudent;
}

void Student::enrollToCourse(Course &course)
{
    for(unsigned int i = 0; i < this->sizeCourseStudent; i++)
    {
        //Checking the course ids for seeing whether the student have this course
        if(this->courseStudent[i].getId() == course.getId())
        {
            cout << "You are already enrolled to this course!" << endl;
            return;
        }
    }

    //The size of the course is increased
    unsigned int sizeOfCourse = this->sizeCourseStudent + 1;

    Course* courses = new Course [sizeOfCourse];

    for(unsigned int i = 0; i < this->sizeCourseStudent; i++)
    {
        courses[i] = this->courseStudent[i]; //Setting the old values for allocated array of course
    }

    //Last element of course must be the course to be enrolled
    courses[this->sizeCourseStudent] = course;

    //Deleting old data
    delete[] this->courseStudent;

    //Setting the old data to the new data
    this->courseStudent = courses;
    this->sizeCourseStudent = sizeOfCourse;

    cout << "The enroll is completed!" << endl;
}

/*
1. Size of username (size_t)
2. Username (string)
3. Size of password (size_t)
4. Password (string)
5. Student ID (unsigned int)
6. GPA (float)
7. Size of Courses (size_t)
8. Course ID of all courses (unsigned int)
*/
void writeStudentFile(const string &fileName, Student student)
{
    ofstream file(fileName, ios::binary | ios::app);

    if(file.is_open())
    {
        size_t size = student.getUsername().size();
        file.write((char*)&size, sizeof(size_t));
        file.write(student.getUsername().data(), size);

        size = student.getPassword().size();
        file.write((char*)&size, sizeof(size_t));
        file.write(student.getPassword().data(), size);

        unsigned int id = student.getId();
        file.write((char*)&id, sizeof(unsigned int));

        float gpa = student.getGpa();
        file.write((char*)&gpa, sizeof(float));

        unsigned int sizeCourseProf = student.getSizeCourseStudent();
        file.write((char*)&sizeCourseProf, sizeof(unsigned int));

        unsigned int courseID = 0;
        for(unsigned int i = 0; i < sizeCourseProf; i++)
        {
            courseID = student.courseStudent[i].getId();
            file.write((char*)&courseID, sizeof(unsigned int));
        }
        file.close();
    }

    else
        cout << "Couldn't open the file!" << endl;
}

bool readStudentFile(const string &fileName, string username, string password, Student &student, bool isExistCheck)
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
        bool isExist = false; //The student exists or not

        while(p < end)
        {
            size_t sizeUsername = *((size_t*)p);
            p+=sizeof(size_t);

            char* bufferUsername = new char[sizeUsername + 1];

            for (size_t i = 0; i < sizeUsername; i++) 
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

            for (size_t i = 0; i < sizePassword; i++) 
            {
                bufferPassword[i] = *(p++);
            }

            bufferPassword[sizePassword] = '\0';
            string passwordString = bufferPassword;
            delete[] bufferPassword;

            //Checking whether the username and password is correct
            if (usernameString == username && passwordString == password)
            {
                isExist = true;

                unsigned int id = *((unsigned int*)p);
                p += sizeof(unsigned int);

                float gpa = *((float*)p);
                p += sizeof(float);

                unsigned int sizeOfCourses = *((unsigned int*)p);
                p+=sizeof(unsigned int);

                student.setUsername(usernameString);
                student.setPassword(passwordString);
                student.setId(id);
                student.setGpa(gpa);
                student.setSizeCourseStudent(sizeOfCourses);
                student.courseStudent = new Course[sizeOfCourses];
                for(unsigned int i = 0; i < sizeOfCourses; i++)
                {
                    unsigned int courseId = *((unsigned int*)p);
                    p+=sizeof(unsigned int);
                    student.courseStudent[i].setId(courseId);
                }

                break;
            }

            /*
            If the student is not the student in the file,
            just pass the other stages and check next student
            in the file is our student or not.
            */
            p += sizeof(int);
            p += sizeof(float);
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

/*
Stages of remove a data from binary file:

1. Open the original file in the reading mode.
2. Open a temporary file in the writing mode.
3. Read the original file and write the data that were read into temporary file.
4. Don't write the data that must be removed. (Skip the data that must be removed)
5. Delete the original file.
6. Change the name temporary file to the same name as the original file.
*/

void removeStudent(const string &fileName, const string &username)
{
    ifstream file(fileName, ios::binary | ios::ate); //Open the original
    ofstream temp("temp.bin", ios::binary); //Open the temporary

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
            //Read the original file
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

            unsigned int sizeOfCourses = *((unsigned int*)p);
            p += sizeof(unsigned int);

            Course* courses = new Course [sizeOfCourses];

            for(unsigned int i = 0; i < sizeOfCourses; i++)
            {
                unsigned int id = *((unsigned int*)p);
                p += sizeof(unsigned int);
                courses[i].setId(id);
            }

            //Check the data if these are the data that we want to remove
            if(username != usernameString)
            {
                //Write the data that were read
                temp.write((char*)&sizeUsername, sizeof(size_t));
                temp.write(usernameString.data(), sizeUsername);
                temp.write((char*)&sizePassword, sizeof(size_t));
                temp.write(passwordString.data(), sizePassword);
                temp.write((char*)&id, sizeof(unsigned int));
                temp.write((char*)&gpa, sizeof(float));
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

        remove(fileName.data()); //Delete original file
        rename("temp.bin", fileName.data()); //Change the name of the temporary file
    }

    else
    {
        cout << "Files couldn't opened!" << endl;
    }
}

/*
The stages for the updating data:
1. Remove old data
2. Write updated data
*/
void updateStudent(Student &student)
{
    removeStudent("studentdb.bin", student.getUsername());
    writeStudentFile("studentdb.bin", student);
}

Student *readAllStudents(const string &fileName, int &numberOfStudent)
{
    Student* tab;

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

            p+=sizeUsername;

            size_t sizePassword = *((size_t*)p);
            p+=sizeof(size_t);

            p+=sizePassword;

            p += sizeof(unsigned int);

            p += sizeof(float);

            unsigned int sizeOfCourses = *((unsigned int*)p);
            p+=sizeof(unsigned int);

            p+=sizeOfCourses * sizeof(unsigned int);
            numberOfStudent++;
        }

        p = mBlock;
        end = fileSize + mBlock;
        tab = new Student[numberOfStudent];

        for(int i = 0; i<numberOfStudent; i++)
        {
           
            size_t sizeUsername = *((size_t*)p);
            p+=sizeof(size_t);

            char* bufferUsername = new char[sizeUsername + 1];
        
            for (size_t i = 0; i < sizeUsername; ++i) {
                bufferUsername[i] = *p++;
            }

            bufferUsername[sizeUsername] = '\0';
            string usernameString = bufferUsername;
            delete[] bufferUsername;

            size_t sizePassword = *((size_t*)p);
            p+=sizeof(size_t);

            char* bufferPassword = new char[sizePassword + 1];

            for (size_t i = 0; i < sizePassword; ++i) {
                bufferPassword[i] = *p++;
            }

            bufferPassword[sizePassword] = '\0';
            string passwordString = bufferPassword;
            delete[] bufferPassword;

            unsigned int id = *((unsigned int*)p);
            p += sizeof(unsigned int);

            float gpa = *((float*)p);
            p += sizeof(float);

            unsigned int sizeOfCourses = *((unsigned int*)p);
            p+=sizeof(unsigned int);

            tab[i].setUsername(usernameString);
            tab[i].setPassword(passwordString);
            tab[i].setId(id);
            tab[i].setGpa(gpa);
            tab[i].setSizeCourseStudent(sizeOfCourses);
            tab[i].courseStudent = new Course[sizeOfCourses];
            for(unsigned int j = 0; j < sizeOfCourses; j++)
            {
                unsigned int id = *((unsigned int*)p);
                p += sizeof(unsigned int);
                tab[i].courseStudent[j].setId(id);
            }

        }
        delete[] mBlock;
    }
    else
        cout << "Couldn't open the file!" << endl;

    return tab;
}

/*
    A student Panel:
    1. Viewing course ID of students
    2. Displaying ID, name, and GPA
    3. Enrolling a course
    4. Back to main menu
    5. Exit the system
*/
void studentPanel(Student &student, bool &isSystemOpen)
{
    cout << "******************************************" << endl;
    cout << "Welcome to Student Panel!" << endl;
    cout << "******************************************" << endl;
    cout << "1. View Courses" << endl;
    cout << "2. View Personal Information" << endl;
    cout << "3. Enroll a course" << endl;
    cout << "4. Back to main menu" << endl;
    cout << "5. Exit the system" << endl;

    unsigned short number = 0;
    do
    {
        cout << "Enter the your choice: ";
        cin >> number;
    } while (number == 0  || number > 5); //Limit for cunning users :D
    
    if(number == 1)
    {
        cout << "Courses: " << endl;
        for(unsigned int i = 0; i < student.getSizeCourseStudent(); i++)
        {
            cout << "Course ID: " << student.courseStudent[i].getId() << endl;
        }
        return studentPanel(student, isSystemOpen);
    }

    if(number == 2)
    {
        cout << "Student ID: " << student.getId() << endl;
        cout << "Student Username: " << student.getUsername() << endl;
        cout << "Student GPA: " << student.getGpa() << endl;
        return studentPanel(student, isSystemOpen);
    }

    /*
        The stages for enrolling:
        1. Enter a course ID.
        2. Check if this course ID is in the course database
        3. If it is in the database, add the course into the array of the student course
        4. Update the student data
    */

    if(number == 3)
    {
        int courseID;

        cout << "Enter the course ID: ";
        cin >> courseID;

        Course course;
        bool isExist = readCourseFile("coursedb.bin", courseID, course);

        if(isExist)
        {
            student.enrollToCourse(course);
            updateStudent(student);
        }

        else
        {
            cout << "This course doens't exist!" << endl;
        }

        return studentPanel(student, isSystemOpen);
    }

    if(number == 5)
    {
        isSystemOpen = false;
    }
}

#endif