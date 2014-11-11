
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

  !_TypeGen_declare_ListNode( public, String, type(String_t),  scalar )
  !_TypeGen_declare_ListNode( alias, String, character(len=*), scalar )
  !_TypeGen_declare_ListNode( alias, String, character(len=1), dimension(:) )

  contains

  !_TypeGen_implementAll()

end module
