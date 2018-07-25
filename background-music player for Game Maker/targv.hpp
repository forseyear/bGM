#pragma once

#if defined(UNICODE) || defined(_UNICODE)
#define __targv __wargv
#else
#define __targv __argv
#endif
