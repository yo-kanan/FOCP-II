#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <limits.h>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include<ctime>
using namespace std;

//Functions Prototype
bool dateValidator(string&);
void testAssistantFunctionality();


//Classes


class UniversitySystemException : public exception {
    private:
        string message;
        int errorCode;
    public:
        UniversitySystemException(string message = "University System Error", int errorCode = 0) {
            this->message = message;
            this->errorCode = errorCode;
        }
        
        virtual const char* what() const noexcept override {
            return message.c_str();
        }
            
        int getErrorCode() const {
            return errorCode;
        }
            
        virtual string getDetails() const {
            return "Error Code: " + to_string(errorCode) + ", Message: " + message;
        }
    };
           
class EnrollmentException : public UniversitySystemException {
    private:
        string studentID;
        string courseCode;
    public:
        EnrollmentException(string message, string studentID, string courseCode, int errorCode = 100) 
            : UniversitySystemException(message, errorCode) {
            this->studentID = studentID;
            this->courseCode = courseCode;
        }
            
        string getStudentID() const {
            return studentID;
        }
            
        string getCourseCode() const {
            return courseCode;
        }
            
        string getDetails() const override {
            return UniversitySystemException::getDetails() + 
                   ", Student ID: " + studentID + 
                   ", Course Code: " + courseCode;
        }
    };        
     
class CourseFullException : public EnrollmentException {
    private:
        int maxCapacity;
        int currentEnrollment;
    public:
        CourseFullException(string studentID, string courseCode, int maxCapacity, int currentEnrollment) 
            : EnrollmentException("Course is at maximum capacity", studentID, courseCode, 101) {
            this->maxCapacity = maxCapacity;
            this->currentEnrollment = currentEnrollment;
        }
            
        int getMaxCapacity() const {
            return maxCapacity;
        }
            
        int getCurrentEnrollment() const {
            return currentEnrollment;
        }
        
        string getDetails() const override {
            return EnrollmentException::getDetails() + 
                   ", Max Capacity: " + to_string(maxCapacity) + 
                   ", Current Enrollment: " + to_string(currentEnrollment);
        }
    };       
    
class EnrollmentDeadlineException : public EnrollmentException {
    private:
        string deadline;
        string currentDate;
    public:
        EnrollmentDeadlineException(string studentID, string courseCode, string deadline, string currentDate) 
            : EnrollmentException("Enrollment deadline has passed", studentID, courseCode, 103) {
            this->deadline = deadline;
            this->currentDate = currentDate;
        }
            
        string getDeadline() const {
            return deadline;
        }
            
        string getCurrentDate() const {
            return currentDate;
        }
            
        string getDetails() const override {
            return EnrollmentException::getDetails() + 
                   ", Deadline: " + deadline + 
                   ", Current Date: " + currentDate;
        }
    };
           
class GradeException : public UniversitySystemException {
    private:
        string studentID;
        string courseCode;
    public:
        GradeException(string message, string studentID, string courseCode, int errorCode = 200) 
            : UniversitySystemException(message, errorCode) {
            this->studentID = studentID;
            this->courseCode = courseCode;
        }
        
        string getStudentID() const {
            return studentID;
        }
        
        string getCourseCode() const {
            return courseCode;
        }
        
        string getDetails() const override {
            return UniversitySystemException::getDetails() + 
                   ", Student ID: " + studentID + 
                   ", Course Code: " + courseCode;
        }
    };
            
class InvalidGradeException : public GradeException {
    private:
        int invalidGrade;
        int minGrade;
        int maxGrade;
    public:
        InvalidGradeException(string studentID, string courseCode, int invalidGrade, int minGrade = 0, int maxGrade = 100) 
            : GradeException("Invalid grade value", studentID, courseCode, 201) {
            this->invalidGrade = invalidGrade;
            this->minGrade = minGrade;
            this->maxGrade = maxGrade;
        }
            
        int getInvalidGrade() const {
            return invalidGrade;
        }
            
        int getMinGrade() const {
            return minGrade;
        }
            
        int getMaxGrade() const {
            return maxGrade;
        }
        
        string getDetails() const override {
            return GradeException::getDetails() + 
                   ", Invalid Grade: " + to_string(invalidGrade) + 
                   ", Valid Range: " + to_string(minGrade) + "-" + to_string(maxGrade);
        }
    };
                
class RequirementsNotCompleteException : public GradeException {
    private:
        vector<string> missingRequirements;
    public:
        RequirementsNotCompleteException(string studentID, string courseCode, vector<string> missingReqs) 
            : GradeException("Student has not completed all requirements", studentID, courseCode, 203) {
            this->missingRequirements = missingReqs;
        }
        
        vector<string> getMissingRequirements() const {
            return missingRequirements;
        }
        
        string getDetails() const override {
            string details = GradeException::getDetails() + ", Missing Requirements: ";
            for (size_t i = 0; i < missingRequirements.size(); i++) {
                details += missingRequirements[i];
                if (i < missingRequirements.size() - 1) {
                    details += ", ";
                }
            }
            return details;
        }
    };
               
class PaymentException : public UniversitySystemException {
    private:
        string personID;
        double amount;
    public:
        PaymentException(string message, string personID, double amount, int errorCode = 300) 
            : UniversitySystemException(message, errorCode) {
            this->personID = personID;
            this->amount = amount;
        }
        
        string getPersonID() const {
            return personID;
        }
        
        double getAmount() const {
            return amount;
        }
        
        string getDetails() const override {
            return UniversitySystemException::getDetails() + 
                   ", Person ID: " + personID + 
                   ", Amount: " + to_string(amount);
        }
    };
               
class PaymentCalculationException : public PaymentException {
    private:
        string reason;
    public:
        PaymentCalculationException(string personID, double amount, string reason) 
               : PaymentException("Error calculating payment", personID, amount, 301) {
                this->reason = reason;
        }
            
        string getReason() const {
            return reason;
        }
            
        string getDetails() const override {
            return PaymentException::getDetails() + ", Reason: " + reason;
        }
    };
               
class InsufficientFundsException : public PaymentException {
    private:
        double availableFunds;
    public:
        InsufficientFundsException(string personID, double amount, double availableFunds) 
            : PaymentException("Insufficient funds for payment", personID, amount, 302) {
            this->availableFunds = availableFunds;
        }
            
        double getAvailableFunds() const {
            return availableFunds;
        }
            
