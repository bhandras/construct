#pragma once


#ifndef foreach
#define foreach(__TYPE, __CONTAINER, __ITER) for (__TYPE::iterator __ITER = (__CONTAINER).begin(); __ITER != (__CONTAINER).end(); ++__ITER)
#endif

#ifndef foreach_const
#define foreach_const(__TYPE, __CONTAINER, __ITER) for (__TYPE::const_iterator __ITER = (__CONTAINER).begin(); __ITER != (__CONTAINER).end(); ++__ITER)
#endif
