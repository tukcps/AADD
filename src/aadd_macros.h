#define ifS(cond)       {scopes().ifAADD(cond);

#define elseS           scopes().elseAADD(__LINE__, __FILE__);

#define endS            scopes().endifAADD(__LINE__, __FILE__);}

#define whileS(cond)    while (cond!=false) \
                        {scopes().whileAADD(cond);

/*
 ScopeMgr scopes;
 Example:
 with Macros:              After preprocessor:

 ifS(a==b)                 scopes.ifAADD(a==b);  // just a method call
 {                         {                // a block that will be executed, no matter if a==b or not.
   lval = expr;              lval = expr;
 }                         }
 elseS                     scopes.elseAADD();
 {                         {                // a block that will be executed, no matter if a==b or not.
   lval = expr2;             lval = expr2;
 } endifS;                 }
                           scopes.endifAADD();
 */

