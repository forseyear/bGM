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
