
#include "testing.h"

#include "src/scriptRuntime/script.h"
#include "src/scriptRuntime/scriptRuntime.h"
#include "analyzer.h"

using namespace BraneScript;

TEST(BraneScript, FlowStatements)
{
    Analyzer analyzer;
    std::string path = "testScripts/flowStatementTests.bs";
    analyzer.load(path);
    analyzer.validate(path);
    checkCompileErrors(analyzer, path);

    auto ir = analyzer.compile(path, CompileFlags_DebugInfo);
    ASSERT_TRUE(ir.modules.contains("tests"));

    ScriptRuntime rt;
    rt.resetMallocDiff();
    Module* testScript = rt.loadModule(ir.modules.at("tests"));
    ASSERT_TRUE(testScript);

    auto testIf = testScript->getFunction<int, int, int, bool>("tests::testIf");
    ASSERT_TRUE(testIf);
    EXPECT_EQ(testIf(32, 64, true), 32);
    EXPECT_EQ(testIf(32, 64, false), 64);

    auto testIfElse = testScript->getFunction<int, int, int, bool>("tests::testIfElse");
    ASSERT_TRUE(testIfElse);
    EXPECT_EQ(testIfElse(32, 64, true), 32);
    EXPECT_EQ(testIfElse(32, 64, false), 64);

    auto testWhile = testScript->getFunction<int, int, int>("tests::testWhile");
    ASSERT_TRUE(testWhile);
    EXPECT_EQ(testWhile(2, 10), 10);
}