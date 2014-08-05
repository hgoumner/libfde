
module generic_ref
  use base_string
  use type_info
  use iso_c_binding
  implicit none
  private


  type, public :: GenericRef_t
    private
    type(BaseString_t)        :: ref_str
    type(TypeInfo_t), pointer :: typeInfo => null()
  end type

  ! interface definitions

  interface assignment(=)
    module procedure gr_assign_gr
  end interface

  interface rank    ; module procedure gr_rank        ; end interface
  interface shape   ; module procedure gr_shape       ; end interface
  interface clone   ; module procedure gr_clone       ; end interface
  interface cptr    ; module procedure gr_cptr        ; end interface
  interface delete  ; module procedure gr_delete      ; end interface
  interface free    ; module procedure gr_free        ; end interface

  ! declare public interfaces 

  public :: assignment(=)
  public :: gr_set_TypeReference  !< needed by generated code
  public :: gr_get_TypeReference  !<            "
  public :: rank
  public :: shape
  public :: clone
  public :: cptr
  public :: delete
  public :: free

  !_TypeReference_declare( public, ref, type(GenericRef_t), scalar, \
  !     assignProc = gr_assign_gr, \
  !     deleteProc = gr_delete,    \
  !     cloneProc  = ref_cloner,  \
  !     derefName  = deref )

!-----------------
  contains
!-----------------

  !_TypeReference_implementAll()


  subroutine gr_assign_gr( lhs, rhs )
    type(GenericRef_t), intent(inout) :: lhs
    type(GenericRef_t),    intent(in) :: rhs

    call bs_assign_bs( lhs%ref_str, rhs%ref_str )
    lhs%typeInfo => rhs%typeInfo
  end subroutine


  subroutine gr_set_TypeReference( self, cptr, bits, ti )
    type(GenericRef_t), intent(inout) :: self
    type(c_ptr),           intent(in) :: cptr
    integer*4,             intent(in) :: bits
    type(TypeInfo_t),          target :: ti
    character(len=bits/8),    pointer :: fptr

    call c_f_pointer( cptr, fptr )
    call bs_set_attribute( self%ref_str, attrib_volatile )
    call bs_assign_cs( self%ref_str, fptr )
    self%typeInfo => ti
  end subroutine


  function gr_get_TypeReference( self ) result(res)
    type(GenericRef_t), intent(in) :: self
    type(c_ptr)                    :: res
    res = bs_cptr( self%ref_str )
  end function


  pure function gr_rank( self ) result(res)
    type(GenericRef_t), intent(in) :: self
    integer                        :: res

    if (associated( self%typeInfo )) then
      res = self%typeInfo%rank
    else
      res = 0
    end if
  end function


  pure function gr_shape( self ) result(res)
    type(GenericRef_t), intent(in) :: self
    integer                        :: res(rank(self))

    if (associated( self%typeInfo )) then
      if (associated( self%typeInfo%shapeProc )) then
        call self%typeInfo%shapeProc( self, res, self%typeInfo%rank )
        return
      end if
    end if
    res = 0
  end function


  function gr_clone( self ) result(res)
    type(GenericRef_t), intent(in) :: self
    type(GenericRef_t)             :: res

    call bs_set_attribute( res%ref_str, attrib_volatile )
    if (associated( self%typeInfo )) then
      if (associated( self%typeInfo%cloneProc )) then
        call self%typeInfo%cloneProc( self, res )
        return
      end if
    end if
    res = self
  end function


  function gr_cptr( self ) result(res)
    type(GenericRef_t), intent(in) :: self
    type(c_ptr)                    :: res
    type(void_t),          pointer :: wrap

    res = gr_get_TypeReference(self)
    if (c_associated(res)) then
      call c_f_pointer( res, wrap )
      res = c_loc(wrap%ptr)
    end if
  end function


  subroutine gr_delete( self )
    type(GenericRef_t) :: self

    call bs_delete( self%ref_str )
    self%typeInfo => null()
  end subroutine


  subroutine gr_free( self )
    type(GenericRef_t)    :: self
    type(void_t), pointer :: wrap
    type(c_ptr)           :: cptr

    cptr = gr_get_TypeReference(self)
    if (c_associated( cptr )) then
      call c_f_pointer( cptr, wrap )

      if (associated( wrap%ptr )) then
        if (associated( self%typeInfo )) then
          if (associated( self%typeInfo%deleteProc )) &
            call self%typeInfo%deleteProc( wrap%ptr )
        end if
        deallocate( wrap%ptr )
      end if
    end if
    call delete( self )
  end subroutine


  function ref_cloner( val ) result(res)
    type(GenericRef_t), intent(in) :: val
    type(GenericRef_t),    pointer :: res
    allocate( res ) !< initializes res as default GenericRef
    res = val
  end function

end module