        string getDetails() const override {
            return PaymentException::getDetails() + 
                   ", Available Funds: " + to_string(availableFunds) + 
                   ", Shortage: " + to_string(getAmount() - availableFunds);
        }
    };
               
class PersonException : public UniversitySystemException {
    private:
        string personID;
    public:
        PersonException(string message, string personID, int errorCode = 400) 
            : UniversitySystemException(message, errorCode) {
            this->personID = personID;
        }
        
        string getPersonID() const {
            return personID;
        }
        
        string getDetails() const override {
            return UniversitySystemException::getDetails() + ", Person ID: " + personID;
            }
    };
        
class InvalidIDException : public PersonException {
    private:
        string invalidID;
        string reason;
    public:
        InvalidIDException(string personID, string invalidID, string reason) 
            : PersonException("Invalid person ID", personID, 401) {
            this->invalidID = invalidID;
            this->reason = reason;
        }
        
        string getInvalidID() const {
            return invalidID;
        }
        
        string getReason() const {
            return reason;
        }
        
        string getDetails() const override {
            return PersonException::getDetails() + 
                   ", Invalid ID: " + invalidID + 
                   ", Reason: " + reason;
        }
    };
        
class InvalidContactInfoException : public PersonException {
    private:
        string invalidContact;
        string reason;
    public:
        InvalidContactInfoException(string personID, string invalidContact, string reason) 
            : PersonException("Invalid contact information", personID, 402) {
            this->invalidContact = invalidContact;
            this->reason = reason;
        }
        
        string getInvalidContact() const {
            return invalidContact;
        }
        
        string getReason() const {
            return reason;
        }
        
        string getDetails() const override {
            return PersonException::getDetails() + 
                   ", Invalid Contact: " + invalidContact + 
                   ", Reason: " + reason;
        }
    };
        



class Person{
        private:
            string name = "";
            int ID;
            long long int contact_info = 0;
            int ageID;
        public:
            Person() {};
            Person(string name, int ageId, int ID, long long int cInfo){
                this->name = name;
                this->ageID = ageId;
                this->ID = ID;
                this->contact_info = cInfo;
            }
        
            string getPersonName() {return name;}
            long long int getPersonContact_info() {return contact_info;}
            int getPersonID() {return ID;}
            int getPersonAgeID() {return ageID;}
        
            void setPersonName(string name){
                if (!name.empty()) {
                    this->name = name;
                    cout << "New Name set Successfully!\n";
                }
                else {
                    cout << "\nName Cant be empty!";
                    throw PersonException("Name cannot be empty", to_string(ID), 403);
                }
            }
            void setPersonAge(int age){
                if (age<0) { 
                    cout << "Become existant first! :)\nAge Invalid!\n"; 
                    throw PersonException("Age cannot be negative", to_string(ID), 404);
                }
                if (age>100) { 
                    cout << "Its about time to rest old Guy T_T\nAge Invalid\n"; 
                    throw PersonException("Age exceeds maximum allowed", to_string(ID), 405);
                }
                this->ageID = age;
                cout << "AgeID successfully Updated!\n";
            }
            void setPersonID(int ID){
                int cpy = ID, count = 0;
                while(cpy!=0) {
                    cpy /= 10; count++;
                }
        
                if (count != 5) {
                    cout << "Id can only be of 5 digits\nID not Set!\n";
                    throw InvalidIDException(to_string(this->ID), to_string(ID), "ID must be 5 digits");
                }
        
                this->ID = ID;
                cout << "Person ID updated Successfully!\n";
            }
            void setPersonContactInfo(long long int number){
                long long cpy = number, count = 0;
                while(cpy!=0) {
                    cpy /= 10; count++;
                }
        
                if (count != 10) {
                    cout << "Invalid Phone Number!\nDetails Not Updated!\n";
                    throw InvalidContactInfoException(to_string(ID), to_string(number), "Contact number must be 10 digits");
                }
        
                this->contact_info = number;
                cout << "Person Phone Number updated Successfully!\n";
            }
        
            
            virtual void displayDetails(){
                cout << "Person Details: \n";
                cout << "Name:\t" << name << '\n';
                cout << "AgeId:\t" << ageID << '\n';
                cout << "ID:\t" << ID << '\n';
                cout << "Contact No:\t" << contact_info << '\n';
            }
        
            virtual double calculatePayment(){
                return 0.0;
            }
        };
void testPerson(Person* person); //Another Function Prototype HEHEHEHEHE


class Student : public Person{
    private:
        string enrollment_date = "", program = "";
        double GPA = 0;
    public:
        Student(){}
        Student(string enrollment, string program, double GPA){
            this->enrollment_date = enrollment;
            this->program = program;
            this->GPA = GPA;
        }
    
        string getStudentEnrollmentDate() {return enrollment_date;}
        string getStudentProgram() {return program;}
        double getStudentGPA() {return GPA;}
    
        void setStudentEnrollmentDate(string date){
            if (dateValidator(date)){
                this->enrollment_date = date;
                cout << "Date Format is Correct!\nStudent Enrollment Date Updated Successfully!\n";
            }
            else {
                cout << "Date Format Invalid!\n 1. It should be : DD/MM/YY\n2. All Date, Month, Year should be Valid\nEnrollment Date Not Updated!\n";
                throw UniversitySystemException("Invalid enrollment date format", 500);
            }
        }
        void setStudentProgram(string p){
            if (!p.empty()) {
                this->program = p;
                cout << "Student Program Updated Successfully!\n";
            } else {
                cout << "Program Cant be empty!\nProgram not updated!\n";
                throw UniversitySystemException("Program cannot be empty", 501);
            }
        }
        void setStudentGPA(double GPA){
            if (GPA>=0 && GPA<=4){
                this->GPA = GPA;
                cout << "Student GPA Updated Successfully!\n";
            } else {
                cout << "GPA should only be between 0-4!\nGPA not updated!\n";
                throw GradeException("Invalid GPA value", to_string(getPersonID()), "", 202);
            }
        }
    
    
        void displayDetails() override{
            Person::displayDetails();
            cout << "Student Details: \n";
            cout << "Enrollment Date:\t" << enrollment_date << '\n';
            cout << "Program:\t" << program << '\n';
            cout << "GPA:\t" << GPA << '\n';
        }
    
        double calculatePayment(double panelty=0){
            return 154500 + panelty;
        }
    };

