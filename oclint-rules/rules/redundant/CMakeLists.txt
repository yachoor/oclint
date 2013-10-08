SET(LIST_OF_RULES
    RedundantConditionalOperator
    RedundantIfStatement
    RedundantLocalVariable
    RedundantNilCheck
    UnnecessaryBooleanLiteral
    UnnecessaryElseStatement
    UselessParentheses
    )

BUILD_DYNAMIC_RULES("${LIST_OF_RULES}")
