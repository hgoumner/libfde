
#include "adt/item.fpp"
#include "adt/itfUtil.fpp"

module adt_item
  use iso_c_binding
  use adt_ref
  use adt_string
  use adt_basetypes
  use adt_list
  implicit none
  private

! declare dummy variables - used to determine each type's storage_size ...
# define _item_type_(typeId, baseType)   baseType :: _paste(typeId,_var);
    _TableOf_item_types_
# undef _item_type_


# define _item_type_(typeId, baseType)   storage_size(_paste(typeId,_var)),

# define _chunkType   integer*8
  _chunkType, parameter :: chunk_var = 0
  integer*4,  parameter :: maxBytes  = max(_TableOf_item_types_ 0)/8
  integer*4,  parameter :: chunkSize = storage_size(chunk_var)/8
  integer*4,  parameter :: numChunks = ceiling( maxBytes / real(chunkSize) )
# undef _item_type_


  type, public :: Item_t
    private
    type(TypeInfo_t), pointer :: typeInfo        => null()
    _chunkType                :: data(numChunks) = 0
  end type


  type, public :: Item_t__impl__
    type(TypeInfo_t), pointer :: typeInfo        => null()
    _chunkType                :: data(numChunks) = 0
  end type


  !_TypeGen_declare_RefType( public, Item, type(Item_t), scalar, \
  !     initProc   = item_init_by_proto_c, \
  !     assignProc = item_assign_item_c,   \
  !     deleteProc = item_delete_c,        \
  !     cloneMode  = _type )

  !_TypeGen_declare_ListNode( public, Item, type(Item_t), scalar )

  
  ! declare Item_of(<type>) interface ...
  ! NOTE: It's named Item_of to avoid ambiguity with Item-interface for type deref.
  public :: Item_of
  interface Item_of
#   define _declare_constructor_(typeId, baseType, bt_import) \
    function _paste(item_of_,typeId)( val ) result(res)      ;\
      import Item_t; bt_import                               ;\
      baseType     :: val                                    ;\
      type(Item_t) :: res                                    ;\
    end function

    _declare_constructor_(bool1,       logical*1,)
    _declare_constructor_(bool2,       logical*2,)
    _declare_constructor_(bool4,       logical*4,)
    _declare_constructor_(bool8,       logical*8,)
    _declare_constructor_(int1,        integer*1,)
    _declare_constructor_(int2,        integer*2,)
    _declare_constructor_(int4,        integer*4,)
    _declare_constructor_(int8,        integer*8,)
    _declare_constructor_(real4,       real*4,)
    _declare_constructor_(real8,       real*8,)
    _declare_constructor_(complex8,    complex*8,)
    _declare_constructor_(complex16,   complex*16,)
    _declare_constructor_(c_void_ptr,  type(c_ptr),    import c_ptr)
    _declare_constructor_(string,      type(String_t), import String_t)
    _declare_constructor_(ref,         type(Ref_t),    import Ref_t)
    _declare_constructor_(charstring,  character(len=*),)
    _declare_constructor_(item,        type(Item_t),)

    function item_of_refencoding( val ) result(res)
      import Item_t, RefEncoding_t, Ref_t
      type(RefEncoding_t), dimension(:) :: val
      type(Ref_t),              pointer :: ptr
      type(Item_t),              target :: res
    end function
  end interface


  ! declare accessor interfaces for getting type pointers ...
# define _interface_Item_of(typeId, baseType, bt_import)   \
  public :: typeId                                        ;\
  interface typeId                                        ;\
    function _paste(item_get_,typeId)( self ) result(res) ;\
      import Item_t; bt_import                            ;\
      type(Item_t), target :: self                        ;\
      baseType,    pointer :: res                         ;\
    end function

  _interface_Item_of(bool1,      logical*1,)
  end interface
  _interface_Item_of(bool2,      logical*2,)
  end interface
  _interface_Item_of(bool4,      logical*4,)
  end interface
  _interface_Item_of(bool8,      logical*8,)
  end interface
  _interface_Item_of(int1,       integer*1,)
  end interface
  _interface_Item_of(int2,       integer*2,)
  end interface
  _interface_Item_of(int4,       integer*4,)
  end interface
  _interface_Item_of(int8,       integer*8,)
  end interface
  _interface_Item_of(real4,      real*4,)
  end interface
  _interface_Item_of(real8,      real*8,)
  end interface
  _interface_Item_of(complex8,   complex*8,)
  end interface
  _interface_Item_of(complex16,  complex*16,)
  end interface
  _interface_Item_of(c_void_ptr, type(c_ptr),    import c_ptr)
  end interface
  _interface_Item_of(string,     type(String_t), import String_t)
  end interface
  _interface_Item_of(ref,        type(Ref_t),    import Ref_t)
  end interface


  ! declare type-check interfaces ...