class UndergraduateStudent : public Student {
        private:
            string major = "";
            string minor = "";
            string expected_graduation_date = "";
        public:
            UndergraduateStudent() {}
            UndergraduateStudent(string enrollment, string program, double GPA, 
                                string major, string minor, string expected_graduation_date) : Student(enrollment, program, GPA) {
                this->major = major;
                this->minor = minor;
                this->expected_graduation_date = expected_graduation_date;
            }
            
            string getUndergraduateStudentMajor() { return major; }
            string getUndergraduateStudentMinor() { return minor; }
            string getUndergraduateStudentExpectedGraduationDate() { return expected_graduation_date; }
            
            void setUndergraduateStudentMajor(string major) {
                if (!major.empty()) {
                    this->major = major;
                    cout << "Undergraduate Student Major Updated Successfully!\n";
                } else {
                    cout << "Major Cannot be Empty!\nMajor not Updated!\n";
                    throw UniversitySystemException("Major cannot be empty", 502);
                }
            }
            
            void setUndergraduateStudentMinor(string minor) {
                if (!minor.empty()) {
                    this->minor = minor;
                    cout << "Undergraduate Student Minor Updated Successfully!\n";
                } else {
                    cout << "Minor Cannot be Empty!\nMinor not Updated!\n";
                    throw UniversitySystemException("Minor cannot be empty", 503);
                }
            }
            
            void setUndergraduateStudentExpectedGraduationDate(string date) {
                if (dateValidator(date)) {
                    this->expected_graduation_date = date;
                    cout << "Date Format is Correct!\nExpected Graduation Date Updated Successfully!\n";
                } else {
                    cout << "Date Format Invalid!\n 1. It should be : DD/MM/YY\n2. All Date, Month, Year should be Valid\nExpected Graduation Date Not Updated!\n";
                    throw UniversitySystemException("Invalid graduation date format", 504);
                }
            }
            
            void displayDetails() override {
                Student::displayDetails();
                cout << "Undergraduate Student Details: \n";
                cout << "Major:\t" << major << '\n';
                cout << "Minor:\t" << minor << '\n';
                cout << "Expected Graduation Date:\t" << expected_graduation_date << '\n';
            }
            
            double calculatePayment() override {
                return Student::calculatePayment() + 1;
            }
        };

class Professor : public Person{
        private:
                string department = "", hireDate = "", specialization = "";
        public:
                Professor(){}
                Professor(string department, string hireDate, string specialization){
                    this->department = department;
                    this->hireDate = hireDate;
                    this->specialization = specialization;
                }
                
                string getProfessorDepartment() {return department;}
                string getProfessorHireDate() {return hireDate;}
                string getProfessorSpecialization() {return specialization;}
                
                void setProfessorDepartment(string department){
                    if (!department.empty()){
                        this->department = department;
                        cout << "Professor Department Set Successfully!\n";
                    }else {
                        cout << "Department Can't Be Empty!\nDepartment not Updated!\n";
                        throw UniversitySystemException("Department cannot be empty", 505);
                    }
                }
                void setHireDate(string date){
                    if (dateValidator(date)){
                        this->hireDate = date;
                        cout << "Date Format is Correct!\nProfessor Hire Date Updated Successfully!\n";
                    }
                    else {
                        cout << "Date Format Invalid!\n 1. It should be : DD/MM/YY\n2. All Date, Month, Year should be Valid\nHire Date Not Updated!\n";
                        throw UniversitySystemException("Invalid hire date format", 506);
                    }
                }
                void setProfessorSpecialization(string specialization){
                    if (!specialization.empty()){
                        this->specialization = specialization;
                        cout << "Professor Specialization Updated Successfully!\n";
                    } else {
                        cout << "Specialization Can not Be Empty!\n";
                        throw UniversitySystemException("Specialization cannot be empty", 507);
                    }
                }
                
                
                void displayDetails() override{
                    Person::displayDetails();
                    cout << "Professor Details: \n";
                    cout << "Department:\t" << department << '\n';
                    cout << "Hire Date:\t" << hireDate << '\n';
                    cout << "Specialization:\t" << specialization << '\n';
                }
                
                double calculatePayment(double bonus=0){
                    return 100500 + bonus;
                }
            };

class GraduateStudent : public Student {
private:
    string research_topic = "";
    Professor* advisor = nullptr;
    string thesis_title = "";
    bool teaching_assistantship = false;
    bool research_assistantship = false;
    double assistantship_stipend = 0.0;
    int teaching_hours = 0;
    int research_hours = 0;
public:
    GraduateStudent() {}
    GraduateStudent(string enrollment, string program, double GPA,
                  string research_topic, string thesis_title) : Student(enrollment, program, GPA) {
        this->research_topic = research_topic;
        this->thesis_title = thesis_title;
        this->teaching_assistantship = false;
        this->research_assistantship = false;
        this->assistantship_stipend = 0.0;
        this->teaching_hours = 0;
        this->research_hours = 0;
    }
    
    string getGraduateStudentResearchTopic() { return research_topic; }
    Professor* getGraduateStudentAdvisor() { return advisor; }
    string getGraduateStudentThesisTitle() { return thesis_title; }
    bool getTeachingAssistantship() { return teaching_assistantship; }
    bool getResearchAssistantship() { return research_assistantship; }
    double getAssistantshipStipend() { return assistantship_stipend; }
    int getTeachingHours() { return teaching_hours; }
    int getResearchHours() { return research_hours; }
    
    void setGraduateStudentResearchTopic(string research_topic) {
        if (!research_topic.empty()) {
            this->research_topic = research_topic;
            cout << "Graduate Student Research Topic Updated Successfully!\n";
        } else {
            cout << "Research Topic Cannot be Empty!\nResearch Topic not Updated!\n";
            return;
            }
    }
    
    void setGraduateStudentAdvisor(Professor* professor) {
        this->advisor = professor;
        cout << "Graduate Student Advisor Updated Successfully!\n";
    }
    
    void setGraduateStudentThesisTitle(string thesis_title) {
        if (!thesis_title.empty()) {
            this->thesis_title = thesis_title;
            cout << "Graduate Student Thesis Title Updated Successfully!\n";
        } else {
            cout << "Thesis Title Cannot be Empty!\nThesis Title not Updated!\n";
            return;
        }
    }
    
    void assignTeachingAssistantship(double stipend, int hours) {
        if(stipend < 0 || hours < 0) {
            cout << "Stipend and hours must be positive values!\nTeaching Assistantship not assigned!\n";
            return;
        }
        this->teaching_assistantship = true;
        this->assistantship_stipend += stipend;
        this->teaching_hours = hours;
        cout << "Teaching Assistantship Assigned Successfully!\n";
    }
    
