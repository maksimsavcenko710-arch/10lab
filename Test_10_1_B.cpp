// Google Test для 10.1B.cpp
#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include <vector>

#define main program_main   // щоб main з програми не конфліктував з main від gtest_main
#ifndef PROG_SOURCE
#define PROG_SOURCE "10.1B.cpp"   // CMake підставляє справжнє ім'я файлу
#endif
#include PROG_SOURCE
#undef main

namespace
{
    // Третя оцінка записується в union відповідним до спеціальності полем
    Student makeStudent(const string &name, int kurs, Speciality s, int f, int m, int third)
    {
        Student st;
        st.prizv = name;
        st.kurs = kurs;
        st.speciality = s;
        st.Fizika = f;
        st.Matematika = m;
        switch (s)
        {
        case KN:
            st.Prog = third;
            break;
        case Inf:
            st.ChysMetody = third;
            break;
        default:
            st.Pedagogika = third;
            break;
        }
        return st;
    }

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
        // після останнього '|' getline уже нічого не читає, тому зайвої клітинки немає
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

// ---------- ThirdGrade ----------

TEST(ThirdGradeTest, KN)
{
    EXPECT_EQ(ThirdGrade(makeStudent("A", 1, KN, 4, 4, 5)), 5);
}

TEST(ThirdGradeTest, Inf)
{
    EXPECT_EQ(ThirdGrade(makeStudent("A", 1, Inf, 4, 4, 3)), 3);
}

TEST(ThirdGradeTest, OtherSpecialities)
{
    EXPECT_EQ(ThirdGrade(makeStudent("A", 1, MatEconom, 4, 4, 4)), 4);
    EXPECT_EQ(ThirdGrade(makeStudent("B", 1, FizInform, 4, 4, 5)), 5);
    EXPECT_EQ(ThirdGrade(makeStudent("C", 1, Trudove, 4, 4, 3)), 3);
}

// ---------- Len ----------

TEST(LenTest, Ascii)
{
    EXPECT_EQ(Len("abc"), 3);
}

TEST(LenTest, Empty)
{
    EXPECT_EQ(Len(""), 0);
}

TEST(LenTest, CyrillicCountsCharactersNotBytes)
{
    EXPECT_EQ(Len("Іваненко"), 8);
    EXPECT_EQ(Len("№"), 1);
    EXPECT_EQ(Len("Комп'ютерні науки"), 17);
}

// ---------- PrintCell ----------

TEST(PrintCellTest, PadsToWidth)
{
    testing::internal::CaptureStdout();
    PrintCell("ab", 5);
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "ab   ");
}

TEST(PrintCellTest, PadsCyrillicByCharacters)
{
    testing::internal::CaptureStdout();
    PrintCell("Фіз", 5);
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "Фіз  ");
}

TEST(PrintCellTest, NoPaddingWhenTooLong)
{
    testing::internal::CaptureStdout();
    PrintCell("abcdef", 3);
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "abcdef");
}

// ---------- SpecName ----------

TEST(SpecNameTest, AllSpecialities)
{
    EXPECT_EQ(SpecName(KN), "Комп'ютерні науки");
    EXPECT_EQ(SpecName(Inf), "Інформатика");
    EXPECT_EQ(SpecName(MatEconom), "Математика та економіка");
    EXPECT_EQ(SpecName(FizInform), "Фізика та інформатика");
    EXPECT_EQ(SpecName(Trudove), "Трудове навчання");
}

// ---------- FiveFizika ----------

TEST(FiveFizikaTest, CountsOnlyFives)
{
    Student p[4] = {
        makeStudent("A", 1, KN, 5, 3, 3),
        makeStudent("B", 1, Inf, 4, 5, 5),
        makeStudent("C", 2, MatEconom, 5, 5, 5),
        makeStudent("D", 3, Trudove, 3, 3, 3)};
    EXPECT_EQ(FiveFizika(p, 4), 2);
}

TEST(FiveFizikaTest, NoFives)
{
    Student p[2] = {
        makeStudent("A", 1, KN, 4, 5, 5),
        makeStudent("B", 1, KN, 3, 3, 3)};
    EXPECT_EQ(FiveFizika(p, 2), 0);
}

// ---------- PrintBezTrijok ----------

TEST(PrintBezTrijokTest, UsesThirdGradeOfEachSpeciality)
{
    Student p[5] = {
        makeStudent("Ivanenko", 1, KN, 4, 5, 4),      // підходить
        makeStudent("Petrenko", 1, KN, 5, 5, 3),      // Prog = 3 -> ні
        makeStudent("Shevchenko", 2, Inf, 5, 5, 5),   // підходить
        makeStudent("Koval", 2, Inf, 5, 4, 3),        // ChysMetody = 3 -> ні
        makeStudent("Bondar", 3, Trudove, 4, 4, 4)};  // підходить

    testing::internal::CaptureStdout();
    PrintBezTrijok(p, 5);
    string out = testing::internal::GetCapturedStdout();

    EXPECT_NE(out.find("Ivanenko"), string::npos);
    EXPECT_NE(out.find("Shevchenko"), string::npos);
    EXPECT_NE(out.find("Bondar"), string::npos);
    EXPECT_EQ(out.find("Petrenko"), string::npos);
    EXPECT_EQ(out.find("Koval"), string::npos);
}

