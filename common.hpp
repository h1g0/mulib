#ifndef MULIB_COMMON_HPP
#define MULIB_COMMON_HPP

//constexpr���g����R���p�C�����H
#ifndef MULIB_USE_COMSTEXPR
#ifdef _MSVC_VER
#if _MSVC_VER >= 1700
#define MULIB_USE_CONSTEXPR
#endif
#endif

#ifdef __GNUC__
#if __GNUC_PREREQ(4,6)
#define MULIB_USE_CONSTEXPR
#endif
#endif
#endif

#endif