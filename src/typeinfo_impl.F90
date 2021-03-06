
#include "fde/itfUtil.fpp"

module impl_typeinfo__
  use fde_typeinfo
  implicit none

  contains

  subroutine type_accept_wrap_( wrap, ti, vstr )
    use fde_visitor
    implicit none
    type(void_t)     :: wrap, dummy
    type(TypeInfo_t) :: ti
    type(Visitor_t)  :: vstr
    dummy%ptr => null()
    call vstr%visit( vstr, dummy, ti )
  end subroutine
  
  subroutine type_stream_wrap_( wrap, ti, outs )
    use fde_ostream
    use fde_convert
    use iso_c_binding
    implicit none
    type(void_t)     :: wrap
    type(TypeInfo_t) :: ti
    type(ostream_t)  :: outs
    ! do nothing - we can't stream void!
  end subroutine

end module

!_PROC_EXPORT(typespecs_object_size_c)
  integer(kind=4) &
    function typespecs_object_size_c() result(res)
    use impl_typeinfo__; implicit none
    type (TypeSpecs_t) :: tmp
    res = storage_size(tmp) / 8
  end function


  !**
  ! typeinfo_init initializes TypeInfo structure.
  ! @param self           - the TypeInfo to initialize
  ! @param typeId         - the type's id string (e.g. double)
  ! @param baseType       - the type's base string (e.g. real(kind=8))
  ! @param bitSize        - the storage size of the type in bytes (=> storage_size(type))
  ! @param rank           - the rank of the type
  ! @param subtype        - the typeinfo of this type's subtype (used by container types)
  ! @param acceptProc     - the subroutine to accept a visitors       : subroutine accept( obj, visitor )
  ! @param assignProc     - the subroutine to assign a variable       : subroutine assign( lhs, rhs )
  ! @param cloneObjProc   - the function to clone an object reference : subroutine clone( tgt, src )
  ! @param cloneRefProc   - the function to clone an object           : subroutine clone( tgt, src )
  ! @param deleteProc     - the subroutine to delete a variable       : subroutine delete( var )
  ! @param initProc       - the subroutine to initialize a variable   : subroutine init( var, hardness )
  ! @param shapeProc      - the function to inspect the shape         : subroutine shape( var, res, rank )
  ! @param streamProc     - the subroutine to stream type via charbuf : subroutine stream( var, flushFunc )
  ! @param tryStreamProc - the subroutine to determine stream length : subroutine tryStream( bufLen, status )
  !*
!_PROC_EXPORT(typeinfo_init)
  subroutine typeinfo_init( self, typeId, baseType, bitSize, rank, subtype, &
                            acceptProc, assignProc, cloneObjProc, cloneRefProc, deleteProc, initProc, shapeProc, streamProc, tryStreamProc )
    use impl_typeinfo__, only: TypeInfo_t, TypeSpecs_t, type_accept_wrap_, type_stream_wrap_
    use iso_c_binding
    implicit none
    type(TypeInfo_t),    intent(inout) :: self
    character(len=*),       intent(in) :: typeId, baseType
    integer(kind=4),        intent(in) :: bitSize
    integer(kind=4),        intent(in) :: rank
    type(TypeInfo_t), target, optional :: subtype
    procedure(),              optional :: acceptProc, assignProc, cloneObjProc, cloneRefProc, deleteProc, initProc, shapeProc &
                                        , streamProc, tryStreamProc

    self%typeId   =  adjustl(typeId);   self%typeId_term   = 0
    self%baseType =  adjustl(baseType); self%baseType_term = 0
    self%subtype  => subtype

    call link_memref( self%typeSpecs%typeId,   self%typeId )
    call link_memref( self%typeSpecs%baseType, self%baseType )

    self%typeSpecs%byteSize = bitSize / 8
    self%typeSpecs%rank     = rank
    self%typeSpecs%subtype  = c_loc(subtype)

    ! pre-initialize optional arguments
    self%acceptProc   => type_accept_wrap_ !< mandatory to assign default
    self%assignProc   => null()
    self%cloneObjProc => null()
    self%cloneRefProc => null()
    self%deleteProc   => null()
    self%initProc     => null()
    self%shapeProc    => null()
    self%streamProc   => type_stream_wrap_ !< mandatory to assign default

    if (present(acceptProc))   self%acceptProc   => acceptProc
    if (present(assignProc))   self%assignProc   => assignProc
    if (present(cloneObjProc)) self%cloneObjProc => cloneObjProc
    if (present(cloneRefProc)) self%cloneRefProc => cloneRefProc
    if (present(deleteProc))   self%deleteProc   => deleteProc
    if (present(initProc))     self%initProc     => initProc
    if (present(shapeProc))    self%shapeProc    => shapeProc
    if (present(streamProc))   self%streamProc   => streamProc

    ! try to find length of stream buffer ...
    if (present(tryStreamProc)) then
      call dim_streamBuffer()
    else if (associated( self%subtype )) then
      self%typeSpecs%streamLen = self%subtype%typeSpecs%streamLen
    end if
    self%initialized = .true.

  contains

    subroutine link_memref( memref, what )
      use fde_memoryref
      type(MemoryRef_t)        :: memref
      character(len=*), target :: what

      memref%loc = c_loc(what(1:1))
      memref%len = len_trim(what)
    end subroutine

    subroutine dim_streamBuffer()
      integer :: bufLen, status

      bufLen = 32; status = 1
      do while (.true.)
        bufLen = bufLen * 2
        call try_streaming( bufLen, status )
        if (status == 0) exit
      end do
    end subroutine
      
    subroutine try_streaming( bufLen, status )
      integer               :: bufLen, status
      character(len=bufLen) :: buffer

      !buffer = "" !< memory inspector wants us to initialize buffer ... ???
      call tryStreamProc( buffer, status )
      if (status == 0) then
        self%typeSpecs%streamLen = len_trim( buffer ) - 1
      end if
    end subroutine
  end subroutine


!_PROC_EXPORT(typeinfo_void_type)
  function typeinfo_void_type() result(res)
    use impl_typeinfo__, only: TypeInfo_t, typeinfo_init
    implicit none
    type(TypeInfo_t),      pointer :: res
    type(TypeInfo_t), save, target :: type_void
    res => type_void
    if (.not. type_void%initialized) &
      call typeinfo_init( type_void, "void", "", 0, 0 )
  end function


!_PROC_EXPORT(typeinfo_void_type_c)
  subroutine typeinfo_void_type_c( res )
    use impl_typeinfo__, only: void_type
    use iso_c_binding
    implicit none
    type(c_ptr) :: res
    res = c_loc(void_type())
  end subroutine