    void assignResearchAssistantship(double stipend, int hours) {
        if(stipend < 0 || hours < 0) {
            cout << "Stipend and hours must be positive values!\nResearch Assistantship not assigned!\n";
            return;
        }
        this->research_assistantship = true;
        this->assistantship_stipend += stipend;
        this->research_hours = hours;
        cout << "Research Assistantship Assigned Successfully!\n";
    }
    
    void removeTeachingAssistantship() {
        if(!teaching_assistantship) {
            cout << "Student does not have a Teaching Assistantship!\n";
            return;
        }
        this->teaching_assistantship = false;
        this->assistantship_stipend -= (assistantship_stipend * teaching_hours) / (teaching_hours + research_hours);
        this->teaching_hours = 0;
        cout << "Teaching Assistantship Removed Successfully!\n";
    }
    
    void removeResearchAssistantship() {
        if(!research_assistantship) {
            cout << "Student does not have a Research Assistantship!\n";
            return;
        }
        this->research_assistantship = false;
        this->assistantship_stipend -= (assistantship_stipend * research_hours) / (teaching_hours + research_hours);
        this->research_hours = 0;
        cout << "Research Assistantship Removed Successfully!\n";
    }
    
    void updateAssistantshipHours(int teaching_hours, int research_hours) {
        if(teaching_hours < 0 || research_hours < 0) {
            cout << "Hours must be positive values!\nHours not updated!\n";
            return;
        }
        this->teaching_hours = teaching_hours;
        this->research_hours = research_hours;
        cout << "Assistantship Hours Updated Successfully!\n";
    }
    
    void displayDetails() override {
        Student::displayDetails();
        cout << "Graduate Student Details: \n";
        cout << "Research Topic:\t" << research_topic << '\n';
        cout << "Thesis Title:\t" << thesis_title << '\n';
        if (advisor != nullptr) {
            cout << "Advisor:\t" << advisor->getPersonName() << '\n';
        } else {
            cout << "No Advisor Assigned\n";
        }
        cout << "Teaching Assistantship:\t" << (teaching_assistantship ? "Yes" : "No") << '\n';
        if(teaching_assistantship) {
            cout << "Teaching Hours:\t" << teaching_hours << '\n';
        }
        cout << "Research Assistantship:\t" << (research_assistantship ? "Yes" : "No") << '\n';
        if(research_assistantship) {
            cout << "Research Hours:\t" << research_hours << '\n';
        }
        if(teaching_assistantship || research_assistantship) {
            cout << "Total Assistantship Stipend:\t" << assistantship_stipend << '\n';
        }
    }
    
    double calculatePayment() override {
        double base_payment = Student::calculatePayment();
        double assistantship_payment = assistantship_stipend;
        double research_bonus = research_assistantship ? 5000.0 : 0.0;
        return base_payment + assistantship_payment + research_bonus;
    }
};

class AssistantProfessor : public Professor {
    private:
        int contract_years = 0;
        int papers_published = 0;
        bool promotion_eligible = false;
    public:
        AssistantProfessor() {}
        AssistantProfessor(string department, string hireDate, string specialization,
                           int contract_years, int papers_published, bool promotion_eligible) : Professor(department, hireDate, specialization) {
            this->contract_years = contract_years;
            this->papers_published = papers_published;
            this->promotion_eligible = promotion_eligible;
        }
        
        int getAssistantProfessorContractYears() { return contract_years; }
        int getAssistantProfessorPapersPublished() { return papers_published; }
        bool getAssistantProfessorPromotionEligible() { return promotion_eligible; }
        
        void setAssistantProfessorContractYears(int years) {
            if (years >= 0) {
                this->contract_years = years;
                cout << "Assistant Professor Contract Years Updated Successfully!\n";
            } else {
                cout << "Contract Years Cannot be Negative!\nContract Years not Updated!\n";
                throw UniversitySystemException("Contract years cannot be negative", 514);
            }
        }
        
        void setAssistantProfessorPapersPublished(int count) {
            if (count >= 0) {
                this->papers_published = count;
                cout << "Assistant Professor Papers Published Updated Successfully!\n";
            } else {
                cout << "Papers Published Cannot be Negative!\nPapers Published not Updated!\n";
                throw UniversitySystemException("Papers published cannot be negative", 515);
            }
        }
        
        void setAssistantProfessorPromotionEligible(bool status) {
            this->promotion_eligible = status;
            cout << "Assistant Professor Promotion Eligibility Updated Successfully!\n";
        }
        
        void displayDetails() override {
            Professor::displayDetails();
            cout << "Assistant Professor Details: \n";
            cout << "Contract Years:\t" << contract_years << '\n';
            cout << "Papers Published:\t" << papers_published << '\n';
            cout << "Promotion Eligible:\t" << (promotion_eligible ? "Yes" : "No") << '\n';
        }
        
        double calculatePayment() override {
            try {
                double base = Professor::calculatePayment();
                double paper_bonus = papers_published * 5000;
                double promotion_bonus = promotion_eligible ? 10000 : 0;
                double year_multiplier = contract_years > 0 ? contract_years * 0.05 : 0;
                
                return base + paper_bonus + promotion_bonus + (base * year_multiplier);
            } catch (...) {
                throw PaymentCalculationException(to_string(getPersonID()), 0, "Error calculating assistant professor payment");
            }
        }
    };   

class AssociateProfessor : public Professor {
        private:
            int teaching_years = 0;
            int projects_done = 0;
            int students_guided = 0;
        public:
            AssociateProfessor() {}
            AssociateProfessor(string department, string hireDate, string specialization,
                          int teaching_years, int projects_done, int students_guided) : Professor(department, hireDate, specialization) {
                this->teaching_years = teaching_years;
                this->projects_done = projects_done;
                this->students_guided = students_guided;
            }
            
            int getAssociateProfessorTeachingYears() { return teaching_years; }
            int getAssociateProfessorProjectsDone() { return projects_done; }
            int getAssociateProfessorStudentsGuided() { return students_guided; }
            
            void setAssociateProfessorTeachingYears(int years) {
                if (years >= 0) {
                    this->teaching_years = years;
                    cout << "Associate Professor Teaching Years Updated Successfully!\n";
                } else {
                    cout << "Teaching Years Cannot be Negative!\nTeaching Years not Updated!\n";
                    throw UniversitySystemException("Teaching years cannot be negative", 516);
                }
            }
            
