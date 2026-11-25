// Google Test для 10.2B.cpp
#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include <vector>

#ifndef PROG_SOURCE
#define PROG_SOURCE "10.2B.cpp"   // CMake підставляє справжнє ім'я файлу
#endif
#define main program_main         // щоб main з програми не конфліктував з main від gtest_main
#include PROG_SOURCE
#undef main

namespace
{
    // Третя оцінка записується у поле union, яке відповідає спеціальності
    Student makeStudent(const string &name, int kurs, Specialnist spec, int f, int m, int third)
    {
        Student st;
        st.prizv = name;
        st.kurs = kurs;
        st.spec = spec;
        st.fizyka = f;
        st.matem = m;
        switch (spec)
        {
        case KOMP_NAUKY:
            st.programuvannya = third;
            break;
        case INFORMATYKA:
            st.chyselniMetody = third;
            break;
        default:
            st.pedagogika = third;
            break;
        }
        return st;
    }

    // Невпорядкований набір з 5 студентів.
    // Очікуваний порядок після сортування (спеціальність, середній бал, прізвище):
    // Alpha, Beta, Gamma (Інформатика), Delta (Комп'ютерні науки), Zeta (Трудове)
    void fillSample(Student *p)
    {
        p[0] = makeStudent("Zeta", 1, TRUDOVE, 4, 4, 4);       // 4.00
        p[1] = makeStudent("Beta", 1, INFORMATYKA, 5, 5, 5);   // 5.00
        p[2] = makeStudent("Alpha", 1, INFORMATYKA, 3, 3, 3);  // 3.00
        p[3] = makeStudent("Gamma", 1, INFORMATYKA, 5, 5, 5);  // 5.00
        p[4] = makeStudent("Delta", 1, KOMP_NAUKY, 4, 5, 5);   // 4.67
    }

    // Підміняє cin на рядок на час життя об'єкта
    struct CinRedirect
    {
        istringstream in;
        streambuf *old;
        explicit CinRedirect(const string &text) : in(text), old(cin.rdbuf(in.rdbuf())) {}
        ~CinRedirect()
        {
            cin.rdbuf(old);
            cin.clear();
        }
    };

    string trim(const string &s)
    {
        size_t a = s.find_first_not_of(' ');
        if (a == string::npos)
            return "";
        size_t b = s.find_last_not_of(' ');
        return s.substr(a, b - a + 1);
    }

    // Розбиває рядок таблиці за '|' і обрізає пробіли
    vector<string> cells(const string &line)
    {
        vector<string> res;
        stringstream ss(line);
        string part;
        getline(ss, part, '|'); // частина перед першим '|'
        while (getline(ss, part, '|'))
            res.push_back(trim(part));
        return res;
    }

    string findLine(const string &out, const string &text)
    {
        stringstream ss(out);
        string line;
        while (getline(ss, line))
            if (line.find(text) != string::npos)
                return line;
        return "";
    }
}

// ---------- Third ----------

TEST(ThirdTest, KompNauky)
{
    EXPECT_EQ(Third(makeStudent("A", 1, KOMP_NAUKY, 4, 4, 5)), 5);
}

TEST(ThirdTest, Informatyka)
{
    EXPECT_EQ(Third(makeStudent("A", 1, INFORMATYKA, 4, 4, 3)), 3);
}

TEST(ThirdTest, OtherSpecialities)
{
    EXPECT_EQ(Third(makeStudent("A", 1, MATEM_EKON, 4, 4, 4)), 4);
    EXPECT_EQ(Third(makeStudent("B", 1, TRUDOVE, 4, 4, 5)), 5);
    EXPECT_EQ(Third(makeStudent("C", 1, FIZ_INF, 4, 4, 3)), 3);
}

// ---------- Avg ----------

TEST(AvgTest, ExactAverage)
{
    EXPECT_DOUBLE_EQ(Avg(makeStudent("A", 1, KOMP_NAUKY, 5, 4, 3)), 4.0);
}

TEST(AvgTest, RoundedToHundredths)
{
    EXPECT_DOUBLE_EQ(Avg(makeStudent("A", 1, KOMP_NAUKY, 5, 5, 4)), 4.67); // 14/3
    EXPECT_DOUBLE_EQ(Avg(makeStudent("A", 1, INFORMATYKA, 5, 4, 4)), 4.33); // 13/3
}

TEST(AvgTest, UsesThirdGradeOfOwnSpeciality)
{
    EXPECT_DOUBLE_EQ(Avg(makeStudent("A", 1, INFORMATYKA, 3, 3, 5)), 3.67); // 11/3
    EXPECT_DOUBLE_EQ(Avg(makeStudent("B", 1, TRUDOVE, 3, 3, 4)), 3.33);     // 10/3
}

// ---------- CmpKey / Cmp ----------

