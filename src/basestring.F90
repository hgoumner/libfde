
#include "fde/itfUtil.fpp"
#include "fde/ref_status.fpp"

module fde_basestring
  use iso_c_binding
  implicit none
  
  type BaseString_t
    character(len=1), dimension(:), pointer :: ptr     => null()
    integer(kind=c_size_t)                  :: len     = 0
    _RefStatus                              :: refstat = _ref_HardLent
  end type

  ! parameter definitions

  !_SYM_EXPORT(temporary_string)
  type(BaseString_t), parameter :: temporary_string    = BaseString_t( null(), 0, _ref_WeakLent )
  !_SYM_EXPORT(permanent_string)
  type(BaseString_t), parameter :: permanent_string    = BaseString_t( null(), 0, _ref_HardLent )
  integer(kind=1),    parameter :: attribute_volatile  = 0
  integer(kind=1),    parameter :: attribute_permanent = 1

  character(0),          target :: empty_string_ = ''

  ! interface definitions

  interface
    subroutine basestring_init_by_basestring_c( bs, has_proto, proto )
      import BaseString_t
      type(BaseString_t), intent(inout) :: bs
      integer(kind=4),    intent(in)    :: has_proto
      type(BaseString_t), intent(in)    :: proto
    end subroutine

    subroutine basestring_init_by_charstring_c( bs, attr, cs )
      import BaseString_t
      type(BaseString_t), intent(inout) :: bs
      integer(kind=1),    intent(in)    :: attr
      character(len=*),   intent(in)    :: cs
    end subroutine

    subroutine basestring_init_by_buf( bs, attr, buf )
      import BaseString_t
      type(BaseString_t),             intent(inout) :: bs
      integer(kind=1),                intent(in)    :: attr
      character(len=1), dimension(:), intent(in)    :: buf
    end subroutine

    subroutine basestring_set_attribute( bs, attr )
      import BaseString_t
      type(BaseString_t), intent(inout) :: bs
      integer(kind=1),    intent(in)    :: attr
    end subroutine

    subroutine basestring_release_weak( bs )
      import BaseString_t
      type(BaseString_t) :: bs
    end subroutine

    subroutine basestring_delete_c( bs )
      import BaseString_t
      type(BaseString_t), intent(inout) :: bs
    end subroutine

    function basestring_ptr( bs ) result(res)
      import BaseString_t
      type(BaseString_t), intent(in) :: bs
      character(len=bs%len), pointer :: res
    end function

    function basestring_safeptr( bs ) result(res)
      import BaseString_t
      type(BaseString_t), intent(in) :: bs
      character(len=:),      pointer :: res
    end function

    function basestring_cptr( ds ) result(res)
      import BaseString_t, c_ptr
      type(BaseString_t), intent(in) :: ds
      type(c_ptr)                    :: res
    end function

    subroutine basestring_cptr_c( res, bs )
      import BaseString_t, c_ptr
      type(c_ptr),        intent(inout) :: res
      type(BaseString_t)                :: bs
    end subroutine

    pure &
    function basestring_len_ref( bs ) result(res)
      import BaseString_t
      type(BaseString_t), intent(in) :: bs
      integer(kind=4)                :: res
    end function

    function basestring_reserve( bs, length ) result(res)
      import BaseString_t
      type(BaseString_t)             :: bs
      integer                        :: length
      character(len=length), pointer :: res
    end function

    subroutine basestring_trim( bs )
      import BaseString_t
      type(BaseString_t) :: bs
    end subroutine

    subroutine basestring_assign_charstring_c( bs, cs )
      import BaseString_t
      type(BaseString_t),      intent(inout) :: bs
      character(len=*), optional, intent(in) :: cs
    end subroutine

    subroutine basestring_assign_buf( lhs, rhs )
      import BaseString_t
      type(BaseString_t),             intent(inout) :: lhs
      character(len=1), dimension(:), intent(in)    :: rhs
    end subroutine

    subroutine basestring_assign_basestring_c( lhs, rhs )
      import BaseString_t
      type(BaseString_t), intent(inout) :: lhs
      type(BaseString_t),    intent(in) :: rhs
    end subroutine
  end interface

end module

