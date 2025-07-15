#ifndef COURSE_H
#define COURSE_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Course
{
    //members
    private:
    string name;
    unsigned int id;

    public:
    //Constructor and Destructors
    Course(); //Constructor without parameters
    ~Course();  //Destructor
    Course(const Course &obj); //Copy Constructor
    Course(string, int);  //Constructor with parameters

    //Getters and Setters
    string getName();
    int getId();
    void setId(int id);
    void setName(string name);
};

Course::Course()
{
    this->name = "";
    this->id = 0;
}

Course::~Course()
{

}

Course::Course(const Course &obj)
{
    this->name = obj.name;
    this->id = obj.id;
}

Course::Course(string name, int id)
{
    this->name = name;
    this->id = id;
}

string Course::getName()
{
    return this->name;
}

int Course::getId()
{
    return this->id;
}

void Course::setId(int id)
{
    this->id = id;
}

void Course::setName(string name)
{
    this->name = name;
}

/*
1. Size of Name (size_t)
2. Name (string)
3. Course Id (int)
*/
void writeCourseFile(const string &fileName, Course &course)
{
    ofstream file(fileName, ios::binary | ios::app);

    if(file.is_open())
    {
        size_t size = course.getName().size();
        file.write((char*)&size, sizeof(size_t));
        file.write(course.getName().data(), size);

        unsigned int id = course.getId();
        file.write((char*)&id, sizeof(unsigned int));
        
        file.close();
    }

    else
        cout << "Couldn't open the file!" << endl;
}

/*
1. Size of Name (size_t)
2. Name (string)
3. Course Id (unsigned int)
*/
bool readCourseFile(const string &fileName, unsigned int CourseID, Course &course)
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
        char* end = mBlock + fileSize; //Limiting for the pointer to memory block
        bool isExist = false; //Checking that there is a course with determined ID

        /*
        while(fileSize < 0)
        fileSize -= sizeof(???)

        is also method for limiting pointer
        */

        while(p < end)
        {
            size_t sizeName = *((size_t*)p);
            p+=sizeof(size_t);

            //Allocating buffer to storing the name
            char* bufferName = new char[sizeName + 1];

            //Filling the buffer with characters
            for (size_t i = 0; i < sizeName; ++i) 
            {
                bufferName[i] = *(p++);
            }
            
            //Last buffer is for null terminator becuase c strings must have null terminator
            bufferName[sizeName] = '\0';
            string nameString = bufferName; //Setting the string to buffer
            delete[] bufferName;
    
            unsigned int fileCourseID = *((unsigned int*)p);
            p += sizeof(unsigned int);
            
            //Checking that courseID in the file equals to determined course ID
            if(fileCourseID == CourseID)
            {
                isExist = true;
                course.setId(fileCourseID);
                course.setName(nameString);
                
                break;
            }
        }
        delete[] mBlock;
        return isExist;
    }
    else
        cout << "Couldn't open the file!" << endl;

    return false;
}

Course* readAllCourseFile(const string &fileName, unsigned int &numberOfCourses, bool &isExist)
{
    Course* tab;
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

        if(fileSize != 0)
            isExist = true;


        //Reading Part for incrementing number of courses
        while(p < end)
        {
            size_t sizeName = *((size_t*)p);
            p+=sizeof(size_t);

            char* bufferName = new char[sizeName + 1];

            for (size_t i = 0; i < sizeName; ++i) {
                bufferName[i] = *(p++);
            }

            bufferName[sizeName] = '\0';
            string nameString = bufferName;
            delete[] bufferName;

            p += sizeof(unsigned int);
            numberOfCourses++;
        }

        p = mBlock;
        tab = new Course[numberOfCourses];

        //Assigning Part
        for(unsigned int i = 0; i < numberOfCourses; i++)
        {
            //Course Name
            size_t sizeName = *((size_t*)p);
            p+=sizeof(size_t);

            char* bufferName = new char[sizeName + 1];
        
            for (size_t i = 0; i < sizeName; ++i) {
                bufferName[i] = *(p++);
            }

            bufferName[sizeName] = '\0';
            string nameString = bufferName;
            delete[] bufferName;

            //Course ID
            unsigned int id = *((unsigned int*)p);
            p += sizeof(unsigned int);

            tab[i].setId(id);
            tab[i].setName(nameString);
        }
        delete[] mBlock;
    }
    else
        cout << "Couldn't open the file!" << endl;
    return tab;
}

void coursePanel()
{
    unsigned short number = 0;

    cout << "******************************************" << endl;
    cout << "Welcome to Course Panel!" << endl;
    cout << "******************************************" << endl;
    cout << "1. View Courses" << endl;

    do
    {
        if(number == 0 || number > 1)
        {
            cout << "Please enter 1" << endl;
        }
        cout << "Enter the your choice: ";
        cin >> number;
    } while (number == 0 || number > 1);
    
    if(number == 1)
    {
        unsigned int numberOfCourses = 0;
        bool isExist = false; //Checker for seeing whether the "coursedb.bin" is empry or not
        Course* allCourses = readAllCourseFile("coursedb.bin", numberOfCourses, isExist);
        if(isExist)
        {
            cout << "******************************************" << endl;
            for(unsigned int i = 0; i < numberOfCourses; i++)
            {
                cout << i+1 << ". Course: "<< allCourses[i].getName() << ", " << allCourses[i].getId() << endl;
            }
        }
        else
            cout << "Course not found!" << endl;

        delete[] allCourses;
    }

}

unsigned int CourseNumber(const string &fileName)
{
    unsigned int count = 0;

    ifstream file(fileName, ios::binary | ios:: ate);
    streampos fileSize = file.tellg();
    file.seekg(0, ios::beg);
    char* mBlock = new char[fileSize];
    file.read(mBlock, fileSize);
    file.close();
    char* p = mBlock;
    char* end = mBlock + fileSize;

    if(file.is_open())
    {
        while(p < end)
        {
            //Course Name
            size_t sizeName = *((size_t*)p);
            p+=sizeof(size_t);
            char* bufferName = new char[sizeName + 1];

            for (size_t i = 0; i < sizeName; ++i) {
                bufferName[i] = *p++;
            }

            bufferName[sizeName] = '\0';
            string nameString = bufferName;
            delete[] bufferName;
            
            p += sizeof(unsigned int);

            count++;
        }
    }

    return count;
}

#endif