            void setAssociateProfessorProjectsDone(int projects) {
                if (projects >= 0) {
                    this->projects_done = projects;
                    cout << "Associate Professor Projects Done Updated Successfully!\n";
                } else {
                    cout << "Projects Done Cannot be Negative!\nProjects Done not Updated!\n";
                    throw UniversitySystemException("Projects done cannot be negative", 517);
                }
            }
            
            void setAssociateProfessorStudentsGuided(int students) {
                if (students >= 0) {
                    this->students_guided = students;
                    cout << "Associate Professor Students Guided Updated Successfully!\n";
                } else {
                    cout << "Students Guided Cannot be Negative!\nStudents Guided not Updated!\n";
                    throw UniversitySystemException("Students guided cannot be negative", 518);
                }
            }
            
            void displayDetails() override {
                Professor::displayDetails();
                cout << "Associate Professor Details: \n";
                cout << "Teaching Years:\t" << teaching_years << '\n';
                cout << "Projects Done:\t" << projects_done << '\n';
                cout << "Students Guided:\t" << students_guided << '\n';
            }
            
            double calculatePayment() override {
                try {
                    double base = Professor::calculatePayment();
                    double year_bonus = teaching_years * 2000;
                    double project_bonus = projects_done * 10000;
                    double student_bonus = students_guided * 1000;
                    double experience_multiplier = teaching_years > 5 ? 0.1 : 0;
                    
                    return base + year_bonus + project_bonus + student_bonus + (base * experience_multiplier);
                } catch (...) {
                    throw PaymentCalculationException(to_string(getPersonID()), 0, "Error calculating associate professor payment");
                }
            }
            
        };    

class FullProfessor : public Professor {
            private:
                int work_years = 0;
                int foreign_papers = 0;
                bool is_head = false;
            public:
                FullProfessor() {}
                FullProfessor(string department, string hireDate, string specialization,
                        int work_years, int foreign_papers, bool is_head) : Professor(department, hireDate, specialization) {
                    this->work_years = work_years;
                    this->foreign_papers = foreign_papers;
                    this->is_head = is_head;
                    }
                
                int getFullProfessorWorkYears() { return work_years; }
                int getFullProfessorForeignPapers() { return foreign_papers; }
                bool getFullProfessorIsHead() { return is_head; }
                
                void setFullProfessorWorkYears(int years) {
                    if (years >= 0) {
                        this->work_years = years;
                        cout << "Full Professor Work Years Updated Successfully!\n";
                    } else {
                        cout << "Work Years Cannot be Negative!\nWork Years not Updated!\n";
                        throw UniversitySystemException("Work years cannot be negative", 519);
                    }
                }
                
                void setFullProfessorForeignPapers(int papers) {
                    if (papers >= 0) {
                        this->foreign_papers = papers;
                        cout << "Full Professor Foreign Papers Updated Successfully!\n";
                    } else {
                        cout << "Foreign Papers Cannot be Negative!\nForeign Papers not Updated!\n";
                        throw UniversitySystemException("Foreign papers cannot be negative", 520);
                    }
                }
                
                void setFullProfessorIsHead(bool status) {
                    this->is_head = status;
                    cout << "Full Professor Head Status Updated Successfully!\n";
                }
                
                void displayDetails() override {
                    Professor::displayDetails();
                    cout << "Full Professor Details: \n";
                    cout << "Work Years:\t" << work_years << '\n';
                    cout << "Foreign Papers:\t" << foreign_papers << '\n';
                    cout << "Is Head:\t" << (is_head ? "Yes" : "No") << '\n';
                }
                
                double calculatePayment() override {
                    try {
                        double base = Professor::calculatePayment();
                        double year_bonus = work_years * 5000;
                        double paper_bonus = foreign_papers * 1000;
                        double head_bonus = is_head ? 50000 : 0;
                        double seniority_multiplier = work_years > 10 ? 0.2 : 0;
                        
                        return base + year_bonus + paper_bonus + head_bonus + (base * seniority_multiplier);
                    } catch (...) {
                        throw PaymentCalculationException(to_string(getPersonID()), 0, "Error calculating full professor payment");
                    }
                }
            };
            

class Course{
                private:
                    string code = "", title = "", description = "";
                    double credits = 0;
                    Professor* instructor = nullptr;
                    vector<Student*> enrolled_students;
                    int max_capacity = 30;
                    string enrollment_deadline = "31/12/23";
                public:
                    Course(){}
                    Course(string code, string title, string description, double credits, int max_capacity = 30){
                        this->code = code;
                        this->title = title;
                        this->description = description;
                        this->credits = credits;
                        this->max_capacity = max_capacity;
                    }
                
                    string getCourseCode() {return code;}
                    string getCourseTitle() {return title;}
                    string getCourseDescription() {return description;}
                    double getCourseCredits() {return credits;}
                    Professor* getCourseInstructor() {return instructor;}
                    vector<Student*> getEnrolledStudents() {return enrolled_students;}
                    int getMaxCapacity() {return max_capacity;}
                    string getEnrollmentDeadline() {return enrollment_deadline;}
                
                    void setCourseCode(string code) {
                        if (!code.empty()) {
                            this->code = code;
                            cout << "Course Code Updated Successfully!\n";
                        } else {
                            cout << "Course Code Cannot be Empty!\nCode not Updated!\n";
                            return;
                        }
                    }
                    
                    void setCourseTitle(string title){
                        if (!title.empty()) {
                            this->title = title;
                            cout << "Course Title Updated Successfully!\n";
                        } else {
                            cout << "Course Title Cannot be Empty!\nTitle not Updated!\n";
                            return;
                        }
                    }
                    
                    void setCourseDescription(string description){
                        this->description = description;
                        cout << "Course Description Updated Successfully!\n";
                    }
                    
                    void setCourseCredits(double credits){
                        if (credits > 0){
                            this->credits = credits;
                            cout << "Course Credits Updated Successfully!\n";
                        }
                        else {
                            cout << "Credits Must be Positive!\nCourse Credits Not Updated!\n";
                            return;
                        }
                    }
                    
                    void setMaxCapacity(int capacity) {
                        if (capacity > 0) {
                            this->max_capacity = capacity;
                            cout << "Course Maximum Capacity Updated Successfully!\n";
                        } else {
                            cout << "Capacity Must be Positive!\nCapacity not Updated!\n";
                            return;
                        }
                    }
                    
