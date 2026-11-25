// Google Test для 10.1A.cpp
#include <gtest/gtest.h>
#include <sstream>
#include <iostream>

#define main program_main   // щоб main з програми не конфліктував з main від gtest_main
#ifndef PROG_SOURCE
#define PROG_SOURCE "10.1A.cpp"   // CMake підставляє справжнє ім'я файлу
#endif
#include PROG_SOURCE
#undef main

namespace
{
    Student makeStudent(const string &name, int kurs, Speciality s, int f, int m, int i)
    {
        Student st;
        st.prizv = name;
        st.kurs = kurs;
        st.speciality = s;
        st.Fizika = f;
        st.Matematika = m;
        st.Informatika = i;
        return st;
    }
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

TEST(FiveFizikaTest, AllFives)
{
    Student p[3] = {
        makeStudent("A", 1, KN, 5, 1, 1),
        makeStudent("B", 1, KN, 5, 1, 1),
        makeStudent("C", 1, KN, 5, 1, 1)};
    EXPECT_EQ(FiveFizika(p, 3), 3);
}

// ---------- Vidminno ----------

TEST(VidminnoTest, PrintsOnlyStudentsWithAllGradesAbove3)
{
    Student p[4] = {
        makeStudent("Ivanenko", 1, KN, 4, 5, 4),     // підходить
        makeStudent("Petrenko", 1, KN, 3, 5, 5),     // фізика = 3 -> ні
        makeStudent("Shevchenko", 2, Inf, 5, 5, 5),  // підходить
        makeStudent("Koval", 2, Inf, 5, 5, 3)};      // інформатика = 3 -> ні

    testing::internal::CaptureStdout();
    Vidminno(p, 4);
    string out = testing::internal::GetCapturedStdout();

    EXPECT_NE(out.find("Ivanenko"), string::npos);
    EXPECT_NE(out.find("Shevchenko"), string::npos);
    EXPECT_EQ(out.find("Petrenko"), string::npos);
    EXPECT_EQ(out.find("Koval"), string::npos);
}

TEST(VidminnoTest, NobodyQualifies)
{
    Student p[1] = {makeStudent("Bad", 1, KN, 3, 3, 3)};

    testing::internal::CaptureStdout();
    Vidminno(p, 1);
    string out = testing::internal::GetCapturedStdout();

    EXPECT_EQ(out.find("Bad"), string::npos);
}

// ---------- Create (імітація вводу з клавіатури) ----------

TEST(CreateTest, ReadsFieldsCorrectly)
{
    // Початковий "\n" імітує залишок рядка після cin >> N у main(),
    // який Create пропускає через cin.ignore.
    istringstream input(
        "\n"
        "Shevchenko\n2\n3\n5\n4\n3\n"
        "Franko\n1\n0\n4\n5\n5\n");
    streambuf *oldIn = cin.rdbuf(input.rdbuf());

    Student p[2];
    testing::internal::CaptureStdout();
    Create(p, 2);
    testing::internal::GetCapturedStdout();

    cin.rdbuf(oldIn);

    EXPECT_EQ(p[0].prizv, "Shevchenko");
    EXPECT_EQ(p[0].kurs, 2);
    EXPECT_EQ(p[0].speciality, FizInform);
    EXPECT_EQ(p[0].Fizika, 5);
    EXPECT_EQ(p[0].Matematika, 4);
    EXPECT_EQ(p[0].Informatika, 3);

    EXPECT_EQ(p[1].prizv, "Franko");
    EXPECT_EQ(p[1].kurs, 1);
    EXPECT_EQ(p[1].speciality, KN);
    EXPECT_EQ(p[1].Fizika, 4);
    EXPECT_EQ(p[1].Matematika, 5);
    EXPECT_EQ(p[1].Informatika, 5);
}

// ---------- print ----------

TEST(PrintTest, ShowsSpecialityNames)
{
    Student p[5] = {
        makeStudent("S1", 1, KN, 5, 5, 5),
        makeStudent("S2", 1, Inf, 5, 5, 5),
        makeStudent("S3", 1, MatEconom, 5, 5, 5),
        makeStudent("S4", 1, FizInform, 5, 5, 5),
        makeStudent("S5", 1, Trudove, 5, 5, 5)};

    testing::internal::CaptureStdout();
    print(p, 5);
    string out = testing::internal::GetCapturedStdout();

    EXPECT_NE(out.find("СПИСОК СТУДЕНТІВ"), string::npos);
    EXPECT_NE(out.find("S1, Комп'ютерні науки"), string::npos);
    EXPECT_NE(out.find("S2, Інформатика"), string::npos);
    EXPECT_NE(out.find("S3, Математика та економіка"), string::npos);
    EXPECT_NE(out.find("S4, Фізика та інформатика"), string::npos);
    EXPECT_NE(out.find("S5, Трудове навчання"), string::npos);
}

TEST(PrintTest, ShowsGrades)
{
    Student p[1] = {makeStudent("Test", 3, KN, 5, 4, 3)};

    testing::internal::CaptureStdout();
    print(p, 1);
    string out = testing::internal::GetCapturedStdout();

    EXPECT_NE(out.find("Test"), string::npos);
    EXPECT_NE(out.find("| 1  | 3    | 5   | 4   | 3   |"), string::npos);
}