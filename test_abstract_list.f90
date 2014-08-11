
! helper macros for extending abstract_list
! to be provided by #include "adt/abstract_list.fpp"
! OR
!  extend refgen.py ?

# define _implementNewItem_( typeId, baseType ) \
    ...
# define _implementItemCast_( typeId, baseType ) \
    ...

module mylist
  use abstract_list
  implicit none

  type :: MyItem_t
    type (Item_t) :: super
    integer*4     :: value
  end type

  !interface MyItem;  module procedure myitem_downcast; end interface
  interface MyValue; module procedure myitem_value;    end interface

  !public :: MyItem
  public :: MyValue

contains

  function newItem( val ) result(res)
    integer*4,    intent(in) :: val
    type (Item_t),   pointer :: res
    type (MyItem_t), pointer :: node
    allocate( node )
    node%value = val
    res => node%super
  end function

  !function myitem_downcast( node ) result(res)
  !  use iso_c_binding
  !  type (Item_t),    target :: node
  !  type (MyItem_t), pointer :: res
  !  call c_f_pointer( c_loc(node), res )
  !end function

  function myitem_value( node ) result(res)
    use iso_c_binding
    type (Item_t),    target :: node
    integer*4,       pointer :: res
    type (MyItem_t), pointer :: ptr
    call c_f_pointer( c_loc(node), ptr )
    res => ptr%value
  end function

end module


program testinger
  use var_item
  use abstract_list
  use base_types
  use mylist
  use iso_c_binding
  implicit none

  type (List_t) :: l1, l2
  integer*4   :: cnt
  type (MyItem_t), pointer :: ptr
  type (VarItem_t)         :: var
  type (GenericRef_t)      :: ref1
  type (ListIterator_t)    :: itr
  procedure(), pointer :: castProc => null()

  call initialize( l1, static_type(1) )
  call initialize( l2, static_type(1) )

  print *, is_valid(l1)
  print *, is_valid(l1, static_type(1))
  print *, is_valid(l1, static_type(2.3))

  do cnt = 1, 10
    call append( l2, newItem( cnt ) )
  end do

  call printItems( iterator( l1 ) )
  call printItems( iterator( l2 ) )
  call append( l1, l2 )
  call printItems( iterator( l1 ) )
  call printItems( iterator( l2 ) )

  itr = iterator( l1, 3 )
  do cnt = -5, -1
    call insert( iterator(l1, back(l1)), newItem(cnt) )
  end do
    


  call printItems( iterator( l1, first ) )
  call printItems( iterator( l1, first, -1 ) )
  call printItems( iterator( l1, last ) )
  call printItems( iterator( l1, last , -1 ) )
  call printItems( iterator( l1, tail ) )
  call printItems( iterator( l1, tail ) )

  call printItems( iterator( l1, 5, 1 ) )
  call printItems( iterator( l1, 5, -1 ) )
  call printItems( iterator( l1, -5 ) )
  call printItems( iterator( l1, -10 ) )

  itr = iterator( l1 )
  do while (is_valid(itr))
    print *, MyValue( itr%node )
    call next(itr)
  end do

  !itr = iterator( l1, front(l1) )
  !do while (is_valid(itr))
  !  print *, MyValue( itr%node )
  !  call next(itr)
  !end do

  itr = iterator( l1, back(l1), -2 )
  do while (is_valid(itr))
    print *, MyValue( itr%node )
    itr = get_next(itr)
  end do

  itr = iterator( l1, tail, -1 )
  do while (is_valid(itr))
    print *, MyValue( itr%node )
    itr = get_next(itr)
  end do

  itr = iterator( l1, front(l1) )
  itr = iterator( l1, front(l1), 2 )
  itr = iterator( l1, front(l1), -1 )
  itr = iterator( l1, back(l1) )
  itr = iterator( l1, back(l1), -1 )
  itr = iterator( l1, back(l1), 1 )
  itr = iterator( l1, itr, 1 )
  itr = iterator( l1, itr, 1 )

  ref1 = ref(l1)

  print *, len(List(ref1))

  print *, MyValue( front(l1) )

  !ref1 = clone(ref1)
  !call free( ref1 ) !< segfaults because of shallow copy!

  call clear( l1 )
  call delete( l1 )
  call delete( l2 )


  
  contains

  subroutine printItems( itr )
    type(ListIterator_t) :: itr

    print *, "items:"
    do while (is_valid(itr))
      print *, MyValue( itr%node )
      call next(itr)
    end do
    print *, "########"
  end subroutine


end program