                    void setEnrollmentDeadline(string deadline) {
                        if (dateValidator(deadline)) {
                            this->enrollment_deadline = deadline;
                            cout << "Course Enrollment Deadline Updated Successfully!\n";
                        } else {
                            cout << "Date Format Invalid!\nEnrollment Deadline not Updated!\n";
                            return;
                        }
                    }
                    
                    void setInstructor(Professor* professor) {
                        this->instructor = professor;
                        cout << "Course Instructor Set Successfully!\n";
                    }
                    
                    void enrollStudent(Student* student, string current_date) {
                        try {
                            if (enrolled_students.size() >= max_capacity) {
                                throw CourseFullException(to_string(student->getPersonID()), code, max_capacity, enrolled_students.size());
                            }
                            
                            if (!dateValidator(current_date)) {
                                throw EnrollmentException("Invalid current date format", to_string(student->getPersonID()), code, 102);
                            }
                            
                            if (current_date > enrollment_deadline) {
                                throw EnrollmentDeadlineException(to_string(student->getPersonID()), code, enrollment_deadline, current_date);
                            }
                            
                            for (auto s : enrolled_students) {
                                if (s->getPersonID() == student->getPersonID()) {
                                    cout << "Student Already Enrolled in this Course!\n";
                                    return;
                                }
                            }
                            
                            enrolled_students.push_back(student);
                            cout << "Student Enrolled in Course Successfully!\n";
                        } catch (const UniversitySystemException& e) {
                            cout << "Enrollment Error: " << e.what() << endl;
                            cout << e.getDetails() << endl;
                            writeToErrorLog(e.getDetails());
                        }
                    }
                    
                    void dropStudent(int studentID) {
                        try {
                            for (int i = 0; i < enrolled_students.size(); i++) {
                                if (enrolled_students[i]->getPersonID() == studentID) {
                                    enrolled_students.erase(enrolled_students.begin() + i);
                                    cout << "Student Dropped from Course Successfully!\n";
                                    return;
                                }
                            }
                            throw EnrollmentException("Student not found in course", to_string(studentID), code, 104);
                        } catch (const UniversitySystemException& e) {
                            cout << "Drop Student Error: " << e.what() << endl;
                            cout << e.getDetails() << endl;
                            writeToErrorLog(e.getDetails());
                        }
                    }
                    
                    void displayCourseDetails() {
                        cout << "Course Details:\n";
                        cout << "Code: " << code << "\n";
                        cout << "Title: " << title << "\n";
                        cout << "Description: " << description << "\n";
                        cout << "Credits: " << credits << "\n";
                        cout << "Max Capacity: " << max_capacity << "\n";
                        cout << "Enrollment Deadline: " << enrollment_deadline << "\n";
                        
                        if (instructor != nullptr) {
                            cout << "Instructor: " << instructor->getPersonName() << "\n";
                        } else {
                            cout << "No Instructor Assigned\n";
                        }
                        
                        cout << "Enrolled Students: " << enrolled_students.size() << "\n";
                    }
                
                    void displayEnrolledStudents() {
                        if (enrolled_students.empty()) {
                            cout << "No Students Enrolled in this Course!\n";
                            return;
                        }
                        cout << "Students Enrolled in " << title << ":\n";
                        for (auto student : enrolled_students) {
                            cout << "ID: " << student->getPersonID() << ", Name: " << student->getPersonName() << "\n";
                        }
                    }
                    
                    void writeToErrorLog(const string& errorDetails) {
                        ofstream logFile("university_system_error.log", ios::app);
                        if (logFile.is_open()) {
                            time_t now = time(0);
                            char* dt = ctime(&now);
                            logFile << "[" << dt << "] " << errorDetails << endl;
                            logFile.close();
                        } else {
                            cout << "Warning: Unable to open error log file!" << endl;
                        }
                    }
                };

class Department{
private:
    string name = "", location = "";
    double budget;
    vector<Professor*> professors;
public:
    Department(){}
    Department(string name, string location, double budget){
        this->name = name;
        this->location = location;
        this->budget = budget;
    }

    string getDepartmentName() {return name;}
    string getDepartmentLocation() {return location;}
    double getDepartmentBudget() {return budget;}
    vector<Professor*> getDepartmentProfessors() {return professors;}

    void setDepartmentName(string name) {
        this->name = name;
        cout << "Department Name Updated Successfully!\n";
    }
    void setDepartmentLocation(string location){
        this->location = location;
        cout << "Department Location Updated Successfully!\n";
    } 
    void setDepartmentBudget(double budget){
        if (budget>0) {
            this->budget = budget;
            cout << "Department Budget Updated Successfully!\n";
        }
        else {
            cout << "Budget Should be greater then 0!\n Budget not Updated!"; return;
        }
    }
    void addProfessor(Professor* professor) {
        professors.push_back(professor);
        cout << "Professor Added to Department Successfully!\n";
    }

    void removeProfessor(int professorID) {
        for (int i = 0; i < professors.size(); i++) {
            if (professors[i]->getPersonID() == professorID) {
                professors.erase(professors.begin() + i);
                cout << "Professor Removed from Department Successfully!\n";
                return;
            }
        }
        cout << "Professor Not Found in Department!\n";
    }

    void displayProfessors() {
        if (professors.empty()) {
            cout << "No Professors in this Department!\n";
            return;
        }
        cout << "Professors in Department " << name << ":\n";
        for (auto prof : professors) {
            prof->displayDetails();
            cout << "------------------------\n";
        }
    }
};



class GradeBook{
private:
    map<string, int> studentGrades;
    string courseCode = "";
public:
    GradeBook(){}

    void addGrade(string ID, int Grade){
        if (Grade <= 100 && Grade >= 0){
            studentGrades[ID] = Grade;
            cout << "Grade of Student ID: " << ID << " Updated Successfully!\n";
        } else {
            cout << "Invalid Grades!\n Student Grade Not Updated!\n";
            return;
        }
    }

    double averageGrade(){
        if (studentGrades.empty()){ return 0;}

        double sum = 0;
        for (auto s : studentGrades){
            sum += s.second;
        }

        return sum/studentGrades.size();
    }

    int HighestMarks(){
        if (studentGrades.empty()) return 0;
        int max = INT_MIN;
        for (auto p : studentGrades){
            if (max < p.second) max = p.second;
        }

        return max;
    }

    vector<string> failingStudents(double PassMarks = 33.5){
        if (studentGrades.empty()) return {};
        vector<string> results;

        for (auto p : studentGrades){
            if (p.second < PassMarks){
                results.push_back(p.first);
            }
        }

        return results;
    }

