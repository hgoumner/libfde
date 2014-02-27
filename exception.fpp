#ifndef __EXCEPTION_FPP
#define __EXCEPTION_FPP

#if defined(__GFORTRAN__)
# define _paste(a,b)    a/**/b
# define _str(a)        "a"
#else
# define _paste(a,b)    a ## b
# define _str(a)        #a
#endif

#define _args_0()
#define _args_1()     arg1,
#define _args_2()     _args_1() arg2,
#define _args_3()     _args_2() arg3,
#define _args_4()     _args_3() arg4,
#define _args_5()     _args_4() arg5,
#define _args_6()     _args_5() arg6,
#define _args_7()     _args_6() arg7,
#define _args_8()     _args_7() arg8,
#define _args_9()     _args_8() arg9,
#define _args_10()    _args_9() arg10,
#define _args_11()    _args_10() arg11,
#define _args_12()    _args_11() arg12,
#define _args_13()    _args_12() arg13,
#define _args_14()    _args_13() arg14,
#define _args_15()    _args_14() arg15,
#define _args_16()    _args_15() arg16,
#define _args_17()    _args_16() arg17,
#define _args_18()    _args_17() arg18,
#define _args_19()    _args_18() arg19,
#define _args_20()    _args_19() arg20,
#define _args(nr)     _paste(_args_,nr)()


#define _tryProcedure( id, args ) \
   function id( catchList, sub, args() argEnd ) bind(C,name="f_try") result(res) ;\
     use, intrinsic :: iso_c_binding


#define _end_tryProcedure \
     integer(kind=c_int), intent(in)    :: catchList(*)  ;\
     type (c_funptr), value, intent(in) :: sub           ;\
     type (c_ptr),    value, intent(in) :: argEnd        ;\
     integer(kind=c_int)                :: res           ;\
   end function


#define _catch(cases)   [cases, 0]
#define _catchAny       [0]
#define _argEnd         c_null_ptr


!--------------------------------------------------------------------
! The block variants of try/catch can be generated by
!   the following macros.
! For all of them, there are some restrictions:
!  - only in subroutines
!  - containing subroutines must be declared recursive
!  - try blocks CAN NOT share local variables!
!  - try blocks CAN NOT appear in control structures
!      like IF, SELECT, DO, FORALL ...
!      => That's why there are "loop"-variants _tryDo and _tryFor
!  - take care of the given label numbers
!  - don't mix block types while declaring top and bottom of a block!
!
! Examples:
!   _tryBlock(10)
!     value = mightFail( x, y )
!   _tryCatch(10, _catchAny)
!   _tryEnd(10)
!
!   _tryDo(20)
!     value = mightFail( x, y )
!   _tryCatch(20, (/ArithmeticError, RuntimeError/))
!     case (ArithmeticError); continue
!     case (RuntimeError);    print *, "catched RuntimeError"
!   _tryWhile(20, value < 0)
!
!   _tryFor(30, i = 0, i < 10, i = i + 1)
!     value = mightFail( x, i )
!   _tryCatch(20, (/ArithmeticError, RuntimeError/))
!     ! just ignore errors ...
!   _tryEndFor(30)
!--------------------------------------------------------------------

!-- start a simple try block
# define _tryBlock(label)             \
    goto _paste(label,02)            ;\
    entry _paste(tryblock__,label)

!-- start a try loop block (bottom-controlled: executes at least once)
# define _tryDo(label)  \
    _tryBlock(label)

!-- start a try loop block (top-controlled)
# define _tryFor(label, init, cond, inc )  \
    init                                  ;\
    goto _paste(label,01)                 ;\
    _paste(label,00) inc                  ;\
    _paste(label,01) continue             ;\
    if (cond) goto _paste(label,02)       ;\
    goto _paste(label,03)                 ;\
    entry _paste(tryblock__,label)


!-- start catch block, catching all given exception types --
# define _tryCatch(label, catchList)  \
    return                           ;\
    _paste(label,02) continue        ;\
    select case( try( _catch(catchList), proc(_paste(tryblock__,label)), _argEnd ) ) ;\
      case (0); continue  !< this is important! Without it gfortran would skip the try!


!-- end a simple try-catch
# define _tryEnd(label) \
    end select

!-- end a _tryDo
# define _tryWhile(label,cond)          \
    _tryEnd(label)                     ;\
    if (cond) goto _paste(label,02)    ;\
    _paste(label,03) continue

!-- end a _tryFor
# define _tryEndFor(label)     \
    end select                ;\
    goto _paste(label,00)     ;\
    _paste(label,03) continue

!-- early exit 
!   RESTRICTIONS:
!    -> breaks only the most inner try-loop
!    -> works only within catch block
# define _exitLoop(label)      \
    goto _paste(label,03)

#endif 