TEST(CmpKeyTest, SpecialityHasHighestPriority)
{
    Student s = makeStudent("Alpha", 1, INFORMATYKA, 3, 3, 3);
    EXPECT_EQ(CmpKey(s, KOMP_NAUKY, 1.0, "Aaa"), -1);
    Student t = makeStudent("Alpha", 1, TRUDOVE, 3, 3, 3);
    EXPECT_EQ(CmpKey(t, INFORMATYKA, 5.0, "Zzz"), 1);
}

TEST(CmpKeyTest, ThenAverageGrade)
{
    Student s = makeStudent("Alpha", 1, INFORMATYKA, 3, 3, 3); // 3.00
    EXPECT_EQ(CmpKey(s, INFORMATYKA, 4.0, "Alpha"), -1);
    EXPECT_EQ(CmpKey(s, INFORMATYKA, 2.0, "Alpha"), 1);
}

TEST(CmpKeyTest, ThenSurname)
{
    Student s = makeStudent("Alpha", 1, INFORMATYKA, 3, 3, 3);
    EXPECT_EQ(CmpKey(s, INFORMATYKA, 3.0, "Beta"), -1);
    EXPECT_EQ(CmpKey(s, INFORMATYKA, 3.0, "Aaa"), 1);
    EXPECT_EQ(CmpKey(s, INFORMATYKA, 3.0, "Alpha"), 0);
}

TEST(CmpTest, IsAntisymmetric)
{
    Student a = makeStudent("Alpha", 1, INFORMATYKA, 3, 3, 3);
    Student b = makeStudent("Beta", 1, INFORMATYKA, 5, 5, 5);
    EXPECT_LT(Cmp(a, b), 0);
    EXPECT_GT(Cmp(b, a), 0);
    EXPECT_EQ(Cmp(a, a), 0);
}

// ---------- PhysicalSort ----------

TEST(PhysicalSortTest, SortsBySpecialityAverageSurname)
{
    Student p[5];
    fillSample(p);
    PhysicalSort(p, 5);

    EXPECT_EQ(p[0].prizv, "Alpha");
    EXPECT_EQ(p[1].prizv, "Beta");
    EXPECT_EQ(p[2].prizv, "Gamma");
    EXPECT_EQ(p[3].prizv, "Delta");
    EXPECT_EQ(p[4].prizv, "Zeta");
}

TEST(PhysicalSortTest, SingleElementStaysTheSame)
{
    Student p[1] = {makeStudent("Only", 1, KOMP_NAUKY, 4, 4, 4)};
    PhysicalSort(p, 1);
    EXPECT_EQ(p[0].prizv, "Only");
}

// ---------- BuildIndex ----------

TEST(BuildIndexTest, BuildsSortedIndexWithoutMovingData)
{
    Student p[5];
    fillSample(p);
    int I[5];
    BuildIndex(p, I, 5);

    // індекси в порядку Alpha, Beta, Gamma, Delta, Zeta
    EXPECT_EQ(I[0], 2);
    EXPECT_EQ(I[1], 1);
    EXPECT_EQ(I[2], 3);
    EXPECT_EQ(I[3], 4);
    EXPECT_EQ(I[4], 0);

    // сам масив не змінився
    EXPECT_EQ(p[0].prizv, "Zeta");
    EXPECT_EQ(p[2].prizv, "Alpha");
}

// ---------- BinSearch ----------

TEST(BinSearchTest, FindsEveryElementOfSortedArray)
{
    Student p[5];
    fillSample(p);
    PhysicalSort(p, 5);
    for (int i = 0; i < 5; i++)
        EXPECT_EQ(BinSearch(p, 5, p[i].prizv, p[i].spec, Avg(p[i])), i) << p[i].prizv;
}

TEST(BinSearchTest, ReturnsMinusOneWhenNotFound)
{
    Student p[5];
    fillSample(p);
    PhysicalSort(p, 5);
    EXPECT_EQ(BinSearch(p, 5, "Nobody", INFORMATYKA, 5.0), -1);
    EXPECT_EQ(BinSearch(p, 5, "Beta", INFORMATYKA, 4.0), -1); // інший середній бал
    EXPECT_EQ(BinSearch(p, 5, "Beta", TRUDOVE, 5.0), -1);     // інша спеціальність
}

// ---------- BinSearchIndexed ----------

TEST(BinSearchIndexedTest, FindsPositionInIndexOrder)
{
    Student p[5];
    fillSample(p); // масив НЕ впорядкований
    int I[5];
    BuildIndex(p, I, 5);

    EXPECT_EQ(BinSearchIndexed(p, I, 5, "Delta", KOMP_NAUKY, 4.67), 3);
    EXPECT_EQ(BinSearchIndexed(p, I, 5, "Zeta", TRUDOVE, 4.0), 4);
    EXPECT_EQ(p[I[3]].prizv, "Delta");
}

