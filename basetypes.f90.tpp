
!                . o O (needed for generated procedure exports)
#include "adt/itfUtil.fpp"

module adt_basetypes
  use iso_c_binding
  use adt_ref
  use adt_list
  use adt_string
  implicit none
  private

  !_TypeGen_declare_RefType( public, bool1,      logical*1,   scalar )
  !_TypeGen_declare_RefType( public, bool2,      logical*2,   scalar )
  !_TypeGen_declare_RefType( public, bool4,      logical*4,   scalar )
  !_TypeGen_declare_RefType( public, bool8,      logical*8,   scalar )
  !_TypeGen_declare_RefType( public, int1,       integer*1,   scalar )
  !_TypeGen_declare_RefType( public, int2,       integer*2,   scalar )
  !_TypeGen_declare_RefType( public, int4,       integer*4,   scalar )
  !_TypeGen_declare_RefType( public, int8,       integer*8,   scalar )
  !_TypeGen_declare_RefType( public, real4,      real*4,      scalar )
  !_TypeGen_declare_RefType( public, real8,      real*8,      scalar )
  !_TypeGen_declare_RefType( public, real16,     real*16,     scalar )
  !_TypeGen_declare_RefType( public, complex8,   complex*8,   scalar )
  !_TypeGen_declare_RefType( public, complex16,  complex*16,  scalar )
  !_TypeGen_declare_RefType( public, complex32,  complex*32,  scalar )
  !_TypeGen_declare_RefType( public, c_void_ptr, type(c_ptr), scalar )

  !_TypeGen_declare_RefType( public, bool1_1d,      logical*1,   dimension(:) )
  !_TypeGen_declare_RefType( public, bool2_1d,      logical*2,   dimension(:) )
  !_TypeGen_declare_RefType( public, bool4_1d,      logical*4,   dimension(:) )
  !_TypeGen_declare_RefType( public, bool8_1d,      logical*8,   dimension(:) )
  !_TypeGen_declare_RefType( public, int1_1d,       integer*1,   dimension(:) )
  !_TypeGen_declare_RefType( public, int2_1d,       integer*2,   dimension(:) )
  !_TypeGen_declare_RefType( public, int4_1d,       integer*4,   dimension(:) )
  !_TypeGen_declare_RefType( public, int8_1d,       integer*8,   dimension(:) )
  !_TypeGen_declare_RefType( public, real4_1d,      real*4,      dimension(:) )
  !_TypeGen_declare_RefType( public, real8_1d,      real*8,      dimension(:) )
  !_TypeGen_declare_RefType( public, real16_1d,     real*16,     dimension(:) )
  !_TypeGen_declare_RefType( public, complex8_1d,   complex*8,   dimension(:) )
  !_TypeGen_declare_RefType( public, complex16_1d,  complex*16,  dimension(:) )
  !_TypeGen_declare_RefType( public, complex32_1d,  complex*32,  dimension(:) )
  !_TypeGen_declare_RefType( public, c_void_ptr_1d, type(c_ptr), dimension(:) )

  !_TypeGen_declare_RefType( public, bool1_2d,      logical*1,   dimension(:,:) )
  !_TypeGen_declare_RefType( public, bool2_2d,      logical*2,   dimension(:,:) )
  !_TypeGen_declare_RefType( public, bool4_2d,      logical*4,   dimension(:,:) )
  !_TypeGen_declare_RefType( public, bool8_2d,      logical*8,   dimension(:,:) )
  !_TypeGen_declare_RefType( public, int1_2d,       integer*1,   dimension(:,:) )
  !_TypeGen_declare_RefType( public, int2_2d,       integer*2,   dimension(:,:) )
  !_TypeGen_declare_RefType( public, int4_2d,       integer*4,   dimension(:,:) )
  !_TypeGen_declare_RefType( public, int8_2d,       integer*8,   dimension(:,:) )
  !_TypeGen_declare_RefType( public, real4_2d,      real*4,      dimension(:,:) )
  !_TypeGen_declare_RefType( public, real8_2d,      real*8,      dimension(:,:) )
  !_TypeGen_declare_RefType( public, real16_2d,     real*16,     dimension(:,:) )
  !_TypeGen_declare_RefType( public, complex8_2d,   complex*8,   dimension(:,:) )
  !_TypeGen_declare_RefType( public, complex16_2d,  complex*16,  dimension(:,:) )
  !_TypeGen_declare_RefType( public, complex32_2d,  complex*32,  dimension(:,:) )
  !_TypeGen_declare_RefType( public, c_void_ptr_2d, type(c_ptr), dimension(:,:) )


  !_TypeGen_declare_ListNode( public, bool1,      logical*1,   scalar )
  !_TypeGen_declare_ListNode( public, bool2,      logical*2,   scalar )
  !_TypeGen_declare_ListNode( public, bool4,      logical*4,   scalar )
  !_TypeGen_declare_ListNode( public, bool8,      logical*8,   scalar )
  !_TypeGen_declare_ListNode( public, int1,       integer*1,   scalar )
  !_TypeGen_declare_ListNode( public, int2,       integer*2,   scalar )
  !_TypeGen_declare_ListNode( public, int4,       integer*4,   scalar )
  !_TypeGen_declare_ListNode( public, int8,       integer*8,   scalar )
  !_TypeGen_declare_ListNode( public, real4,      real*4,      scalar )
  !_TypeGen_declare_ListNode( public, real8,      real*8,      scalar )
  !_TypeGen_declare_ListNode( public, real16,     real*16,     scalar )
  !_TypeGen_declare_ListNode( public, complex8,   complex*8,   scalar )
  !_TypeGen_declare_ListNode( public, complex16,  complex*16,  scalar )
  !_TypeGen_declare_ListNode( public, complex32,  complex*32,  scalar )
  !_TypeGen_declare_ListNode( public, c_void_ptr, type(c_ptr), scalar )

  !_TypeGen_declare_ListNode( public, ref, type(Ref_t),        scalar )
  !_TypeGen_declare_ListNode( alias, ref, type(RefEncoding_t), dimension(:) )


  !!_TypeGen_declare_RefType( public, charString_1d, character(len=:), dimension(:) )
  !!_TypeGen_declare_RefType( public, charString_2d, character(len=:), dimension(:,:) )
  !! FIXME: ^ these two types cause gfortran to freak out with an internal compiler error >:-(
  !_TypeGen_declare_RefType( public, char10_1d, character(len=10), dimension(:) )

  !_TypeGen_declare_RefType( public, String_1d, type(String_t),  dimension(:) )
  !_TypeGen_declare_RefType( public, String_2d, type(String_t),  dimension(:,:) )

  !_TypeGen_declare_ListNode( public, String, type(String_t),  scalar )
  !_TypeGen_declare_ListNode( alias, String, character(len=*), scalar )
  !_TypeGen_declare_ListNode( alias, String, character(len=1), dimension(:) )

  abstract interface
    subroutine Callback_itf(); end subroutine
  end interface
  public :: Callback_itf

  !_TypeGen_declare_RefType( public, Callback, procedure(Callback_itf), scalar )
  ! TODO: _TypeGen_declare_ListNode not possible yet!

  contains

  !_TypeGen_implementAll()

end module