# define _interface_isType(typeId, baseType)            \
  public :: _paste(is_,typeId)                         ;\
  interface _paste(is_,typeId)                         ;\
    logical function _paste(item_is_,typeId)_c( self ) ;\
      import Item_t                                    ;\
      type(Item_t), intent(in) :: self                 ;\
    end function

  _interface_isType(bool1,      logical*1)
  end interface
  _interface_isType(bool2,      logical*2)
  end interface
  _interface_isType(bool4,      logical*4)
  end interface
  _interface_isType(bool8,      logical*8)
  end interface
  _interface_isType(int1,       integer*1)
  end interface
  _interface_isType(int2,       integer*2)
  end interface
  _interface_isType(int4,       integer*4)
  end interface
  _interface_isType(int8,       integer*8)
  end interface
  _interface_isType(real4,      real*4)
  end interface
  _interface_isType(real8,      real*8)
  end interface
  _interface_isType(complex8,   complex*8)
  end interface
  _interface_isType(complex16,  complex*16)
  end interface
  _interface_isType(c_void_ptr, type(c_ptr))
  end interface
  _interface_isType(string,     type(String_t))
  end interface
  _interface_isType(ref,        type(Ref_t))
  end interface


  ! declare assignment interfaces ...
  public :: assignment(=), assign
  interface assignment(=)
#   define _declare_assign_(typeId, baseType, bt_import)  \
    subroutine _paste(item_assign_,typeId)_c( lhs, rhs ) ;\
      import Item_t; bt_import                           ;\
      type(Item_t), target, intent(inout) :: lhs         ;\
      baseType,                intent(in) :: rhs         ;\
    end subroutine

    _declare_assign_(bool1,      logical*1,)
    _declare_assign_(bool2,      logical*2,)
    _declare_assign_(bool4,      logical*4,)
    _declare_assign_(bool8,      logical*8,)
    _declare_assign_(int1,       integer*1,)
    _declare_assign_(int2,       integer*2,)
    _declare_assign_(int4,       integer*4,)
    _declare_assign_(int8,       integer*8,)
    _declare_assign_(real4,      real*4,)
    _declare_assign_(real8,      real*8,)
    _declare_assign_(complex8,   complex*8,)
    _declare_assign_(complex16,  complex*16,)
    _declare_assign_(c_void_ptr, type(c_ptr),    import c_ptr)
    _declare_assign_(string,     type(String_t), import String_t)
    _declare_assign_(ref,        type(Ref_t),    import Ref_t)
    _declare_assign_(charstring, character(len=*),)

    subroutine item_assign_refencoding_( lhs, rhs )
      import Item_t, RefEncoding_t
      type(Item_t),           target, intent(inout) :: lhs
      type(RefEncoding_t), dimension(:), intent(in) :: rhs
    end subroutine


#   define _declare_assign_to_(typeId, baseType, bt_import)  \
    subroutine _paste(item_assign_to_,typeId)_c( lhs, rhs ) ;\
      import Item_t; bt_import                              ;\
      baseType, intent(inout) :: lhs                        ;\
      type(Item_t),    target :: rhs                        ;\
    end subroutine

    _declare_assign_to_(bool1,      logical*1,)
    _declare_assign_to_(bool2,      logical*2,)
    _declare_assign_to_(bool4,      logical*4,)
    _declare_assign_to_(bool8,      logical*8,)
    _declare_assign_to_(int1,       integer*1,)
    _declare_assign_to_(int2,       integer*2,)
    _declare_assign_to_(int4,       integer*4,)
    _declare_assign_to_(int8,       integer*8,)
    _declare_assign_to_(real4,      real*4,)
    _declare_assign_to_(real8,      real*8,)
    _declare_assign_to_(complex8,   complex*8,)
    _declare_assign_to_(complex16,  complex*16,)
    _declare_assign_to_(c_void_ptr, type(c_ptr),    import c_ptr)
    _declare_assign_to_(string,     type(String_t), import String_t)
    _declare_assign_to_(ref,        type(Ref_t),    import Ref_t)

    module procedure item_assign_item_private
  end interface

  ! declare by-call assignment interface
  interface assign
    subroutine item_assign_item_c( lhs, rhs )
      import Item_t
      type(Item_t), intent(inout) :: lhs
      type(Item_t)                :: rhs
    end subroutine
  end interface

  public :: is_valid
  interface is_valid
    logical function item_is_valid_c( self ) result(res)
      import Item_t
      type(Item_t), intent(in) :: self
    end function
  end interface

  public :: dynamic_type
  interface dynamic_type
    function item_dynamic_type( self ) result(res)
      import Item_t, TypeInfo_t
      type(Item_t),  intent(in) :: self
      type(TypeInfo_t), pointer :: res
    end function
  end interface

  public :: delete
  interface delete
    recursive subroutine item_delete_c( self )
      import Item_t
      type(Item_t) :: self
    end subroutine
  end interface

  interface
    subroutine item_init_by_proto_c( self, has_proto, proto )
      import Item_t
      type(Item_t),    intent(inout) :: self
      integer(kind=4), intent(in)    :: has_proto
      type(Item_t),    intent(in)    :: proto
    end subroutine
  end interface

!-----------------
  contains
!-----------------

  !_TypeGen_implementAll()
  
!_PROC_EXPORT(item_assign_item_private)
  subroutine item_assign_item_private( lhs, rhs )
    type(Item_t), intent(inout) :: lhs
    type(Item_t), intent(in)    :: rhs
    call assign( lhs, rhs )
  end subroutine

end module
