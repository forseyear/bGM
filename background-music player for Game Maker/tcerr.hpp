#pragma once

#if defined(UNICODE) || defined(_UNICODE)
#define tcerr std::wcerr
#else
#define tcerr std::cerr
#endif