TEST(PrintBezTrijokTest, FailingFizikaOrMatematika)
{
    Student p[2] = {
        makeStudent("LowPhys", 1, KN, 3, 5, 5),
        makeStudent("LowMath", 1, Inf, 5, 3, 5)};

    testing::internal::CaptureStdout();
    PrintBezTrijok(p, 2);
    string out = testing::internal::GetCapturedStdout();

    EXPECT_EQ(out.find("LowPhys"), string::npos);
    EXPECT_EQ(out.find("LowMath"), string::npos);
}

// ---------- Create (імітація вводу з клавіатури) ----------

TEST(CreateTest, ReadsThirdGradeBySpeciality)
{
    // Початковий "\n" імітує залишок рядка після cin >> N у main(),
    // який Create пропускає через cin.ignore.
    istringstream input(
        "\n"
        "Franko\n1\n0\n4\n5\n3\n"          // KN        -> Prog = 3
        "Shevchenko\n2\n1\n5\n4\n5\n"       // Inf       -> ChysMetody = 5
        "Ukrainka\n3\n3\n3\n4\n4\n");       // FizInform -> Pedagogika = 4
    streambuf *oldIn = cin.rdbuf(input.rdbuf());

    Student p[3];
    testing::internal::CaptureStdout();
    Create(p, 3);
    testing::internal::GetCapturedStdout();

    cin.rdbuf(oldIn);

    EXPECT_EQ(p[0].prizv, "Franko");
    EXPECT_EQ(p[0].kurs, 1);
    EXPECT_EQ(p[0].speciality, KN);
    EXPECT_EQ(p[0].Fizika, 4);
    EXPECT_EQ(p[0].Matematika, 5);
    EXPECT_EQ(p[0].Prog, 3);

    EXPECT_EQ(p[1].prizv, "Shevchenko");
    EXPECT_EQ(p[1].speciality, Inf);
    EXPECT_EQ(p[1].ChysMetody, 5);

    EXPECT_EQ(p[2].prizv, "Ukrainka");
    EXPECT_EQ(p[2].kurs, 3);
    EXPECT_EQ(p[2].speciality, FizInform);
    EXPECT_EQ(p[2].Fizika, 3);
    EXPECT_EQ(p[2].Matematika, 4);
    EXPECT_EQ(p[2].Pedagogika, 4);
}

// ---------- Print ----------

TEST(PrintTest, HeaderAndTitle)
{
    Student p[1] = {makeStudent("Test", 1, KN, 5, 5, 5)};

    testing::internal::CaptureStdout();
    Print(p, 1);
    string out = testing::internal::GetCapturedStdout();

    EXPECT_NE(out.find("СПИСОК СТУДЕНТІВ"), string::npos);
    EXPECT_NE(out.find("Прізвище"), string::npos);
    EXPECT_NE(out.find("Прог"), string::npos);
    EXPECT_NE(out.find("ЧМ"), string::npos);
    EXPECT_NE(out.find("Пед"), string::npos);
}

TEST(PrintTest, FillsOnlyOneOfThreeLastCells)
{
    Student p[3] = {
        makeStudent("Kn", 1, KN, 5, 4, 3),
        makeStudent("Info", 2, Inf, 4, 5, 4),
        makeStudent("Pedag", 3, Trudove, 3, 4, 5)};

    testing::internal::CaptureStdout();
    Print(p, 3);
    string out = testing::internal::GetCapturedStdout();

    // клітинки: №, прізвище, курс, спеціальність, фіз, мат, прог, ЧМ, пед
    vector<string> c1 = cells(findLine(out, "Kn"));
    ASSERT_EQ(c1.size(), 9u);
    EXPECT_EQ(c1[1], "Kn");
    EXPECT_EQ(c1[2], "1");
    EXPECT_EQ(c1[3], "Комп'ютерні науки");
    EXPECT_EQ(c1[4], "5");
    EXPECT_EQ(c1[5], "4");
    EXPECT_EQ(c1[6], "3");
    EXPECT_EQ(c1[7], "");
    EXPECT_EQ(c1[8], "");

    vector<string> c2 = cells(findLine(out, "Info"));
    ASSERT_EQ(c2.size(), 9u);
    EXPECT_EQ(c2[3], "Інформатика");
    EXPECT_EQ(c2[6], "");
    EXPECT_EQ(c2[7], "4");
    EXPECT_EQ(c2[8], "");

    vector<string> c3 = cells(findLine(out, "Pedag"));
    ASSERT_EQ(c3.size(), 9u);
    EXPECT_EQ(c3[3], "Трудове навчання");
    EXPECT_EQ(c3[6], "");
    EXPECT_EQ(c3[7], "");
    EXPECT_EQ(c3[8], "5");
}