TEST(BinSearchIndexedTest, ReturnsMinusOneWhenNotFound)
{
    Student p[5];
    fillSample(p);
    int I[5];
    BuildIndex(p, I, 5);
    EXPECT_EQ(BinSearchIndexed(p, I, 5, "Nobody", INFORMATYKA, 5.0), -1);
}

// ---------- Search (з імітацією вводу) ----------

TEST(SearchTest, FoundInSortedArray)
{
    Student p[5];
    fillSample(p);
    PhysicalSort(p, 5);
    int I[5];
    BuildIndex(p, I, 5);

    CinRedirect in("Delta\n1\n4.67\n");
    testing::internal::CaptureStdout();
    Search(p, I, 5, false);
    string out = testing::internal::GetCapturedStdout();

    EXPECT_NE(out.find("знайдено в позиції 4"), string::npos);
    EXPECT_EQ(out.find("не знайдено"), string::npos);
}

TEST(SearchTest, FoundThroughIndex)
{
    Student p[5];
    fillSample(p);
    int I[5];
    BuildIndex(p, I, 5);

    CinRedirect in("Delta\n1\n4.67\n");
    testing::internal::CaptureStdout();
    Search(p, I, 5, true);
    string out = testing::internal::GetCapturedStdout();

    EXPECT_NE(out.find("знайдено в позиції 4"), string::npos);
}

TEST(SearchTest, NotFound)
{
    Student p[5];
    fillSample(p);
    PhysicalSort(p, 5);
    int I[5];
    BuildIndex(p, I, 5);

    CinRedirect in("Nobody\n0\n5\n");
    testing::internal::CaptureStdout();
    Search(p, I, 5, false);
    string out = testing::internal::GetCapturedStdout();

    EXPECT_NE(out.find("Такого студента не знайдено"), string::npos);
}

// ---------- ReadInt / ReadAvg / ReadString ----------

TEST(ReadIntTest, ReturnsValidValue)
{
    CinRedirect in("3\n");
    testing::internal::CaptureStdout();
    int x = ReadInt("n: ", 1, 10);
    string out = testing::internal::GetCapturedStdout();
    EXPECT_EQ(x, 3);
    EXPECT_EQ(out.find("Помилка"), string::npos);
}

TEST(ReadIntTest, RepeatsOnNonNumber)
{
    CinRedirect in("abc\n7\n");
    testing::internal::CaptureStdout();
    int x = ReadInt("n: ", 1, 10);
    string out = testing::internal::GetCapturedStdout();
    EXPECT_EQ(x, 7);
    EXPECT_NE(out.find("Помилка"), string::npos);
}

TEST(ReadIntTest, RepeatsOnOutOfRange)
{
    CinRedirect in("0\n11\n5\n");
    testing::internal::CaptureStdout();
    int x = ReadInt("n: ", 1, 10);
    string out = testing::internal::GetCapturedStdout();
    EXPECT_EQ(x, 5);
    EXPECT_NE(out.find("Помилка"), string::npos);
}

TEST(ReadAvgTest, RoundsToHundredths)
{
    CinRedirect in("4.678\n");
    testing::internal::CaptureStdout();
    double x = ReadAvg("avg: ");
    testing::internal::GetCapturedStdout();
    EXPECT_DOUBLE_EQ(x, 4.68);
}

TEST(ReadAvgTest, RepeatsOnOutOfRange)
{
    CinRedirect in("7\n4.5\n");
    testing::internal::CaptureStdout();
    double x = ReadAvg("avg: ");
    string out = testing::internal::GetCapturedStdout();
    EXPECT_DOUBLE_EQ(x, 4.5);
    EXPECT_NE(out.find("Помилка"), string::npos);
}

TEST(ReadStringTest, SkipsEmptyLines)
{
    CinRedirect in("\nHello\n");
    testing::internal::CaptureStdout();
    string s = ReadString("s: ");
    string out = testing::internal::GetCapturedStdout();
    EXPECT_EQ(s, "Hello");
    EXPECT_NE(out.find("Помилка"), string::npos);
}

// ---------- Create ----------

