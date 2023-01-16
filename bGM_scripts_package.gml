#define bGM_startup
{
   /********************************************************************************
    * (ver 1.0.0)
    *
    * bGM_startup()
    *
    * bGMスクリプトの初期化を行う。
    * bGM_play関数を呼ぶ前に、この関数を実行する必要がある。
    * ゲーム開始後に1度だけ実行されれば十分である。
    *
    * return        : 初期化に成功するとtrue、失敗するとfalseを返す。
    ********************************************************************************/

   /* ------------------- 以下は、ゲームに応じて設定できる項目 ------------------- */

   // ゲームの作業ディレクトリからbGM.exeまでの相対パス、もしくはフルパス
   // ディレクトリの区切りは「/」でも「\」でも可
   global.bGM_exe_filepath = "bGM/bGM.exe";

   /* ------------------- 以下、変更不可 ------------------- */

   // backslashで統一
   global.bGM_exe_filepath = string_replace_all(global.bGM_exe_filepath, "/", "\");

   // bGM.exeが見つからない
   if (!file_exists(global.bGM_exe_filepath))
   {
      return false;
   }

   // 既にこの関数が呼ばれている場合、なにもしない
   if (variable_global_exists("bGM_startup_has_called"))
   {
      return true;
   }

   // ゲームのプロセスを判別するためのunique idを発行する
   global.bGM_1time_game_id = string(round(random(1 << 20) + game_id + current_time));

   // サウンドハンドルの準備
   global.bGM_sound_controll_id = 0;

   // 改行コード [depricated]
   // global.bGM_CRLF = chr(13) + chr(10);

   // デバッグコンソールを有効にするか
   global.bGM_enable_debugging_console = false;

   // この関数が呼ばれたことを覚えておく
   global.bGM_startup_has_called = true;

   // 初期化に成功
   return true;
}
#define bGM_play
{
   /********************************************************************************
    * (ver 1.0.0)
    *
    * bGM_play(filename, volume, is_loop, pan, pitch)
    *
    * filenameで指定された音楽ファイルをvolumeで指定された音量で再生開始する。
    *
    * -string  filename  : 再生するファイル名(mp3/ogg/mod/it/xm/s3mのいずれか)
    *                    : ゲームの作業ディレクトリからの相対パス、もしくはフルパス。
    *                    : 半角スペースが含まれていてもかまわない。
    * -double  volume    : 音量(0.0～1.0で指定する。省略不可)
    *                      ※volume/pan/pitchは、0.000001単位で指定できる。
    * -bool    is_loop   : ループ再生を行うかどうか(省略可。デフォルト値 = false)
    * -double  pan       : パン(-1.0で左端、0.0で中央、1.0で右端。省略可。デフォルト値 = 0.0)
    * -double  pitch     : 再生スピード(0.5～2.0で指定する。省略可。デフォルト値 = 1.0)
    *
    * return        : 再生開始に成功するとサウンドid(1以上の整数)を返す。
    *               : このサウンドidを使って停止などを行うことができる。
    *               : 再生開始に失敗するとfalseを返す。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   // bGM_startup関数が呼ばれていない
   if (!variable_global_exists("bGM_startup_has_called"))
   {
      return false;
   }

   var filename, is_loop, volume, pan, pitch;     // 引数をキャッチする変数
   var arg_index;                                 // 引数走査index
   var cmd_args;                                  // bGM.exeへ渡すコマンドライン引数

   // ファイル名
   arg_index = 0;
   if (is_string(argument[arg_index]))
   {
      filename = argument[arg_index];
   }
   else
   {
      return false;
   }

   // ボリューム
   arg_index = 1;
   if (is_real(argument[arg_index]))
   {
      volume = max(0.0, min(1.0, argument[arg_index]));
   }
   else
   {
      volume = 0.5;
   }
   // ループ
   arg_index = 2;
   if (argument[arg_index])
   {
      is_loop = true;
   }
   else
   {
      is_loop = false;
   }
   // パン
   arg_index = 3;
   if (is_real(argument[arg_index]))
   {
      pan = max(-1.0, min(1.0, argument[arg_index]));
   }
   else
   {
      pan = 0.0;
   }
   // ピッチシフト
   arg_index = 4;
   if (is_real(argument[arg_index]) && argument[arg_index] != 0)
   {
      pitch = max(0.5, min(2.0, argument[arg_index]));
   }
   else
   {
      pitch = 1.0;
   }

   // サウンドidの割り当て
   global.bGM_sound_controll_id += 1;

   // コマンドライン引数の組み立て
   cmd_args =
               '-f "' + filename + '"' +
               " -v " + string_format(volume, 7, 6) +
               " -p " + string_format(pan, 7, 6) +
               " -s " + string_format(pitch, 7, 6) +
               " -w " + string(window_handle()) +
               " -i " + bGM_private_generate_pid(global.bGM_sound_controll_id);
   if (is_loop)
   {
      cmd_args += " -L";
   }
   if (global.bGM_enable_debugging_console)
   {
      cmd_args += " -e";
   }

   // bGMを実行。
   execute_program(global.bGM_exe_filepath, cmd_args, false);

   return global.bGM_sound_controll_id;
}
#define bGM_pause
{
   /********************************************************************************
    * (ver 1.0.0)
    *
    * bGM_pause(sound_id)
    *
    * サウンドを一時停止させる。
    * あるいは、すでに一時停止状態だった場合、再生を再開させる。
    *
    * -int   sound_id    : 一時停止/再開させたいサウンドid (bGM_playの戻り値)
    *
    * return        : 戻り値無し。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */
   bGM_private_controll_1arg(argument0, "o");
   exit;
}
#define bGM_stop
{
   /********************************************************************************
    * (ver 1.0.0)
    *
    * bGM_stop(sound_id)
    *
    * サウンドの再生を停止させる。
    *
    * -int   sound_id    : 停止させたいサウンドid (bGM_playの戻り値)
    *
    * return        : 戻り値無し。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   bGM_private_controll_1arg(argument0, "m");
   exit;
}
#define bGM_reset
{
   /********************************************************************************
    * (ver 1.0.0)
    *
    * bGM_reset(sound_id)
    *
    * サウンドの再生位置を先頭にリセットさせる。
    * サウンドが再生中であった場合、リセット後でも再生は続行される。
    *
    * -int   sound_id    : リセットさせたいサウンドid (bGM_playの戻り値)
    *
    * return        : 戻り値無し。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   bGM_private_controll_1arg(argument0, "r");
   exit;
}
#define bGM_close
{
   /********************************************************************************
    * (ver 1.0.0)
    *
    * bGM_close(sound_id)
    *
    * サウンドを完全停止させ、ファイルも閉じさせる。
    * この後はもちろんsound_idも無効となる。
    *
    * -int   sound_id    : 完全停止させたいサウンドid (bGM_playの戻り値)
    *
    * return        : 戻り値無し。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   bGM_private_controll_1arg(argument0, "d");
   exit;
}
#define bGM_set_volume
{
   /********************************************************************************
    * (ver 1.0.0)
    *
    * bGM_set_volume(sound_id, volume)
    *
    * サウンドの音量を変更させる。
    *
    * -int   sound_id    : 音量を調節したいサウンドid (bGM_playの戻り値)
    * -double  volume    : 音量(0.0～1.0で指定する)
    *
    * return        : 戻り値無し。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   bGM_private_controll_1arg(argument0, "v " + string_format(argument1, 7, 6));
   exit;
}
#define bGM_set_loop
{
   /********************************************************************************
    * (ver 1.0.0)
    *
    * bGM_set_loop(sound_id, is_loop)
    *
    * サウンドのループ再生を切り替える。
    *
    * -int   sound_id    : 対象のサウンドid (bGM_playの戻り値)
    * -bool    is_loop   : ループ再生を行うかどうか
    *
    * return        : 戻り値無し。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   if (argument1)
   {
      bGM_private_controll_1arg(argument0, "L");
   }
   else
   {
      bGM_private_controll_1arg(argument0, "u");
   }
   exit;
}
#define bGM_set_pan
{
   /********************************************************************************
    * (ver 1.0.0)
    *
    * bGM_set_pan(sound_id, pan)
    *
    * サウンドのパン(左右の音量の比率)を変更させる。
    *
    * -int   sound_id    : 対象のサウンドid (bGM_playの戻り値)
    * -double  pan       : パンの数値(-1.0で左端、0.0で中央、1.0で右端)
    *
    * return        : 戻り値無し。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   bGM_private_controll_1arg(argument0, "p " + string_format(argument1, 7, 6));
   exit;
}
#define bGM_set_pitch
{
   /********************************************************************************
    * (ver 1.0.0)
    *
    * bGM_set_pitch(sound_id, pitch)
    *
    * サウンドの再生スピード(ピッチシフト値)を変更させる。
    *
    * -int   sound_id    : 対象のサウンドid (bGM_playの戻り値)
    * -double  pitch     : 再生スピード(0.5～2.0で指定する)
    *
    * return        : 戻り値無し。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   bGM_private_controll_1arg(argument0, "s " + string_format(argument1, 7, 6));
   exit;
}
#define bGM_switch_debug
{
   /********************************************************************************
    * (ver 1.0.0)
    *
    * bGM_switch_debug(is_debug)
    *
    * この関数にtrueが渡された後に bGM_play関数が呼ばれると、デバック用の
    * ウィンドウが表示される。
    * このデバッグ用ウィンドウは、再生開始までにかかる時間を調べるために、
    * または、サウンド再生に失敗する場合の状況把握に有用である。音量やその他の値も
    * リアルタイムに表示される。
    * この関数は、bGM_startup関数よりも後に実行されないと意味がないので注意。
    *
    * -bool  is_debug    : デバッグコンソールを表示するならtrue、無効にするならfalse
    *
    * return        : 戻り値無し。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   // デバッグコンソールを有効化/無効化
   global.bGM_enable_debugging_console = argument0;
   exit;
}
#define bGM_private_controll_1arg
{
   /* ------------------- bGM Scriptsの内部から呼ばれるプライベート関数 ------------------- */

   // 再生プロセスへメッセージをポストする

   var sound_id, arg;                             // 引数をキャッチする変数
   var cmd_args;                                  // bGM.exeへ渡すコマンドライン引数

   sound_id = argument0;
   arg      = argument1;

   // コマンドライン引数の組み立て
   cmd_args = "-t " + bGM_private_generate_pid(sound_id) + " -" + arg;

   // 実行。
   execute_program(global.bGM_exe_filepath, cmd_args, false);

   exit;
}
#define bGM_private_generate_pid
{
   /* ------------------- bGM Scriptsの内部から呼ばれるプライベート関数 ------------------- */

   // サウンド再生を行っているbGMプロセスのIDを返す
   return "GAME=" + string(global.bGM_1time_game_id) + ":SND=" + string(argument0);
}
#define bGM_play_midi
{
   /********************************************************************************
    * (ver 2.2.1)
    *
    * bGM_play_midi(filename, is_loop)
    *
    * filenameで指定されたmidiファイルを再生開始する。
    *
    * -string  filename  : 再生するmidiファイル名(midi/midのいずれか)
    *                    : ゲームの作業ディレクトリからの相対パス、もしくはフルパス。
    *                    : 半角スペースが含まれていてもかまわない。
    * -bool    is_loop   : ループ再生を行うかどうか(省略可。デフォルト値 = false)
    *
    * return        : 再生開始に成功するとサウンドid(1以上の整数)を返す。
    *               : このサウンドidを使って停止などを行うことができる。
    *               : また、bGM_pause関数を使って再生ができる。
    *               : 再生開始に失敗するとfalseを返す。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */
   // bGM_startup関数が呼ばれていない
   if (!variable_global_exists("bGM_startup_has_called"))
   {
      return false;
   }

   var filename, is_loop;  // 引数をキャッチする変数
   var arg_index;          // 引数走査index
   var cmd_args;           // bGM.exeへ渡すコマンドライン引数

   // ファイル名
   arg_index = 0;
   if (is_string(argument[arg_index]))
   {
      filename = argument[arg_index];
   }
   else
   {
      return false;
   }
   // ループ
   arg_index = 1;
   if (argument[arg_index])
   {
      is_loop = true;
   }
   else
   {
      is_loop = false;
   }

   // サウンドidの割り当て
   global.bGM_sound_controll_id += 1;

   // コマンドライン引数の組み立て
   cmd_args =
               '-f "' + filename + '"' +
               " -w " + string(window_handle()) +
               " -i " + bGM_private_generate_pid(global.bGM_sound_controll_id) +
               " -M";
   if (is_loop)
   {
      cmd_args += " -L";
   }
   if (global.bGM_enable_debugging_console)
   {
      cmd_args += " -e";
   }

   // bGMを実行。
   execute_program(global.bGM_exe_filepath, cmd_args, false);

   return global.bGM_sound_controll_id;
}
#define bGM_load
{
   /********************************************************************************
    * (ver 2.0.0)
    *
    * bGM_load(filename, volume, is_loop, pan, pitch)
    *
    * filenameで指定された音楽ファイルをvolumeで指定された音量で再生待機する。
    *
    * -string  filename  : 再生するファイル名(mp3/ogg/mod/it/xm/s3mのいずれか)
    *                    : ゲームの作業ディレクトリからの相対パス、もしくはフルパス。
    *                    : 半角スペースが含まれていてもかまわない。
    * -double  volume    : 音量(0.0～1.0で指定する。省略不可)
    *                      ※volume/pan/pitchは、0.000001単位で指定できる。
    * -bool    is_loop   : ループ再生を行うかどうか(省略可。デフォルト値 = false)
    * -double  pan       : パン(-1.0で左端、0.0で中央、1.0で右端。省略可。デフォルト値 = 0.0)
    * -double  pitch     : 再生スピード(0.5～2.0で指定する。省略可。デフォルト値 = 1.0)
    *
    * return        : 再生待機に成功するとサウンドid(1以上の整数)を返す。
    *               : このサウンドidを使って停止などを行うことができる。
    *               : また、bGM_pause関数を使って再生ができる。
    *               : 再生待機に失敗するとfalseを返す。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */
   // bGM_startup関数が呼ばれていない
   if (!variable_global_exists("bGM_startup_has_called"))
   {
      return false;
   }

   var filename, is_loop, volume, pan, pitch;     // 引数をキャッチする変数
   var arg_index;                                 // 引数走査index
   var cmd_args;                                  // bGM.exeへ渡すコマンドライン引数

   // ファイル名
   arg_index = 0;
   if (is_string(argument[arg_index]))
   {
      filename = argument[arg_index];
   }
   else
   {
      return false;
   }

   // ボリューム
   arg_index = 1;
   if (is_real(argument[arg_index]))
   {
      volume = max(0.0, min(1.0, argument[arg_index]));
   }
   else
   {
      volume = 0.5;
   }
   // ループ
   arg_index = 2;
   if (argument[arg_index])
   {
      is_loop = true;
   }
   else
   {
      is_loop = false;
   }
   // パン
   arg_index = 3;
   if (is_real(argument[arg_index]))
   {
      pan = max(-1.0, min(1.0, argument[arg_index]));
   }
   else
   {
      pan = 0.0;
   }
   // ピッチシフト
   arg_index = 4;
   if (is_real(argument[arg_index]) && argument[arg_index] != 0)
   {
      pitch = max(0.5, min(2.0, argument[arg_index]));
   }
   else
   {
      pitch = 1.0;
   }

   // サウンドidの割り当て
   global.bGM_sound_controll_id += 1;

   // コマンドライン引数の組み立て
   cmd_args =
               '-f "' + filename + '"' +
               " -v " + string_format(volume, 7, 6) +
               " -p " + string_format(pan, 7, 6) +
               " -s " + string_format(pitch, 7, 6) +
               " -w " + string(window_handle()) +
               " -i " + bGM_private_generate_pid(global.bGM_sound_controll_id);
   if (is_loop)
   {
      cmd_args += " -L";
   }
   if (global.bGM_enable_debugging_console)
   {
      cmd_args += " -e";
   }
   cmd_args += " -z";

   // bGMを実行。
   execute_program(global.bGM_exe_filepath, cmd_args, false);

   return global.bGM_sound_controll_id;
}
#define bGM_load_midi
{
   /********************************************************************************
    * (ver 2.2.1)
    *
    * bGM_load_midi(filename, is_loop)
    *
    * filenameで指定されたmidiファイルを再生待機する。
    *
    * -string  filename  : 再生するmidiファイル名(midi/midのいずれか)
    *                    : ゲームの作業ディレクトリからの相対パス、もしくはフルパス。
    *                    : 半角スペースが含まれていてもかまわない。
    * -bool    is_loop   : ループ再生を行うかどうか(省略可。デフォルト値 = false)
    *
    * return        : 再生待機に成功するとサウンドid(1以上の整数)を返す。
    *               : このサウンドidを使って停止などを行うことができる。
    *               : また、bGM_pause関数を使って再生ができる。
    *               : 再生待機に失敗するとfalseを返す。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */
   // bGM_startup関数が呼ばれていない
   if (!variable_global_exists("bGM_startup_has_called"))
   {
      return false;
   }

   var filename, is_loop;     // 引数をキャッチする変数
   var arg_index;             // 引数走査index
   var cmd_args;              // bGM.exeへ渡すコマンドライン引数

   // ファイル名
   arg_index = 0;
   if (is_string(argument[arg_index]))
   {
      filename = argument[arg_index];
   }
   else
   {
      return false;
   }
   // ループ
   arg_index = 1;
   if (argument[arg_index])
   {
      is_loop = true;
   }
   else
   {
      is_loop = false;
   }

   // サウンドidの割り当て
   global.bGM_sound_controll_id += 1;

   // コマンドライン引数の組み立て
   cmd_args =
               '-f "' + filename + '"' +
               " -w " + string(window_handle()) +
               " -i " + bGM_private_generate_pid(global.bGM_sound_controll_id) +
               " -M";
   if (is_loop)
   {
      cmd_args += " -L";
   }
   if (global.bGM_enable_debugging_console)
   {
      cmd_args += " -e";
   }
   cmd_args += " -z";

   // bGMを実行。
   execute_program(global.bGM_exe_filepath, cmd_args, false);

   return global.bGM_sound_controll_id;
}
#define bGM_add_repeat
{

   /********************************************************************************
    * (ver 2.0.0)
    *
    * bGM_add_repeat(sound_id, location, target)
    *
    * サウンドにループポイントを追加する。
    *
    * -int   sound_id                     : 対象のサウンドid (bGM_playの戻り値)
    * -int   location (repeat_point_B)    : ループが発生する場所(0～999,999,999で指定する)(大きい)
    * -int    target  (repeat_point_A)    : ジャンプ先の場所(0～999,999,999で指定する)(小さい)
    *
    * return        : 戻り値無し。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   bGM_private_controll_1arg(argument0, "a " + string_format(argument1, 9, 0) + " -b " + string_format(argument2, 9, 0));
   exit;

}

#define bGM_set_repeat_count
{

   /********************************************************************************
    * (ver 2.1.0)
    *
    * bGM_set_repeat_count(sound_id, repeat_count)
    *
    * サウンドの繰り返し回数を決める。
    *
    * -int   sound_id        : 対象のサウンドid (bGM_playの戻り値)
    * -int   repeat_count    : 繰り返す回数
    *
    * return        : 戻り値無し。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   bGM_private_controll_1arg(argument0, "g " + string_format(argument1, 2, 0));
   exit;

}
#define bGM_try_repeat
{

   /********************************************************************************
    * (ver 2.0.0)
    *
    * bGM_try_repeat(sound_id)
    *
    * 追加したループポイントの確認ができる。
    *
    * -int   sound_id    : 対象のサウンドid (bGM_playの戻り値)
    *
    * return        : 戻り値無し。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   bGM_private_controll_1arg(argument0, "c");
   exit;

}

#define bGM_remove_repeat
{

   /********************************************************************************
    * (ver 2.0.0)
    *
    * bGM_remove_repeat(sound_id)
    *
    * 追加したループポイントを削除する。
    *
    * -int   sound_id    : 対象のサウンドid (bGM_playの戻り値)
    *
    * return        : 戻り値無し。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   bGM_private_controll_1arg(argument0, "x");
   exit;

}
#define bGM_fade
{

   /********************************************************************************
    * (ver 2.0.0)
    *
    * bGM_fade(sound_id, is_fade, sec, volume)
    *
    * サウンドにフェード効果を追加する。
    *
    * -int     sound_id    : 対象のサウンドid (bGM_playの戻り値)
    * -bool    is_fade     : フェードインにはtrue、フェードアウトにはfalseを指定する
    * -int       sec       : フェードの合計時間 (単位はミリ秒。)
    * -double  volume      : 音量(0.0～1.0で指定する。省略可。)
    *                        デフォルト値は、is_fadeがtrueの場合は1.0、falseの場合は0.0
    *                        volumeは、0.000001単位で指定できる。
    *
    * return        : 戻り値無し。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */
   var volume;

   if (is_real(argument3))
   {
      volume = max(0.0, min(1.0, argument3));
   }
   else
   {
      if(argument1)
      {
         volume = 1.000000;
      }
      else
      {
         volume = 0.000000;
      }
   }

   if (argument1)
   {
      bGM_private_controll_1arg(argument0, "V " + string_format(volume, 7, 6) + " -h " + string_format(argument2, 5, 0));
   }
   else
   {
      bGM_private_controll_1arg(argument0, "V " + string_format(volume, 7, 6) + " -q " + string_format(argument2, 5, 0));
   }
   exit;

}
#define bGM_set_position
{
   /********************************************************************************
    * (ver 2.0.0)
    *
    * bGM_set_position(sound_id, position)
    *
    * サウンドの場所を変更させる。
    *
    * -int   sound_id    : 場所を調整したいサウンドid (bGM_playの戻り値)
    * -int   position    : 場所(0～999,999,999で指定する)
    *
    * return        : 戻り値無し。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   bGM_private_controll_1arg(argument0, "k " + string_format(argument1, 9, 0));
   exit;
}
#define bGM_get_position
{

   /********************************************************************************
    * (ver 2.0.0)
    *
    * bGM_get_position(sound_id)
    *
    * サウンドの場所を確認する。再生中に呼び出す場合はラグに注意。
    *
    * -int   sound_id    : 対象のサウンドid (bGM_playの戻り値)
    *
    * return        : 成功するとサウンドの場所(int)を返す。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   // 再生プロセスへメッセージをポストする

   var f;                                         // ファイル変数
   var sound_id, arg, number;                     // 引数をキャッチする変数
   var cmd_args;                                  // bGM.exeへ渡すコマンドライン引数

   sound_id = argument0;
   arg      = "j";

   // コマンドライン引数の組み立て
   cmd_args = "-t " + bGM_private_generate_pid(sound_id) + " -" + arg;

   // 実行。
   execute_program(global.bGM_exe_filepath, cmd_args, true);

   // ファイルパスを作成
   global.bGM_gpo_file_path = "GetPositionTemp";

   if(!file_exists(global.bGM_gpo_file_path))
   {
      return false;
   }

   // ファイルから戻り値を読み取る
   f = file_text_open_read(global.bGM_gpo_file_path);
   number = file_text_read_real(f);
   file_text_close(f);

   // ファイルを削除する
   file_delete(global.bGM_gpo_file_path);

   // 値を返す
   return number;

}
#define bGM_get_length
{

   /********************************************************************************
    * (ver 2.0.0)
    *
    * bGM_get_length(sound_id)
    *
    * サウンドの長さを取得する。
    *
    * -int   sound_id    : 対象のサウンドid (bGM_playの戻り値)
    *
    * return        : 成功するとサウンドの長さ(int)を返す。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   // 再生プロセスへメッセージをポストする

   var f;                                         // ファイル変数
   var sound_id, arg, number;                     // 引数をキャッチする変数
   var cmd_args;                                  // bGM.exeへ渡すコマンドライン引数

   sound_id = argument0;
   arg      = "l";

   // コマンドライン引数の組み立て
   cmd_args = "-t " + bGM_private_generate_pid(sound_id) + " -" + arg;

   // 実行。
   execute_program(global.bGM_exe_filepath, cmd_args, true);

   // ファイルパスを作成
   global.bGM_gln_file_path = "GetLengthTemp";

   if(!file_exists(global.bGM_gln_file_path))
   {
      return false;
   }

   // ファイルから戻り値を読み取る
   f = file_text_open_read(global.bGM_gln_file_path);
   number = file_text_read_real(f);
   file_text_close(f);

   // ファイルを削除する
   file_delete(global.bGM_gln_file_path);

   // 値を返す
   return number;

}
#define bGM_is_opening
{

   /********************************************************************************
    * (ver 2.0.0)
    *
    * bGM_is_opening(sound_id)
    *
    * サウンドウィンドウの状態を確認する。
    *
    * -int   sound_id    : 対象のサウンドid (bGM_playの戻り値)
    *
    * return        : サウンドを開いているとtrueを返す。
    *               : 通常はfalseを返す。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   // 再生プロセスへメッセージをポストする

   var f;                                         // ファイル変数
   var sound_id, arg, key;                        // 引数をキャッチする変数
   var cmd_args;                                  // bGM.exeへ渡すコマンドライン引数

   sound_id = argument0;
   arg      = "n";

   // コマンドライン引数の組み立て
   cmd_args = "-t " + bGM_private_generate_pid(sound_id) + " -" + arg;

   // 実行。
   execute_program(global.bGM_exe_filepath, cmd_args, true);

   // ファイルパスを作成
   global.bGM_iso_file_path = "isOpenTemp";

   if(!file_exists(global.bGM_iso_file_path))
   {
      return false;
   }

   // ファイルから戻り値を読み取る
   f = file_text_open_read(global.bGM_iso_file_path);
   key = file_text_read_real(f);
   file_text_close(f);

   // ファイルを削除する
   file_delete(global.bGM_iso_file_path);

   // 値を返す
   if(key == 1)
   {
      return true;
   }
   return false;

}
#define bGM_is_playing
{

   /********************************************************************************
    * (ver 2.0.0)
    *
    * bGM_is_playing(sound_id)
    *
    * サウンドの再生状態を確認する。
    *
    * -int   sound_id    : 対象のサウンドid (bGM_playの戻り値)
    *
    * return        : サウンドを再生しているとtrueを返す。
    *               : 停止中はfalseを返す。
    ********************************************************************************/

   /* ------------------- 以下、変更不可 ------------------- */

   // 再生プロセスへメッセージをポストする

   var f;                                         // ファイル変数
   var sound_id, arg, key;                        // 引数をキャッチする変数
   var cmd_args;                                  // bGM.exeへ渡すコマンドライン引数

   sound_id = argument0;
   arg      = "y";

   // コマンドライン引数の組み立て
   cmd_args = "-t " + bGM_private_generate_pid(sound_id) + " -" + arg;

   // 実行。
   execute_program(global.bGM_exe_filepath, cmd_args, true);

   // ファイルパスを作成
   global.bGM_isp_file_path = "isPlayTemp";

   if(!file_exists(global.bGM_isp_file_path))
   {
      return false;
   }

   // ファイルから戻り値を読み取る
   f = file_text_open_read(global.bGM_isp_file_path);
   key = file_text_read_real(f);
   file_text_close(f);

   // ファイルを削除する
   file_delete(global.bGM_isp_file_path);

   // 値を返す
   if(key == 1)
   {
      return true;
   }
   return false;

}
