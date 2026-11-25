#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <limits>
using namespace std;

// Спеціальності: порядок у переліку = алфавітний порядок їхніх назв,
// тому порівняння значень переліку дорівнює порівнянню назв
enum Specialnist
{
    INFORMATYKA,
    KOMP_NAUKY,
    MATEM_EKON,
    TRUDOVE,
    FIZ_INF
};
const string specStr[] = {"Інформатика", "Комп'ютерні науки",
                          "Математика та економіка", "Трудове навчання",
                          "Фізика та інформатика"};
const int SPEC_COUNT = 5;

struct Student
{
    string prizv;
    int kurs;
    Specialnist spec;
    int fizyka;
    int matem;
    union // третя оцінка залежить від спеціальності
    {
        int programuvannya; // Комп'ютерні науки
        int chyselniMetody; // Інформатика
        int pedagogika;     // решта спеціальностей
    };
};

// ---------- допоміжні функції введення ----------
int ReadInt(const string &prompt, int min, int max);
double ReadAvg(const string &prompt);
string ReadString(const string &prompt);

// ---------- робота з даними ----------
int Third(const Student &s);
double Avg(const Student &s);
void Create(Student *p, const int N);
void Print(const Student *p, const int N);
void PrintIndexed(const Student *p, const int *I, const int N);
int CmpKey(const Student &s, Specialnist spec, double avg, const string &prizv);
int Cmp(const Student &a, const Student &b);
void PhysicalSort(Student *p, const int N);
void BuildIndex(const Student *p, int *I, const int N);
int BinSearch(const Student *p, const int N, const string &prizv,
              Specialnist spec, double avg);
int BinSearchIndexed(const Student *p, const int *I, const int N,
                     const string &prizv, Specialnist spec, double avg);
void Search(const Student *p, const int *I, const int N, bool useIndex);

int main()
{
    int N = ReadInt("Введіть кількість студентів N: ", 1, 1000);
    Student *p = new Student[N];
    int *I = new int[N];
    for (int i = 0; i < N; i++)
        I[i] = i;

    int menuItem;
    do
    {
        cout << "\nВиберіть дію:\n\n"
             << " [1] - введення даних з клавіатури\n"
             << " [2] - вивід даних на екран\n"
             << " [3] - фізичне впорядкування даних\n"
             << " [4] - побудова індексного масиву\n"
             << " [5] - вивід даних через індексний масив\n"
             << " [6] - бінарний пошук у фізично впорядкованому масиві\n"
             << " [7] - бінарний пошук через індексний масив\n\n"
             << " [0] - вихід\n\n";
        menuItem = ReadInt("Введіть значення: ", 0, 7);
        cout << endl;

        switch (menuItem)
        {
        case 1:
            Create(p, N);
            BuildIndex(p, I, N);
            break;
        case 2:
            Print(p, N);
            break;
        case 3:
            PhysicalSort(p, N);
            BuildIndex(p, I, N);
            break;
        case 4:
            BuildIndex(p, I, N);
            break;
        case 5:
            PrintIndexed(p, I, N);
            break;
        case 6:
            Search(p, I, N, false);
            break;
        case 7:
            Search(p, I, N, true);
            break;
        }
    } while (menuItem != 0);

    delete[] p;
    delete[] I;
    return 0;
}

int ReadInt(const string &prompt, int min, int max)
{
    int x;
    while (true)
    {
        cout << prompt;
        if (cin >> x && x >= min && x <= max)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        if (cin.eof())
            exit(0);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Помилка! Введіть ціле число від " << min << " до " << max << endl;
    }
}

double ReadAvg(const string &prompt)
{
    double x;
    while (true)
    {
        cout << prompt;
        if (cin >> x && x >= 0 && x <= 5)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return round(x * 100) / 100;
        }
        if (cin.eof())
            exit(0);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Помилка! Введіть число від 0 до 5" << endl;
    }
}

string ReadString(const string &prompt)
{
    string s;
    do
    {
        cout << prompt;
        getline(cin, s);
        if (cin.eof())
            exit(0);
        if (s.empty())
            cout << "  Помилка! Рядок не може бути порожнім" << endl;
    } while (s.empty());
    return s;
}

// третя оцінка: яке саме поле об'єднання активне, визначає спеціальність
int Third(const Student &s)
{
    switch (s.spec)
    {
    case KOMP_NAUKY:
        return s.programuvannya;
    case INFORMATYKA:
        return s.chyselniMetody;
    default:
        return s.pedagogika;
    }
}

// середній бал, округлений до сотих (щоб порівняння дійсних чисел було надійним)
double Avg(const Student &s)
{
    return round((s.fizyka + s.matem + Third(s)) / 3.0 * 100) / 100;
}

void Create(Student *p, const int N)
{
    for (int i = 0; i < N; i++)
    {
        cout << "Студент № " << i + 1 << ":" << endl;
        p[i].prizv = ReadString("    прізвище: ");
        p[i].kurs = ReadInt("    курс (1-6): ", 1, 6);
        cout << "    спеціальність:" << endl;
        for (int k = 0; k < SPEC_COUNT; k++)
            cout << "        " << k << " - " << specStr[k] << endl;
        p[i].spec = (Specialnist)ReadInt("    виберіть (0-4): ", 0, SPEC_COUNT - 1);
        p[i].fizyka = ReadInt("    оцінка з фізики (1-5): ", 1, 5);
        p[i].matem = ReadInt("    оцінка з математики (1-5): ", 1, 5);
        switch (p[i].spec)
        {
        case KOMP_NAUKY:
            p[i].programuvannya = ReadInt("    оцінка з програмування (1-5): ", 1, 5);
            break;
        case INFORMATYKA:
            p[i].chyselniMetody = ReadInt("    оцінка з чисельних методів (1-5): ", 1, 5);
            break;
        default:
            p[i].pedagogika = ReadInt("    оцінка з педагогіки (1-5): ", 1, 5);
        }
        cout << endl;
    }
}

