#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

int main() {
    int numCourses;
    cout << "Enter the number of courses taken: ";
    cin >> numCourses;

    vector<string> courseNames(numCourses);
    vector<double> grades(numCourses);
    vector<int> creditHours(numCourses);
    double totalGradePoints = 0.0;
    int totalCredits = 0;

    for (int i = 0; i < numCourses; i++) {
        cout << "Enter the name of course " << (i + 1) << ": ";
        cin >> courseNames[i];
        cout << "Enter the grade for " << courseNames[i] << ": ";
        cin >> grades[i];
        cout << "Enter the credit hours for " << courseNames[i] << ": ";
        cin >> creditHours[i];

        totalGradePoints += grades[i] * creditHours[i];
        totalCredits += creditHours[i];
    }

    double GPA = totalGradePoints / totalCredits;
    cout << fixed << setprecision(2);
    cout << "GPA for the semester: " << GPA << endl;

    // Assuming CGPA is calculated as the same for simplicity
    double CGPA = GPA; // In a real scenario, CGPA would be calculated over multiple semesters
    cout << "Overall CGPA: " << CGPA << endl;

    cout << "Individual Course Grades:" << endl;
    for (int i = 0; i < numCourses; i++) {
        cout << courseNames[i] << ": " << grades[i] << " (Credit Hours: " << creditHours[i] << ")" << endl;
    }

    return 0;
}
