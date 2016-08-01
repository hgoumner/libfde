

######################################
class Stateful(object):
######################################
  """Mixin class extending ADTController types.
    
  Stateful provides cashed access to certain state scope, determined by option statePath

  """
  __opts__   = dict( statePath = '{rootId}' )
  __keysep__ = staticmethod(str.split)

  @property
  def state( self ):
    """return state scope, specified by option statePath."""
    try   : return self._stock._state
    except:
      self._stock._state = self._get_state()
      return self._stock._state


  def _get_state( self ):
    from adt.core import Scope
    path = self._statePath.format( **self.about ).split('/')
    return Scope.getProcessScope( *path )


  def setStateData( self, dataDict ):
    for ref, val in zip( self.getStateData( dataDict.keys() ), dataDict.values() ):
      ref.value = val
    return True


  def getStateData( self, keyList ):
    pairs = zip( *self.state.iterDomain( keyList, self.__keysep__ ) )
    return pairs and pairs[1] or []


  def setScopeKeySeparator( self, sep = None ):
    if sep: self.__keysep__ = lambda s: filter( bool, map( str.strip, s.split(sep) ) )
    else  : self.__dict__.pop( '__keysep__', None )

