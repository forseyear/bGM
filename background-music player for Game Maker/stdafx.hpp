// stdafx.hpp : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#include "targetver.hpp"

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーから使用されていない部分を除外します。
// Windows ヘッダー ファイル:
#include <windows.h>

// C ランタイム ヘッダー ファイル
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <memory>
#include <future>
#include <algorithm>
#include <tchar.h>

// TODO: プログラムに必要な追加ヘッダーをここで参照してください
#include "tcout.hpp"
#include "tcerr.hpp"
#include "tstring.hpp"
#include "targv.hpp"
