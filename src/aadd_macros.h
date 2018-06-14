#ifndef AADD_MACROS_H
#define AADD_MACROS_H
/* 
   Macros that replace conditional and iteration statements. 
 */ 

#define ifS(cond)       {scopes().ifAADD(cond);
#define elseS           scopes().elseAADD(__LINE__, __FILE__);
#define endS            scopes().endifAADD(__LINE__, __FILE__);}
#define endf            scopes().endifAADD(__LINE__, __FILE__);
#define whileS(cond)    while (cond!=false) \
                        {scopes().whileAADD(cond);


/*
 Example:
 with Macros:              After preprocessor:

 ifS(a==b)                 { scopes.ifAADD(a==b); 
   lval = expr;              lval = expr;
 elseS                       scopes.elseAADD();
   lval = expr2;             lval = expr2;
 endifS;                     scopes.endifAADD(); }
 */


/** 
   Procedures that convert between symbolic and concrete execution. 
   @brief SYMBOLIC replaces an AADD a by a range that is an abstraction resp. safe inclusion of a. 
 */  
inline void SYMBOLIC(AADD &a, double lb, double ub)
{
    opt_sol bounds=a.GetBothBounds();
        cout << bounds.min << " " << bounds.max << endl;

    assert(bounds.min < lb); 
    assert(bounds.max > ub); 
    a=AADD(lb,ub); 
}

/**
  @brief CONCRETE replaces an AADD by a refinement to a double that is included in the AADD. 
 */ 
inline void CONCRETE(AADD &a, double d)
{
    opt_sol bounds=a.GetBothBounds();
    assert(bounds.min < d);
    assert(bounds.max > d);
    a=d; 
}

#endif
