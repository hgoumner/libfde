
module hash_map
  use abstract_list
  use var_item
  use dynamic_string
  use generic_ref
  implicit none
  private

  integer, parameter :: min_indexSize = 10
  integer, parameter :: max_indexSize = 100000


  type, public :: HashMapItem_t
    private
    type(DynamicString_t) :: key
    type(VarItem_t)       :: value
  end type


  type, public :: HashMap_t
    type(List_t), dimension(:), pointer :: indexVector    => null()
    integer*4                           :: items          =  0
    integer*4                           :: indexLimits(2) = (/ min_indexSize, max_indexSize /)
  end type

  
  type(List_t), target :: hm_nodeCache

  
  interface initialize ; module procedure hm_initialize, hm_initialize_default ; end interface
  interface clear      ; module procedure hm_clear                             ; end interface
  interface delete     ; module procedure hm_delete                            ; end interface

  public :: initialize
  public :: clear
  public :: delete

  !_TypeGen_declare_RefType( private, HashMapItem, type(HashMapItem_t), scalar )
  !_TypeGen_declare_ListItem( private, HashMapItem, type(HashMapItem_t), scalar )

  !_TypeGen_declare_RefType( public, HashMap, type(HashMap_t), scalar, \
  !     initProc   = hm_initialize_proto, \
  !     assignProc = hm_assign_hm,        \
  !     deleteProc = hm_delete,           \
  !     cloneMode  = _type )

  !_TypeGen_declare_ListItem( public, HashMap, type(HashMap_t), scalar )

contains

  subroutine hm_initialize_proto( self, has_proto, proto )
    type(HashMap_t)     :: self
    integer             :: has_proto
    type(HashMap_t)     :: proto
    integer*4           :: indexLimits(2)

    self%indexVector => null()
    self%items       =  0
    if (has_proto /= 0) then; indexLimits = proto%indexLimits
                        else; indexLimits = (/ min_indexSize, max_indexSize /)
    end if
    call hm_initialize( self, indexLimits(1), indexLimits(2) )
  end subroutine


  subroutine hm_initialize_default( self )
    type(HashMap_t), intent(inout) :: self
    call hm_initialize( self, min_indexSize, max_indexSize )
  end subroutine


  subroutine hm_initialize( self, index_min, index_max )
    type(HashMap_t), intent(inout) :: self
    integer                        :: index_min, index_max
    type(HashMapItem_t)            :: item

    self%indexLimits(1) = max(1, index_min)
    self%indexLimits(2) = max(1, index_min, index_max)
    if (.not. is_valid(hm_nodeCache)) &
      call initialize( hm_nodeCache, item_type(item%value) )
    call hm_setup_index( self, self%indexLimits(1) )
  end subroutine


  subroutine hm_setup_index( self, indexSize, tgtList )
    type(HashMap_t), intent(inout) :: self
    integer*4,          intent(in) :: indexSize
    type(List_t),         optional :: tgtList
    type(TypeInfo_t),      pointer :: itemType
    integer*4                      :: i

    call hm_clear( self, tgtList )
    if (indexSize /= size(self%indexVector)) then
      if (associated( self%indexVector )) &
        deallocate( self%indexVector )

      allocate( self%indexVector(0 : indexSize - 1) )
      itemType => dynamic_type( hm_nodeCache )
      !DEC$ parallel
      do i = 0, indexSize - 1
        call initialize( self%indexVector(i), itemType )
      end do
    end if
  end subroutine

  
  subroutine hm_clear( self, tgtList )
    type(HashMap_t), intent(inout) :: self
    type(List_t), optional, target :: tgtList
    type(List_t),          pointer :: tgt
    integer*4                      :: i
    
    if (associated( self%indexVector )) then
      if (present(tgtList)) then; tgt => tgtList
                            else; tgt => hm_nodeCache
      end if

      do i = 0, size( self%indexVector ) - 1
        call append( tgt, self%indexVector(i) )
      end do
      self%items = 0
    end if
  end subroutine


  subroutine hm_delete( self )
    type(HashMap_t), intent(inout) :: self

    if (associated( self%indexVector )) then
      call hm_clear( self )
      deallocate( self%indexVector )
    end if       
  end subroutine


  subroutine hm_assign_hm( lhs, rhs )
    type(HashMap_t), target, intent(inout) :: lhs
    type(HashMap_t), target,    intent(in) :: rhs
    integer*4                              :: i
   
    if (.not. associated( lhs%indexVector, rhs%indexVector )) then
      lhs%indexLimits = rhs%indexLimits
      call hm_setup_index( lhs, size(rhs%indexVector) )
      do i = 0, size( lhs%indexVector ) - 1
        !lhs%indexVector(i) = rhs%indexVector(i) !< too simple ... should reuse cached items
      end do
    end if
  end subroutine  


  subroutine hm_clear_cache()
    call clear( hm_nodeCache )
  end subroutine


  function hm_get_bucketIndex( self, key ) result(res)
    use hash_code
    type(HashMap_t),  intent(in) :: self
    character(len=*), intent(in) :: key
    type(ListIndex_t)            :: res
    integer                      :: idx, n
    
    n = size( self%indexVector )
    idx = mod( hash(key), n )
    ! have to fix negativ indices since fortran doesn't know unsigned integers >:(
    if (idx < 0) &
      idx = idx + n
    res = index( self%indexVector(idx) )
  end function


  logical &
  function hm_locate_item( self, key, idx ) result(res)
    type(HashMap_t), intent(inout) :: self
    character(len=*),   intent(in) :: key
    type(ListIndex_t), intent(out) :: idx
    type(HashMapItem_t),   pointer :: item
    
    if (self%indexLimits(1) < self%indexLimits(2)) &
      call hm_reindex( self )
    
    idx = hm_get_bucketIndex( self, key )
    do while (is_valid(idx))
      item => HashMapItem(idx)
      if (item%key /= key) then; call next(idx)
                           else; exit
      end if
    end do
    res = is_valid(idx)
  end function


  subroutine hm_set( self, key, val )
    type(HashMap_t), intent(inout) :: self
    character(len=*),   intent(in) :: key
    type(VarItem_t),    intent(in) :: val
    type(HashMapItem_t),   pointer :: item
    
    type(ListIndex_t)              :: idx

    if (.not. hm_locate_item( self, key, idx )) then
      call insert( idx, new_ListItem( item ) )
      item%key   = key
      self%items = self%items + 1
    else
      item => HashMapItem(idx)
    end if
    item%value = val
  end subroutine


  function newHashNode( self, key, valPtr ) result(res)
    type(HashMap_t)                                 :: self
    character(len=*),                    intent(in) :: key
    type(VarItem_t), optional, pointer, intent(out) :: valPtr
    type(Item_t),                           pointer :: res
    type(HashMapItem_t),                    pointer :: itemPtr
    
    res => new_ListItem( itemPtr )
  end function


  subroutine hm_reindex( self )
    type(HashMap_t), intent(inout) :: self
  end subroutine

  !_TypeGen_implementAll()

end