static void PrintHeader()
{
    cout << "===========================================================================================================================\n"
         << "| №   | Прізвище         | Курс | Спеціальність            | Фізика | Матем. | Програм. | Числ.мет. | Педагог. |\n"
         << "---------------------------------------------------------------------------------------------------------------------------\n";
}

// setw рахує байти, а кирилиця в UTF-8 займає 2 байти — тому вирівнюємо вручну
static void PrintPadded(const string &s, int width)
{
    int len = 0;
    for (unsigned char c : s)
        if ((c & 0xC0) != 0x80)
            len++;
    cout << s << string(max(0, width - len), ' ');
}

static void PrintRow(const Student &s, int num)
{
    cout << "| " << setw(3) << right << num << " | ";
    PrintPadded(s.prizv, 16);
    cout << " | " << setw(4) << s.kurs << " | ";
    PrintPadded(specStr[s.spec], 24);
    cout << " | " << setw(6) << s.fizyka
         << " | " << setw(6) << s.matem
         << " | ";
    // клітинка заповнена лише для третього предмета своєї спеціальності
    cout << setw(8) << right;
    if (s.spec == KOMP_NAUKY)
        cout << s.programuvannya;
    else
        cout << "";
    cout << " | " << setw(9);
    if (s.spec == INFORMATYKA)
        cout << s.chyselniMetody;
    else
        cout << "";
    cout << " | " << setw(8);
    if (s.spec != KOMP_NAUKY && s.spec != INFORMATYKA)
        cout << s.pedagogika;
    else
        cout << "";
    cout << " |\n";
}

void Print(const Student *p, const int N)
{
    PrintHeader();
    for (int i = 0; i < N; i++)
        PrintRow(p[i], i + 1);
    cout << "===========================================================================================================================\n";
}

void PrintIndexed(const Student *p, const int *I, const int N)
{
    PrintHeader();
    for (int i = 0; i < N; i++)
        PrintRow(p[I[i]], i + 1);
    cout << "===========================================================================================================================\n";
}

// порівнює студента s з ключем (спеціальність, середній бал, прізвище): -1, 0, 1
int CmpKey(const Student &s, Specialnist spec, double avg, const string &prizv)
{
    if (s.spec < spec)
        return -1;
    if (s.spec > spec)
        return 1;
    double a = Avg(s);
    if (a < avg)
        return -1;
    if (a > avg)
        return 1;
    if (s.prizv < prizv)
        return -1;
    if (s.prizv > prizv)
        return 1;
    return 0;
}

int Cmp(const Student &a, const Student &b)
{
    return CmpKey(a, b.spec, Avg(b), b.prizv);
}

void PhysicalSort(Student *p, const int N)
{
    for (int i0 = 0; i0 < N - 1; i0++)
        for (int i1 = 0; i1 < N - i0 - 1; i1++)
            if (Cmp(p[i1], p[i1 + 1]) > 0)
                swap(p[i1], p[i1 + 1]); // міняємо місцями самі структури
}

void BuildIndex(const Student *p, int *I, const int N)
{
    for (int i = 0; i < N; i++)
        I[i] = i;
    for (int i0 = 0; i0 < N - 1; i0++)
        for (int i1 = 0; i1 < N - i0 - 1; i1++)
            if (Cmp(p[I[i1]], p[I[i1 + 1]]) > 0)
                swap(I[i1], I[i1 + 1]); // міняємо місцями лише індекси
}

int BinSearch(const Student *p, const int N, const string &prizv,
              Specialnist spec, double avg)
{
    int L = 0, R = N - 1;
    while (L <= R)
    {
        int m = (L + R) / 2;
        int c = CmpKey(p[m], spec, avg, prizv);
        if (c == 0)
            return m;
        if (c < 0)
            L = m + 1;
        else
            R = m - 1;
    }
    return -1;
}

int BinSearchIndexed(const Student *p, const int *I, const int N,
                     const string &prizv, Specialnist spec, double avg)
{
    int L = 0, R = N - 1;
    while (L <= R)
    {
        int m = (L + R) / 2;
        int c = CmpKey(p[I[m]], spec, avg, prizv);
        if (c == 0)
            return m;
        if (c < 0)
            L = m + 1;
        else
            R = m - 1;
    }
    return -1;
}

void Search(const Student *p, const int *I, const int N, bool useIndex)
{
    cout << "Введіть ключі пошуку:\n";
    string prizv = ReadString("    прізвище: ");
    for (int k = 0; k < SPEC_COUNT; k++)
        cout << "        " << k << " - " << specStr[k] << endl;
    Specialnist spec = (Specialnist)ReadInt("    спеціальність (0-4): ", 0, SPEC_COUNT - 1);
    double avg = ReadAvg("    середній бал (0-5): ");

    int found = useIndex ? BinSearchIndexed(p, I, N, prizv, spec, avg)
                         : BinSearch(p, N, prizv, spec, avg);
    if (found != -1)
        cout << "Студента знайдено в позиції " << found + 1 << endl;
    else
        cout << "Такого студента не знайдено" << endl;
}