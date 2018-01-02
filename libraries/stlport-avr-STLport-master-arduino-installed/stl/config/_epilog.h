#if defined (_STLP_MSVC) || defined (__ICL)

#  pragma warning (pop)
#  pragma pack (pop)

#elif defined (__BORLANDC__)

#  pragma option pop
#  pragma option -w-pow     // -w-8062 Previous options and warnings not restored

#elif defined (__IBMCPP__)

#  pragma info(restore)

#endif
