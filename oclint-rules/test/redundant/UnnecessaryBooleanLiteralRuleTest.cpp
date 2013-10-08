#include "TestHeaders.h"

#include "rules/redundant/UnnecessaryBooleanLiteralRule.cpp"

TEST(UnnecessaryBooleanLiteralRuleTest, PropertyTest)
{
    UnnecessaryBooleanLiteralRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("unnecessary boolean literal", rule.name());
}

TEST(UnnecessaryBooleanLiteralRuleTest, NoUnnecessaryBooleanLiteral)
{
    const char* code = "int aMethod(bool b) { if (b) { return 0; } return 1; }";
    testRuleOnCXXCode(new UnnecessaryBooleanLiteralRule(), code);
}

TEST(UnnecessaryBooleanLiteralRuleTest, BooleanVariableEqualToAnotherBoolean)
{
    const char* code = "int aMethod(bool a, bool b) { if (a == b) { return 0; } return 1; }";
    testRuleOnCXXCode(new UnnecessaryBooleanLiteralRule(), code);
}

TEST(UnnecessaryBooleanLiteralRuleTest, BooleanVariableComparedToLiteral)
{
    //                                1         2         3         4         5         6
    //                       1234567890123456789012345678901234567890123456789012345678901234567
    const char* codes[] = { "int aMethod(bool b) { if (b     == true)  { return 0; } return 1; }",
                            "int aMethod(bool b) { if (b     == false) { return 0; } return 1; }",
                            "int aMethod(bool b) { if (b     != true)  { return 0; } return 1; }",
                            "int aMethod(bool b) { if (b     != false) { return 0; } return 1; }",
                            "int aMethod(bool b) { if (true  == b)  { return 0; } return 1; }",
                            "int aMethod(bool b) { if (false == b) { return 0; } return 1; }",
                            "int aMethod(bool b) { if (true  != b)  { return 0; } return 1; }",
                            "int aMethod(bool b) { if (false != b) { return 0; } return 1; }", };

    for (auto code : codes)
    {
        testRuleOnCXXCode(new UnnecessaryBooleanLiteralRule(), code, 0, 1, 27, 1, 36);
    }
}

TEST(UnnecessaryBooleanLiteralRuleTest, BooleanExpressionComparedWithLiteral)
{
    //                           1         2         3         4         5         6
    //                  12345678901234567890123456789012345678901234567890123456789012345678
    const char* code = "int aMethod(int i) { if ((i == 5) == true) { return 0; } return 1; }";
    testRuleOnCXXCode(new UnnecessaryBooleanLiteralRule(), code, 0, 1, 26, 1, 38);
}

TEST(UnnecessaryBooleanLiteralRuleTest, OverloadedOperatorUsedWithLiteral)
{
    testRuleOnCXXCode(new UnnecessaryBooleanLiteralRule(),
                      "struct Test { bool operator==(bool b) { return !b; } };"
                      "int aMethod() { Test test; if (test == true) { return 0; } return 1; }");
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