    void showGrades(){
        if (studentGrades.empty()){
            cout << "Empty Grades!\n";
            return;
        }

        cout << "Student ID\tGrades\n";
        for (auto p : studentGrades){
            cout << p.first << '\t' << p.second << '\n';
        }
    }
};

class EnrollmentManager{
private:
    map<string, vector<string>> enrolledStudents;
public:
    EnrollmentManager(){}

    void enrollStudent(string courseCode, string ID){
        if (enrolledStudents.find(courseCode) != enrolledStudents.end()){
            for (auto c : enrolledStudents[courseCode]){
                if (c==ID){
                    cout << "Student Already Enrolled!\n";
                    return;
                }
            }
        }
        enrolledStudents[courseCode].push_back(ID);
        cout << "Student of Student ID: " << ID << " enrolled in courseCode: " << courseCode << " succesfully!\n";
    }

    void dropStudent(string courseCode, string ID){
        if (enrolledStudents.find(courseCode) != enrolledStudents.end()){
            if (find(enrolledStudents[courseCode].begin(), enrolledStudents[courseCode].end(), ID) != enrolledStudents[courseCode].end()){
                auto& vec = enrolledStudents[courseCode];
                vec.erase(remove(vec.begin(), vec.end(), ID), vec.end());
                cout << "Student Removed Successfully!\n";
                return;
            } else {
                cout << "Student Already Not Enrolled!\n"; return;
            }
        } else {
            cout << "Course Not Found!\n";
            return;
        }
    }

    int getEnrollmentCount(string courseCode){
        if (enrolledStudents.find(courseCode) != enrolledStudents.end()){
            return enrolledStudents[courseCode].size();
        }
        return 0;
    }

    vector<string> getEnrolledStudents(string courseCode){
        vector<string> result;

        if (enrolledStudents.find(courseCode) != enrolledStudents.end()){
            for (auto c : enrolledStudents[courseCode]){
                result.push_back(c);
            }
        }   
        else return {};
        return result;
    }

};


class University {
private:
    string name = "";
    string location = "";
    vector<Department*> departments;
public:
    University() {}
    University(string name, string location) {
        this->name = name;
        this->location = location;
    }
    
    string getUniversityName() { return name; }
    string getUniversityLocation() { return location; }
    vector<Department*> getUniversityDepartments() { return departments; }
    
    void setUniversityName(string name) {
        if (!name.empty()) {
            this->name = name;
            cout << "University Name Updated Successfully!\n";
        } else {
            cout << "University Name Cannot be Empty!\nName not Updated!\n";
        }
    }
    
    void setUniversityLocation(string location) {
        if (!location.empty()) {
            this->location = location;
            cout << "University Location Updated Successfully!\n";
        } else {
            cout << "University Location Cannot be Empty!\nLocation not Updated!\n";
        }
    }
    
    void addDepartment(Department* department) {
        departments.push_back(department);
        cout << "Department Added to University Successfully!\n";
    }
    
    void removeDepartment(string departmentName) {
        for (int i = 0; i < departments.size(); i++) {
            if (departments[i]->getDepartmentName() == departmentName) {
                departments.erase(departments.begin() + i);
                cout << "Department Removed from University Successfully!\n";
                return;
                }
        }
        cout << "Department Not Found in University!\n";
    }
    
    vector<Professor*> getAllProfessors() {
        vector<Professor*> allProfessors;
        for (auto dept : departments) {
            vector<Professor*> deptProfessors = dept->getDepartmentProfessors();
            allProfessors.insert(allProfessors.end(), deptProfessors.begin(), deptProfessors.end());
        }
        return allProfessors;
    }
    
    void displayDepartments() {
        if (departments.empty()) {
            cout << "No Departments in University!\n";
            return;
        }
        cout << "Departments in " << name << ":\n";
        for (auto dept : departments) {
            cout << "Name: " << dept->getDepartmentName() << ", Location: " << dept->getDepartmentLocation() << "\n";
        }
    }
    
    void displayAllProfessors() {
        vector<Professor*> allProfessors = getAllProfessors();
        if (allProfessors.empty()) {
            cout << "No Professors in University!\n";
            return;
        }
        cout << "All Professors in " << name << ":\n";
        for (auto prof : allProfessors) {
            cout << "ID: " << prof->getPersonID() << ", Name: " << prof->getPersonName() << ", Department: " << prof->getProfessorDepartment() << "\n";
        }
    }
};

class Classroom {
private:
    string room_number = "";
    string building = "";
    int capacity = 0;
    bool has_projector = false;
public:
    Classroom() {}
    Classroom(string room_number, string building, int capacity, bool has_projector) {
        this->room_number = room_number;
        this->building = building;
        this->capacity = capacity;
        this->has_projector = has_projector;
    }
    
    string getClassroomRoomNumber() { return room_number; }
    string getClassroomBuilding() { return building; }
    int getClassroomCapacity() { return capacity; }
    bool getClassroomHasProjector() { return has_projector; }
    
    void setClassroomRoomNumber(string room_number) {
        if (!room_number.empty()) {
            this->room_number = room_number;
            cout << "Classroom Room Number Updated Successfully!\n";
        } else {
            cout << "Room Number Cannot be Empty!\nRoom Number not Updated!\n";
        }
    }
    
    void setClassroomBuilding(string building) {
        if (!building.empty()) {
            this->building = building;
            cout << "Classroom Building Updated Successfully!\n";
        } else {
            cout << "Building Cannot be Empty!\nBuilding not Updated!\n";
        }
    }
    
    void setClassroomCapacity(int capacity) {
        if (capacity > 0) {
            this->capacity = capacity;
            cout << "Classroom Capacity Updated Successfully!\n";
        } else {
            cout << "Capacity Must be Positive!\nCapacity not Updated!\n";
        }
    }
    
    void setClassroomHasProjector(bool has_projector) {
        this->has_projector = has_projector;
        cout << "Classroom Projector Status Updated Successfully!\n";
    }

    void displayClassroomDetails() {
        cout << "Classroom Details:\n";
        cout << "Room Number: " << room_number << "\n";
        cout << "Building: " << building << "\n";
        cout << "Capacity: " << capacity << "\n";
        cout << "Has Projector: " << (has_projector ? "Yes" : "No") << "\n";
    }
};

class Schedule {
private:
    struct TimeSlot {
        string day;
        string start_time;
        string end_time;
        Course* course;
        Classroom* classroom;
    };
    vector<TimeSlot> time_slots;
public:
    Schedule() {}
    
