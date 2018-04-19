
# Fils for AADD
add_library(aadd SHARED 
aa_aafapprox.cpp 
aa_aafcommon.cpp 
aa_aaftrigo.cpp 
aa_interval.cpp
aa_aafarithm.cpp
aa_aafspecial.cpp
aa_exceptions.cpp
aa_rounding.cpp
aa.h
aa_aaf.h
aa_exceptions.h
aa_interval.h
aa_rounding.h
aadd_config.h
aadd.h
aadd_arithmetic.cpp
aadd_logical.cpp
aadd_common.cpp 
aadd_relational.cpp 
aadd_lp_glpk.h
aadd_lp_glpk.cpp
aadd_mgr.cpp
aadd_mgr.h
aadd_ddbase.cpp
aadd_ddbase.h
aadd_ddbase_impl.h
aadd_bdd.cpp
aadd_bdd.h
aadd_macros.h
)

#
# header files to be installed in DESTINATION/include
#
install (FILES aadd_macros.h aadd_lp_glpk.h aadd_mgr.h aadd_config.h aadd.h aadd_ddbase.h aadd_ddbase_impl.h aadd_off.h aadd_bdd.h aa.h aa_aaf.h aa_exceptions.h aa_interval.h aa_rounding.h DESTINATION include)

#
# libraries to be installed in DESTINATION/lib
#
install (TARGETS aadd DESTINATION lib)

target_link_libraries(aadd ${GLPK_LIBRARIES})


