#ifndef __COMMON_GLOBAL_H__
#define __COMMON_GLOBAL_H__

#ifdef _WIN32

#ifdef COMMON_LIB
# define COMMON_EXPORT __declspec(dllexport)
#else
# define COMMON_EXPORT __declspec(dllimport)
#endif

#else
# define COMMON_EXPORT
#endif // _WIN32

#define DS_DISABLE_COPY(Class) \
    Class(const Class &); \
    Class &operator=(const Class &);

//////////////////////////////////////////////////////////////////////////
template <typename T> static inline T *DSGetPtrHelper(T* pPtr) { return pPtr; }

#define DS_DECLARE_PRIVATE(Class) \
	inline Class##Private* d_func() { return reinterpret_cast<Class##Private *>(DSGetPtrHelper(d_ptr)); } \
	inline const Class##Private* d_func() const { return reinterpret_cast<const Class##Private *>(DSGetPtrHelper(d_ptr)); } \
	friend class Class##Private;

#define DS_DECLARE_PUBLIC(Class)                                    \
	inline Class* q_func() { return static_cast<Class *>(q_ptr); } \
	inline const Class* q_func() const { return static_cast<const Class *>(q_ptr); } \
	friend class Class;

#define DS_D(Class) Class##Private * const d = d_func()
#define DS_Q(Class) Class * const q = q_func()


#define DS_UNUSED(x) (void)x;

#endif // __COMMON_GLOBAL_H__