    void addTimeSlot(string day, string start_time, string end_time, Course* course, Classroom* classroom) {      
        
        for (auto slot : time_slots) {
            if (slot.day == day && slot.classroom == classroom) {
                if ((start_time >= slot.start_time && start_time < slot.end_time) || (end_time > slot.start_time && end_time <= slot.end_time) ||
                    (start_time <= slot.start_time && end_time >= slot.end_time)) {
                    cout << "Time Slot Conflict in Room " << classroom->getClassroomRoomNumber() << "!\nTime Slot Not Added!\n";
                    return;
                }
            }
        }
            
        TimeSlot new_slot = {day, start_time, end_time, course, classroom};
        time_slots.push_back(new_slot);
        cout << "Time Slot Added Successfully!\n";
    }
    
    void removeTimeSlot(string day, string start_time, Course* course) {
        for (int i = 0; i < time_slots.size(); i++) {
            if (time_slots[i].day == day && 
                time_slots[i].start_time == start_time && 
                time_slots[i].course == course) {
                time_slots.erase(time_slots.begin() + i);
                cout << "Time Slot Removed Successfully!\n";
                return;
            }
        }
        cout << "Time Slot Not Found!\n";
    }
    
    vector<TimeSlot> getCourseSchedule(Course* course) {
        vector<TimeSlot> course_slots;
        for (auto slot : time_slots) {
            if (slot.course == course) {
                course_slots.push_back(slot);
            }
         }
        return course_slots;
    }
    
    vector<TimeSlot> getClassroomSchedule(Classroom* classroom) {
        vector<TimeSlot> classroom_slots;
        for (auto slot : time_slots) {
            if (slot.classroom == classroom) {
                classroom_slots.push_back(slot);
            }
        }
        return classroom_slots;
    }
    
    void displaySchedule() {
        if (time_slots.empty()) {
            cout << "No Time Slots in Schedule!\n";
            return;
        }
        cout << "Schedule:\n";
        cout << "Day\tStart\tEnd\tCourse\tRoom\n";
        for (auto slot : time_slots) {
            cout << slot.day << "\t" 
                 << slot.start_time << "\t" 
                 << slot.end_time << "\t" 
                 << slot.course->getCourseCode() << "\t" 
                 << slot.classroom->getClassroomRoomNumber() << "\n";
        }
    }
};







int main(){
    Person person1; Person person2;
    person1.setPersonAge(10); person2.setPersonContactInfo(9817256938);


    Student Student1; Student Student2;
    Student1.getPersonAgeID();  Student2.setPersonAge(20);


    Professor pr1; Professor pr2;
    pr1.setPersonName("Aj"); pr2.setPersonAge(99);

    Course c1; Course c2;
    c1.setCourseCode("MAL151"); c2.setCourseTitle("Very Bad course T_T");


    Department d1; Department d2;
    d1.setDepartmentBudget(1001); d2.setDepartmentLocation("Bahadurgarh");


    // Part C, point 5
    vector<Person*> people;
    

    Student* s1 = new Student("15/09/2024", "Computer Science", 3.8);
    s1->setPersonName("Kanan");
    s1->setPersonID(12345);
    s1->setPersonAge(18);
    s1->setPersonContactInfo(9817256938);
    
    Student* s2 = new Student("15/02/2023", "Physics", 3.5);
    s2->setPersonName("Aditi");
    s2->setPersonID(54321);
    s2->setPersonAge(16);
    s2->setPersonContactInfo(1234567890);
    

    Professor* p1 = new Professor("Computer Science", "01/06/2020", "Machine Learning");
    p1->setPersonName("Richa Singh");
    p1->setPersonID(98765);
    p1->setPersonAge(22);
    p1->setPersonContactInfo(5555555555);
    
    Professor* p2 = new Professor("Mathematics", "15/07/2018", "Stats");
    p2->setPersonName("Rajni Mam");
    p2->setPersonID(56789);
    p2->setPersonAge(99);
    p2->setPersonContactInfo(6666666666);
    

    people.push_back(s1);
    people.push_back(p1);
    people.push_back(s2);
    people.push_back(p2);
    
    cout << "\n------ Polymorphism Behaviour Yayyyyy ------\n";
    for (auto person : people) {
        testPerson(person);
        cout << "--------------------------------------------------\n";
    }
    
    for (auto person : people) {
        delete person;
    }

    testAssistantFunctionality();
    return 0;
}


//Functions
bool dateValidator(string &date){
    vector<string> seperated_dates;
    string temp = "";

    for (auto c : date){
        if (c == '/'){
            seperated_dates.push_back(temp);
            temp = "";
        } else {
            temp += c;
        }
    }
    seperated_dates.push_back(temp);

    if (seperated_dates.size() != 3) return false;

    int day, month, year;
    try{
        day = stoi(seperated_dates[0]);
        month = stoi(seperated_dates[1]);
        year = stoi(seperated_dates[2]);
    }catch (...){
        return false;
    }

    if (day<1 || day>31) return false;
    if (month<1 || month>12) return false;
    
    return true;
}

void testPerson(Person* person){
    person->displayDetails();
    cout << "Person's Payment: " << person->calculatePayment() << endl;
}

void testAssistantFunctionality(){
    GraduateStudent* gradStudent = new GraduateStudent("10/01/2023", "Computer Science PhD", 3.9, "Machine Learning", "Neural Networks for Image Recognition");
    gradStudent->setPersonName("Kanan");
    gradStudent->setPersonID(12345);
    gradStudent->setPersonAge(18);
    gradStudent->setPersonContactInfo(9876543210);
    
    Professor* prof = new FullProfessor("Computer Science", "01/01/2010", "Artificial Intelligence", 15, 30, true);
    prof->setPersonName("Richa Singh");
    prof->setPersonID(54321);
    
    gradStudent->setGraduateStudentAdvisor(prof);
    gradStudent->assignTeachingAssistantship(10000, 20);
    gradStudent->assignResearchAssistantship(15000, 30);
    
    cout << "\n------ Graduate Student with Assistantships ------\n";
    gradStudent->displayDetails();
    cout << "Total Payment: " << gradStudent->calculatePayment() << endl;
    
    gradStudent->removeTeachingAssistantship();
    cout << "\n------ After Removing Teaching Assistantship ------\n";
    gradStudent->displayDetails();
    cout << "Total Payment: " << gradStudent->calculatePayment() << endl;
    
    delete gradStudent;
    delete prof;
}
