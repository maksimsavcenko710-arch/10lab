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
    union // третя оцінка: одна пам'ять, три назви
    {
        int Prog;       // KN
        int ChysMetody; // Inf
        int Pedagogika; // MatEconom, FizInform, Trudove
    };
};

void Create(Student *p, const int N);
void Print(const Student *p, const int N);
int ThirdGrade(const Student &s);
void PrintBezTrijok(const Student *p, const int N);
int FiveFizika(const Student *p, const int N);
int Len(const string &s);
void PrintCell(const string &s, const int width);
string SpecName(const Speciality s);

int main()
{
    int N;
    cout << "Введіть кількість студентів: ";
    cin >> N;

    Student *p = new Student[N];

    Create(p, N);
    Print(p, N);
    PrintBezTrijok(p, N);

    int kilkist = FiveFizika(p, N);
    cout << "Кількість студентів, які мають оцінку 5 з фізики: " << kilkist << endl;

    delete[] p;
    return 0;
}

// введення даних; третя оцінка залежить від спеціальності
void Create(Student *p, const int N)
{
    int spec;
    for (int i = 0; i < N; i++)
    {
        cout << "Введіть дані студента " << i + 1 << ":" << endl;

        cin.ignore(10000, '\n');
        cout << "Прізвище: ";
        getline(cin, p[i].prizv);

        cout << "Курс: ";
        cin >> p[i].kurs;

        cout << "Спеціальність (0 - Комп'ютерні науки, 1 - Інформатика, "
                "2 - Математика та економіка, 3 - Фізика та інформатика, "
                "4 - Трудове навчання): ";
        cin >> spec;
        p[i].speciality = (Speciality)spec;

        cout << "Оцінка з фізики: ";
        cin >> p[i].Fizika;
        cout << "Оцінка з математики: ";
        cin >> p[i].Matematika;

        switch (p[i].speciality)
        {
        case KN:
            cout << "Оцінка з програмування: ";
            cin >> p[i].Prog;
            break;
        case Inf:
            cout << "Оцінка з чисельних методів: ";
            cin >> p[i].ChysMetody;
            break;
        default:
            cout << "Оцінка з педагогіки: ";
            cin >> p[i].Pedagogika;
            break;
        }
        cout << endl;
    }
}

// повертає третю оцінку студента, обираючи потрібне поле за спеціальністю
int ThirdGrade(const Student &s)
{
    switch (s.speciality)
    {
    case KN:
        return s.Prog;
    case Inf:
        return s.ChysMetody;
    default:
        return s.Pedagogika;
    }
}

int Len(const string &s)
{
    int n = 0;
    for (unsigned char c : s)
        if ((c & 0xC0) != 0x80)
            n++;
    return n;
}

void PrintCell(const string &s, const int width)
{
    cout << s;
    int spaces = width - Len(s);
    if (spaces > 0)
        cout << string(spaces, ' ');
}

string SpecName(const Speciality s)
{
    switch (s)
    {
    case KN:
        return "Комп'ютерні науки";
    case Inf:
        return "Інформатика";
    case MatEconom:
        return "Математика та економіка";
    case FizInform:
        return "Фізика та інформатика";
    default:
        return "Трудове навчання";
    }
}

// таблиця з 9 клітинок; з трьох останніх заповнена лише одна
void Print(const Student *p, const int N)
{
    const int W = 90;
    cout << "СПИСОК СТУДЕНТІВ" << endl;
    cout << string(W, '=') << endl;

    cout << "| ";
    PrintCell("№", 3);
    cout << " | ";
    PrintCell("Прізвище", 14);
    cout << " | ";
    PrintCell("Курс", 4);
    cout << " | ";
    PrintCell("Спеціальність", 24);
    cout << " | ";
    PrintCell("Фіз", 3);
    cout << " | ";
    PrintCell("Мат", 3);
    cout << " | ";
    PrintCell("Прог", 4);
    cout << " | ";
    PrintCell("ЧМ", 4);
    cout << " | ";
    PrintCell("Пед", 3);
    cout << " |" << endl;
    cout << string(W, '-') << endl;

    for (int i = 0; i < N; i++)
    {
        string prog = "", chm = "", ped = "";
        switch (p[i].speciality)
        {
        case KN:
            prog = to_string(p[i].Prog);
            break;
        case Inf:
            chm = to_string(p[i].ChysMetody);
            break;
        default:
            ped = to_string(p[i].Pedagogika);
            break;
        }

        cout << "| " << setw(3) << left << i + 1 << " | ";
        PrintCell(p[i].prizv, 14);
        cout << " | " << setw(4) << left << p[i].kurs << " | ";
        PrintCell(SpecName(p[i].speciality), 24);
        cout << " | " << setw(3) << left << p[i].Fizika
             << " | " << setw(3) << left << p[i].Matematika << " | ";
        PrintCell(prog, 4);
        cout << " | ";
        PrintCell(chm, 4);
        cout << " | ";
        PrintCell(ped, 3);
        cout << " |" << endl;
    }
    cout << string(W, '=') << endl
         << endl;
}

// виводить прізвища студентів без трійок
void PrintBezTrijok(const Student *p, const int N)
{
    cout << "Студенти, які вчаться без трійок:" << endl;
    for (int i = 0; i < N; i++)
        if (p[i].Fizika > 3 && p[i].Matematika > 3 && ThirdGrade(p[i]) > 3)
            cout << "  " << p[i].prizv << endl;
    cout << endl;
}

// повертає кількість студентів з оцінкою 5 з фізики
int FiveFizika(const Student *p, const int N)
{
    int k = 0;
    for (int i = 0; i < N; i++)
        if (p[i].Fizika == 5)
            k++;
    return k;
}