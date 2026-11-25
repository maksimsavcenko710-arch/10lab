#include <iostream>
#include <string>
#include <iomanip>
using namespace std;
enum Speciality
{
    KN,
    Inf,
    MatEconom,
    FizInform,
    Trudove
};
struct Student
{
    string prizv;
    int kurs;
    Speciality speciality;
    int Fizika;
    int Matematika;
    int Informatika;
};
void Create(Student *p, const int N);
void Vidminno(Student *p, const int N);
int FiveFizika(Student *p, const int N);
void print(const Student *p, const int N);
int main()
{
    int N;
    cout << "Введіть кількість студентів: ";
    cin >> N;

    Student *p = new Student[N];
    Create(p, N);
    Vidminno(p, N);
    int count = FiveFizika(p, N);
    cout << "Кількість студентів, які мають оцінку 5 з фізики: " << count << endl;
    print(p, N);
    delete[] p;
    return 0;
}

void Create(Student *p, const int N)
{
    int speciality;
    for (int i = 0; i < N; i++)
    {
        cout << "Введіть дані студента " << i + 1 << ":" << endl;
        cout << "Прізвище: " << endl;
        cin.ignore(10000, '\n');
        getline(cin, p[i].prizv);
        cout << "Курс: " << endl;
        cin >> p[i].kurs;
        cout << "Спеціальність (0 - Компютерні науки, 1 - Інформатика, 2 - Математика та економіка, 3 - Фізика та інформатика, 4 - Трудове навчання): ";
        cin >> speciality;
        p[i].speciality = (Speciality)speciality;
        cout << "Оцінка з фізики: ";
        cin >> p[i].Fizika;
        cout << "Оцінка з математики: ";
        cin >> p[i].Matematika;
        cout << "Оцінка з інформатики: ";
        cin >> p[i].Informatika;
        cout << endl;
    }
}

void Vidminno(Student *p, const int N)
{
    cout << "Студенти, які мають не погані оцінки з усіх предметів:" << endl;
    for (int i = 0; i < N; i++)
    {
        if (p[i].Fizika > 3 && p[i].Matematika > 3 && p[i].Informatika > 3)
        {
            cout << p[i].prizv << endl;
        }
    }
}

int FiveFizika(Student *p, const int N)
{
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        if (p[i].Fizika == 5)
        {
            count++;
        }
    }
    return count;
}
void print(const Student *p, const int N)
{
    cout << "СПИСОК СТУДЕНТІВ" << endl;
    cout << string(80, '=') << endl;
    cout << "| №  | Курс | Фіз | Мат | Інф | Прізвище, спеціальність" << endl;
    for (int i = 0; i < N; i++)
    {
        cout << "| " << setw(2) << left << i + 1
             << " | " << setw(4) << left << p[i].kurs
             << " | " << setw(3) << left << p[i].Fizika
             << " | " << setw(3) << left << p[i].Matematika
             << " | " << setw(3) << left << p[i].Informatika
             << " | " << p[i].prizv << ", ";

        switch (p[i].speciality)
        {
        case KN:
            cout << "Комп'ютерні науки";
            break;
        case Inf:
            cout << "Інформатика";
            break;
        case MatEconom:
            cout << "Математика та економіка";
            break;
        case FizInform:
            cout << "Фізика та інформатика";
            break;
        case Trudove:
            cout << "Трудове навчання";
            break;
        }
        cout << endl;
    }
    cout << string(80, '=') << endl;
}