TEST(CreateTest, ReadsThirdGradeBySpeciality)
{
    CinRedirect in(
        "Franko\n2\n1\n5\n4\n3\n"        // КН          -> програмування = 3
        "Shevchenko\n1\n0\n4\n5\n5\n"     // Інформатика -> чисельні методи = 5
        "Ukrainka\n3\n4\n3\n4\n4\n");     // Фіз. та інф. -> педагогіка = 4

    Student p[3];
    testing::internal::CaptureStdout();
    Create(p, 3);
    testing::internal::GetCapturedStdout();

    EXPECT_EQ(p[0].prizv, "Franko");
    EXPECT_EQ(p[0].kurs, 2);
    EXPECT_EQ(p[0].spec, KOMP_NAUKY);
    EXPECT_EQ(p[0].fizyka, 5);
    EXPECT_EQ(p[0].matem, 4);
    EXPECT_EQ(p[0].programuvannya, 3);

    EXPECT_EQ(p[1].prizv, "Shevchenko");
    EXPECT_EQ(p[1].spec, INFORMATYKA);
    EXPECT_EQ(p[1].chyselniMetody, 5);

    EXPECT_EQ(p[2].prizv, "Ukrainka");
    EXPECT_EQ(p[2].kurs, 3);
    EXPECT_EQ(p[2].spec, FIZ_INF);
    EXPECT_EQ(p[2].fizyka, 3);
    EXPECT_EQ(p[2].matem, 4);
    EXPECT_EQ(p[2].pedagogika, 4);
}

// ---------- Print / PrintIndexed ----------

TEST(PrintTest, HeaderIsPrinted)
{
    Student p[1] = {makeStudent("Printname", 1, KOMP_NAUKY, 5, 5, 5)};
    testing::internal::CaptureStdout();
    Print(p, 1);
    string out = testing::internal::GetCapturedStdout();

    EXPECT_NE(out.find("Прізвище"), string::npos);
    EXPECT_NE(out.find("Спеціальність"), string::npos);
    EXPECT_NE(out.find("Програм."), string::npos);
    EXPECT_NE(out.find("Числ.мет."), string::npos);
    EXPECT_NE(out.find("Педагог."), string::npos);
}

TEST(PrintTest, FillsOnlyOneOfThreeLastCells)
{
    Student p[3] = {
        makeStudent("Knname", 1, KOMP_NAUKY, 5, 4, 3),
        makeStudent("Infoname", 2, INFORMATYKA, 4, 5, 4),
        makeStudent("Pedname", 3, TRUDOVE, 3, 4, 5)};
    testing::internal::CaptureStdout();
    Print(p, 3);
    string out = testing::internal::GetCapturedStdout();

    // клітинки: №, прізвище, курс, спеціальність, фізика, матем., програм., числ.мет., педагог.
    vector<string> c1 = cells(findLine(out, "Knname"));
    ASSERT_EQ(c1.size(), 9u);
    EXPECT_EQ(c1[0], "1");
    EXPECT_EQ(c1[1], "Knname");
    EXPECT_EQ(c1[2], "1");
    EXPECT_EQ(c1[3], "Комп'ютерні науки");
    EXPECT_EQ(c1[4], "5");
    EXPECT_EQ(c1[5], "4");
    EXPECT_EQ(c1[6], "3");
    EXPECT_EQ(c1[7], "");
    EXPECT_EQ(c1[8], "");

    vector<string> c2 = cells(findLine(out, "Infoname"));
    ASSERT_EQ(c2.size(), 9u);
    EXPECT_EQ(c2[3], "Інформатика");
    EXPECT_EQ(c2[6], "");
    EXPECT_EQ(c2[7], "4");
    EXPECT_EQ(c2[8], "");

    vector<string> c3 = cells(findLine(out, "Pedname"));
    ASSERT_EQ(c3.size(), 9u);
    EXPECT_EQ(c3[3], "Трудове навчання");
    EXPECT_EQ(c3[6], "");
    EXPECT_EQ(c3[7], "");
    EXPECT_EQ(c3[8], "5");
}

TEST(PrintTest, ShowsAllSpecialityNames)
{
    Student p[5] = {
        makeStudent("S0", 1, INFORMATYKA, 5, 5, 5),
        makeStudent("S1", 1, KOMP_NAUKY, 5, 5, 5),
        makeStudent("S2", 1, MATEM_EKON, 5, 5, 5),
        makeStudent("S3", 1, TRUDOVE, 5, 5, 5),
        makeStudent("S4", 1, FIZ_INF, 5, 5, 5)};
    testing::internal::CaptureStdout();
    Print(p, 5);
    string out = testing::internal::GetCapturedStdout();

    for (int k = 0; k < SPEC_COUNT; k++)
        EXPECT_NE(out.find(specStr[k]), string::npos) << specStr[k];
}

TEST(PrintIndexedTest, PrintsInIndexOrder)
{
    Student p[2] = {
        makeStudent("Zeta", 1, TRUDOVE, 4, 4, 4),
        makeStudent("Alpha", 1, INFORMATYKA, 3, 3, 3)};
    int I[2] = {1, 0};

    testing::internal::CaptureStdout();
    PrintIndexed(p, I, 2);
    string out = testing::internal::GetCapturedStdout();

    EXPECT_LT(out.find("Alpha"), out.find("Zeta"));
    vector<string> c = cells(findLine(out, "Alpha"));
    ASSERT_EQ(c.size(), 9u);
    EXPECT_EQ(c[0], "1"); // нумерація йде за індексним масивом
}
