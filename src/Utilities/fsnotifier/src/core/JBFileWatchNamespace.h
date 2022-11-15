#ifndef JBFILEWATCHNAMESPACE_H
#define JBFILEWATCHNAMESPACE_H

#include "JBLocalFileSystem.h"

namespace JB {

    using WatchRequest = JBFileWatchRequest;
    using FileWatcher = JBFileWatcher;
    using NativeFileWatcher = JBNativeFileWatcher;
    using WatchRootsManager = JBWatchRootsManager;
    using LocalFileSystem = JBLocalFileSystem;

    template <class _Kty, class _Ty, class _Pr = std::less<_Kty>,
              class _Alloc = std::allocator<std::pair<const _Kty, _Ty>>>
    using NavigableMap = JBNavigableMap<_Kty, _Ty, _Pr, _Alloc>;

    template <class _Kty, class _Pr = std::less<_Kty>, class _Alloc = std::allocator<_Kty>>
    using NavigableSet = JBNavigableSet<_Kty, _Pr, _Alloc>;

    using CanonicalPathMap = JBCanonicalPathMap;
    using DirtyPaths = JBFileWatcherDirtyPaths;

    namespace Utils = JBFileWatcherUtils;

} // namespace JB

#endif // JBFILEWATCHNAMESPACE_H
