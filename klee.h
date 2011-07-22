#ifndef KLEE_H
#define KLEE_H

#ifdef KLEE

// Enums of ptr and var names to be used as indexes into klee_ptr_copy
#undef  KLEE_XX_PTR
#undef  KLEE_XX_VAR
#define KLEE_XX_PTR(x) ENUM_ ## x ,
#define KLEE_XX_VAR(x) ENUM_ ## x ,
enum { 
#include "klee_state.inc" 
};

#define IFKLEE(x) x
#define IFNKLEE(x)
#define KLEE_PRINTF 

#define KLEE_MAKE_SYMBOLIC_PTR(ptr)						\
  if (ptr != NULL) { 								\
  KLEE_PRINTF("KLEE_MAKE_SYMBOLIC_PTR( " #ptr " )\n"); 				\
  klee_make_symbolic_unknown_size(ptr, #ptr );					\
  } else KLEE_PRINTF("KLEE_MAKE_SYMBOLIC_PTR( " #ptr " ) FAILED.\n");

#define KLEE_MAKE_SYMBOLIC_VAR(var) 						\
  KLEE_PRINTF("KLEE_MAKE_SYMBOLIC_VAR( " #var " )\n"); 				\
  klee_make_symbolic_unknown_size(&var, #var );

#define KLEE_COPY_PTR_NAME(ptr)							\
  if (ptr != NULL) {								\
  KLEE_PRINTF("KLEE_COPY_PTR_NAME( " #ptr " )\n");				\
  klee_ptr_copy[ENUM_ ## ptr ] = (void*) klee_duplicate_symbolic(ptr, #ptr );	\
  } else KLEE_PRINTF("KLEE_COPY_PTR_NAME( " #ptr " ) FAILED.\n");

#define KLEE_COPY_VAR_NAME(var) 						\
  KLEE_PRINTF("KLEE_COPY_VAR_NAME( " #var " )\n"); 				\
  klee_ptr_copy[ENUM_ ## var ] = (void*) klee_duplicate_symbolic(&var, #var );

#define KLEE_COPY_PTR(ptr)							\
  if (ptr != NULL) {								\
  KLEE_PRINTF("KLEE_COPY_PTR( " #ptr " )\n");					\
  klee_ptr_copy[ENUM_ ## ptr ] = (void*) klee_duplicate_symbolic(ptr);		\
  } else KLEE_PRINTF("KLEE_COPY_PTR( " #ptr " ) FAILED.\n");

#define KLEE_COPY_VAR(var) 							\
  KLEE_PRINTF("KLEE_COPY_VAR( " #var " )\n"); 					\
  klee_ptr_copy[ENUM_ ## var ] = (void*) klee_duplicate_symbolic(&var);

#define KLEE_EQUAL_PTR(ptr) 							\
  if (ptr != NULL) { 								\
  KLEE_PRINTF("KLEE_EQUAL_PTR( " #ptr " )\n"); 					\
  klee_equal(ptr, klee_ptr_copy[ENUM_ ## ptr ] ); 				\
  } else KLEE_PRINTF("KLEE_EQUAL_PTR( " #ptr " ) FAILED.\n");

#define KLEE_EQUAL_VAR(var) 							\
  KLEE_PRINTF("KLEE_EQUAL_VAR( " #var " )\n"); 					\
  klee_equal(&var, klee_ptr_copy[ENUM_ ## var] );

#define KLEE_VAR_SIMPLE(var) 							\
  KLEE_PRINTF("KLEE_VAR_SIMPLE( " #var " )\n"); 				\
  klee_make_symbolic_unknown_size(&var, #var );

#define KLEE_STOP 								\
  KLEE_PRINTF("klee_write_constraints()\n"); 					\
  klee_write_constraints(); 							\
  exit(1);

#define KLEE_EXTERNAL_MEM(type, addr) 						\
  KLEE_PRINTF("klee_add_external_object( " #addr ", sizeof(" #type "))\n");	\
  klee_add_external_object(addr, sizeof(type) );

#else

#define IFKLEE(x) 
#define IFNKLEE(x) x
#define KLEE_VAR(x)
#define KLEE_VAR_SIMPLE(var)
#define KLEE_MEM(x)
#define KLEE_EXTERNAL_MEM(type, addr)
#define KLEE_STOP(x)
#define KLEE_DEBUG(x)

#endif 

#endif /* KLEE_H */
