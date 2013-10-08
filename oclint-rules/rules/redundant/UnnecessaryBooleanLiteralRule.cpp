#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

// It starts with a simple if (x == false) statement.
// Later during some refactoring someone needs opposite condition.
// Some statements are changed to if (x == true) ...
// ... and some to if (x != false)
// Some people prefer constants and literals on the left: if (true == x)
// and we end up with 5 ways to write it:
// if (x)
// if (x == true)
// if (x != false)
// if (true == x)
// if (false != x)
// And I've seen some insane code like: if ((a == b) == true)
// So let's Keep It Stupid Simple and stick to if (x)
class UnnecessaryBooleanLiteralRule : public AbstractASTVisitorRule<UnnecessaryBooleanLiteralRule>
{
public:
    virtual const string name() const
    {
        return "unnecessary boolean literal";
    }

    virtual int priority() const
    {
        return 3;
    }

    bool VisitBinaryOperator(BinaryOperator* binaryOperator)
    {
        if (!binaryOperator->isEqualityOp())
        {
            return true;
        }
        Expr* leftExpression = binaryOperator->getLHS();
        Expr* rightExpression = binaryOperator->getRHS();
        if (isBooleanLiteral(leftExpression) || isBooleanLiteral(rightExpression))
        {
            addViolation(binaryOperator, this);
        }

        return true;
    }

private:
    static RuleSet rules;

    template <typename nodeType>
    nodeType* extractFromImplicitCastExpr(Expr* fromExpr)
    {
        ImplicitCastExpr* implicitCastExpr = dyn_cast_or_null<ImplicitCastExpr>(fromExpr);
        if (implicitCastExpr)
        {
            return dyn_cast_or_null<nodeType>(implicitCastExpr->getSubExpr());
        }
        return NULL;
    }

    bool isBooleanLiteral(Expr* expr)
    {
        return extractFromImplicitCastExpr<CXXBoolLiteralExpr>(expr);
    }
};

RuleSet UnnecessaryBooleanLiteralRule::rules(new UnnecessaryBooleanLiteralRule());
